/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	CVulkanDevice::CVulkanDevice(CVulkan *pVulkan)
		: m_pVulkan(pVulkan)
		, m_vkDevice(VK_NULL_HANDLE)
		, m_vkPhysicalDevice(VK_NULL_HANDLE)

		, m_pQueue(NULL)
		, m_pFenceManager(NULL)
		, m_pSemaphoreManager(NULL)
		, m_pMemoryManager(NULL)
		, m_pStagingBufferManager(NULL)
		, m_pCommandBufferManager(NULL)
		, m_pDescriptorSetManager(NULL)
		, m_pDescriptorSetLayoutManager(NULL)

		, m_pBufferManager(NULL)
		, m_pShaderManager(NULL)
		, m_pSamplerManager(NULL)
		, m_pTextureManager(NULL)
		, m_pPipelineManager(NULL)
		, m_pRenderPassManager(NULL)
		, m_pFrameBufferManager(NULL)
	{
		memset(&m_vkDeviceFeatures, 0, sizeof(m_vkDeviceFeatures));
		memset(&m_vkDeviceProperties, 0, sizeof(m_vkDeviceProperties));
		memset(&m_vkMemoryProperties, 0, sizeof(m_vkMemoryProperties));

		m_pQueue = SAFE_NEW CVulkanQueue(this);
		m_pFenceManager = SAFE_NEW CVulkanFenceManager(this);
		m_pSemaphoreManager = SAFE_NEW CVulkanSemaphoreManager(this);
		m_pMemoryManager = SAFE_NEW CVulkanMemoryManager(this);
		m_pStagingBufferManager = SAFE_NEW CVulkanStagingBufferManager(this);
		m_pCommandBufferManager = SAFE_NEW CVulkanCommandBufferManager(this);
		m_pDescriptorSetManager = SAFE_NEW CVulkanDescriptorSetManager(this);
		m_pDescriptorSetLayoutManager = SAFE_NEW CVulkanDescriptorSetLayoutManager(this);

		m_pBufferManager = SAFE_NEW CVulkanBufferManager(this);
		m_pShaderManager = SAFE_NEW CVulkanShaderManager(this);
		m_pSamplerManager = SAFE_NEW CVulkanSamplerManager(this);
		m_pTextureManager = SAFE_NEW CVulkanTextureManager(this);
		m_pPipelineManager = SAFE_NEW CVulkanPipelineManager(this);
		m_pRenderPassManager = SAFE_NEW CVulkanRenderPassManager(this);
		m_pFrameBufferManager = SAFE_NEW CVulkanFrameBufferManager(this);
	}

	CVulkanDevice::~CVulkanDevice(void)
	{
		ASSERT(m_vkDevice == VK_NULL_HANDLE);
		ASSERT(m_vkPhysicalDevice == VK_NULL_HANDLE);

		SAFE_DELETE(m_pQueue);
		SAFE_DELETE(m_pFenceManager);
		SAFE_DELETE(m_pSemaphoreManager);
		SAFE_DELETE(m_pMemoryManager);
		SAFE_DELETE(m_pStagingBufferManager);
		SAFE_DELETE(m_pCommandBufferManager);
		SAFE_DELETE(m_pDescriptorSetManager);
		SAFE_DELETE(m_pDescriptorSetLayoutManager);

		SAFE_DELETE(m_pBufferManager);
		SAFE_DELETE(m_pShaderManager);
		SAFE_DELETE(m_pSamplerManager);
		SAFE_DELETE(m_pTextureManager);
		SAFE_DELETE(m_pPipelineManager);
		SAFE_DELETE(m_pRenderPassManager);
		SAFE_DELETE(m_pFrameBufferManager);
	}

	BOOL CVulkanDevice::Create(void)
	{
		try {
			uint32_t queueFamilyIndex;
			std::vector<VkPhysicalDevice> devices;

			CALL_VK_FUNCTION_THROW(EnumeratePhysicalDevices(devices));
			CALL_VK_FUNCTION_THROW(CreateDevice(SelectPhysicalDevices(devices, queueFamilyIndex), queueFamilyIndex));
			CALL_VK_FUNCTION_THROW(CreateQueue(queueFamilyIndex));
			CALL_VK_FUNCTION_THROW(CreateFenceManager());
			CALL_VK_FUNCTION_THROW(CreateSemaphoreManager());
			CALL_VK_FUNCTION_THROW(CreateMemoryManager());
			CALL_VK_FUNCTION_THROW(CreateStagingBufferManager());
			CALL_VK_FUNCTION_THROW(CreateCommandBufferManager());
			CALL_VK_FUNCTION_THROW(CreateDescriptorSetManager());
			CALL_VK_FUNCTION_THROW(CreateDescriptorSetLayoutManager());

			CALL_VK_FUNCTION_THROW(CreateBufferManager());
			CALL_VK_FUNCTION_THROW(CreateShaderManager());
			CALL_VK_FUNCTION_THROW(CreateSamplerManager());
			CALL_VK_FUNCTION_THROW(CreateTextureManager());
			CALL_VK_FUNCTION_THROW(CreatePipelineManager());
			CALL_VK_FUNCTION_THROW(CreateRenderPassManager());
			CALL_VK_FUNCTION_THROW(CreateFrameBufferManager());

			CVulkanHelper::vkSetupFormat(m_vkPhysicalDevice);

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanDevice::Destroy(void)
	{
		if (m_vkDevice == VK_NULL_HANDLE || m_vkPhysicalDevice == VK_NULL_HANDLE) {
			return;
		}

		vkDeviceWaitIdle(m_vkDevice);
		{
			DestroyFrameBufferManager();
			DestroyRenderPassManager();
			DestroyPipelineManager();
			DestroyTextureManager();
			DestroySamplerManager();
			DestroyShaderManager();
			DestroyBufferManager();

			DestroyDescriptorSetLayoutManager();
			DestroyDescriptorSetManager();
			DestroyCommandBufferManager();
			DestroyStagingBufferManager();
			DestroyMemoryManager();
			DestroySemaphoreManager();
			DestroyFenceManager();
			DestroyQueue();
		}
		vkDestroyDevice(m_vkDevice, m_pVulkan->GetAllocator()->GetAllocationCallbacks());

		m_vkDevice = VK_NULL_HANDLE;
		m_vkPhysicalDevice = VK_NULL_HANDLE;
	}

	VkResult CVulkanDevice::EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const
	{
		devices.clear();

		uint32_t numDevices;
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pVulkan->GetInstance(), &numDevices, NULL));

		ASSERT(numDevices > 0);
		devices.resize(numDevices);
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pVulkan->GetInstance(), &numDevices, devices.data()));

		return VK_SUCCESS;
	}

	VkResult CVulkanDevice::CreateQueue(uint32_t queueFamilyIndex)
	{
		return m_pQueue->Create(queueFamilyIndex) ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateFenceManager(void)
	{
		return m_pFenceManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateSemaphoreManager(void)
	{
		return m_pSemaphoreManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateMemoryManager(void)
	{
		return m_pMemoryManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateStagingBufferManager(void)
	{
		return m_pStagingBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateCommandBufferManager(void)
	{
		return m_pCommandBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateDescriptorSetManager(void)
	{
		return m_pDescriptorSetManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateDescriptorSetLayoutManager(void)
	{
		return m_pDescriptorSetLayoutManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateBufferManager(void)
	{
		return m_pBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateShaderManager(void)
	{
		return m_pShaderManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateSamplerManager(void)
	{
		return m_pSamplerManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateTextureManager(void)
	{
		return m_pTextureManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreatePipelineManager(void)
	{
		return m_pPipelineManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateRenderPassManager(void)
	{
		return m_pRenderPassManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CVulkanDevice::CreateFrameBufferManager(void)
	{
		return m_pFrameBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	void CVulkanDevice::DestroyQueue(void)
	{
		m_pQueue->Destroy();
	}

	void CVulkanDevice::DestroyFenceManager(void)
	{
		m_pFenceManager->Destroy();
	}

	void CVulkanDevice::DestroySemaphoreManager(void)
	{
		m_pSemaphoreManager->Destroy();
	}

	void CVulkanDevice::DestroyMemoryManager(void)
	{
		m_pMemoryManager->Destroy();
	}

	void CVulkanDevice::DestroyStagingBufferManager(void)
	{
		m_pStagingBufferManager->Destroy();
	}

	void CVulkanDevice::DestroyCommandBufferManager(void)
	{
		m_pCommandBufferManager->Destroy();
	}

	void CVulkanDevice::DestroyDescriptorSetManager(void)
	{
		m_pDescriptorSetManager->Destroy();
	}

	void CVulkanDevice::DestroyDescriptorSetLayoutManager(void)
	{
		m_pDescriptorSetLayoutManager->Destroy();
	}

	void CVulkanDevice::DestroyBufferManager(void)
	{
		m_pBufferManager->Destroy();
	}

	void CVulkanDevice::DestroyShaderManager(void)
	{
		m_pShaderManager->Destroy();
	}

	void CVulkanDevice::DestroySamplerManager(void)
	{
		m_pSamplerManager->Destroy();
	}

	void CVulkanDevice::DestroyTextureManager(void)
	{
		m_pTextureManager->Destroy();
	}

	void CVulkanDevice::DestroyPipelineManager(void)
	{
		m_pPipelineManager->Destroy();
	}

	void CVulkanDevice::DestroyRenderPassManager(void)
	{
		m_pRenderPassManager->Destroy();
	}

	void CVulkanDevice::DestroyFrameBufferManager(void)
	{
		m_pFrameBufferManager->Destroy();
	}

	CVulkan* CVulkanDevice::GetVulkan(void) const
	{
		return m_pVulkan;
	}

	CVulkanQueue* CVulkanDevice::GetQueue(void) const
	{
		return m_pQueue;
	}

	VkDevice CVulkanDevice::GetDevice(void) const
	{
		return m_vkDevice;
	}

	VkPhysicalDevice CVulkanDevice::GetPhysicalDevice(void) const
	{
		return m_vkPhysicalDevice;
	}

	const VkPhysicalDeviceFeatures& CVulkanDevice::GetDeviceFeatures(void) const
	{
		return m_vkDeviceFeatures;
	}

	const VkPhysicalDeviceProperties& CVulkanDevice::GetDeviceProperties(void) const
	{
		return m_vkDeviceProperties;
	}

	const VkPhysicalDeviceMemoryProperties& CVulkanDevice::GetMemoryProperties(void) const
	{
		return m_vkMemoryProperties;
	}

	CVulkanFenceManager* CVulkanDevice::GetFenceManager(void) const
	{
		return m_pFenceManager;
	}

	CVulkanSemaphoreManager* CVulkanDevice::GetSemaphoreManager(void) const
	{
		return m_pSemaphoreManager;
	}

	CVulkanMemoryManager* CVulkanDevice::GetMemoryManager(void) const
	{
		return m_pMemoryManager;
	}

	CVulkanStagingBufferManager* CVulkanDevice::GetStagingBufferManager(void) const
	{
		return m_pStagingBufferManager;
	}

	CVulkanCommandBufferManager* CVulkanDevice::GetCommandBufferManager(void) const
	{
		return m_pCommandBufferManager;
	}

	CVulkanDescriptorSetManager* CVulkanDevice::GetDescriptorSetManager(void) const
	{
		return m_pDescriptorSetManager;
	}

	CVulkanDescriptorSetLayoutManager* CVulkanDevice::GetDescriptorSetLayoutManager(void) const
	{
		return m_pDescriptorSetLayoutManager;
	}

	CVulkanBufferManager* CVulkanDevice::GetBufferManager(void) const
	{
		return m_pBufferManager;
	}

	CVulkanShaderManager* CVulkanDevice::GetShaderManager(void) const
	{
		return m_pShaderManager;
	}

	CVulkanSamplerManager* CVulkanDevice::GetSamplerManager(void) const
	{
		return m_pSamplerManager;
	}

	CVulkanTextureManager* CVulkanDevice::GetTextureManager(void) const
	{
		return m_pTextureManager;
	}

	CVulkanPipelineManager* CVulkanDevice::GetPipelineManager(void) const
	{
		return m_pPipelineManager;
	}

	CVulkanRenderPassManager* CVulkanDevice::GetRenderPassManager(void) const
	{
		return m_pRenderPassManager;
	}

	CVulkanFrameBufferManager* CVulkanDevice::GetFrameBufferManager(void) const
	{
		return m_pFrameBufferManager;
	}

	void CVulkanDevice::DumpLog(void) const
	{
		m_pMemoryManager->DumpLog("Device Memory ...");
		m_pFrameBufferManager->DumpLog("FrameBuffer ...");
		m_pRenderPassManager->DumpLog("RenderPass ...");
		m_pDescriptorSetManager->DumpLog("DescriptorSet ...");
		m_pFenceManager->DumpLog("Fence ...");
		m_pBufferManager->DumpLog("Buffer ...");
		m_pShaderManager->DumpLog("Shader ...");
		m_pSamplerManager->DumpLog("Sampler ...");
		m_pTextureManager->DumpLog("Texture ...");
		m_pPipelineManager->DumpLog("Pipeline ...");
	}

}
