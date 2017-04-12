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

	CRendererDevice::CRendererDevice(CRenderer *pRenderer)
		: m_pRenderer(pRenderer)
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

		m_pQueue = SAFE_NEW CRendererQueue(this);
		m_pFenceManager = SAFE_NEW CRendererFenceManager(this);
		m_pSemaphoreManager = SAFE_NEW CRendererSemaphoreManager(this);
		m_pMemoryManager = SAFE_NEW CRendererMemoryManager(this);
		m_pStagingBufferManager = SAFE_NEW CRendererStagingBufferManager(this);
		m_pCommandBufferManager = SAFE_NEW CRendererCommandBufferManager(this);
		m_pDescriptorSetManager = SAFE_NEW CRendererDescriptorSetManager(this);
		m_pDescriptorSetLayoutManager = SAFE_NEW CRendererDescriptorSetLayoutManager(this);

		m_pBufferManager = SAFE_NEW CRendererBufferManager(this);
		m_pShaderManager = SAFE_NEW CRendererShaderManager(this);
		m_pSamplerManager = SAFE_NEW CRendererSamplerManager(this);
		m_pTextureManager = SAFE_NEW CRendererTextureManager(this);
		m_pPipelineManager = SAFE_NEW CRendererPipelineManager(this);
		m_pRenderPassManager = SAFE_NEW CRendererRenderPassManager(this);
		m_pFrameBufferManager = SAFE_NEW CRendererFrameBufferManager(this);
	}

	CRendererDevice::~CRendererDevice(void)
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

	CRenderer* CRendererDevice::GetRenderer(void) const
	{
		return m_pRenderer;
	}

	CRendererQueue* CRendererDevice::GetQueue(void) const
	{
		return m_pQueue;
	}

	VkDevice CRendererDevice::GetDevice(void) const
	{
		return m_vkDevice;
	}

	VkPhysicalDevice CRendererDevice::GetPhysicalDevice(void) const
	{
		return m_vkPhysicalDevice;
	}

	const VkPhysicalDeviceFeatures& CRendererDevice::GetDeviceFeatures(void) const
	{
		return m_vkDeviceFeatures;
	}

	const VkPhysicalDeviceProperties& CRendererDevice::GetDeviceProperties(void) const
	{
		return m_vkDeviceProperties;
	}

	const VkPhysicalDeviceMemoryProperties& CRendererDevice::GetMemoryProperties(void) const
	{
		return m_vkMemoryProperties;
	}
	
	BOOL CRendererDevice::Create(void)
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

			CRendererHelper::vkSetupFormat(m_vkPhysicalDevice);

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererDevice::Destroy(void)
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
		vkDestroyDevice(m_vkDevice, m_pRenderer->GetAllocator()->GetAllocationCallbacks());

		m_vkDevice = VK_NULL_HANDLE;
		m_vkPhysicalDevice = VK_NULL_HANDLE;
	}

	VkResult CRendererDevice::EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const
	{
		devices.clear();

		uint32_t numDevices;
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pRenderer->GetInstance(), &numDevices, NULL));

		ASSERT(numDevices > 0);
		devices.resize(numDevices);
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pRenderer->GetInstance(), &numDevices, devices.data()));

		return VK_SUCCESS;
	}

	VkResult CRendererDevice::CreateQueue(uint32_t queueFamilyIndex)
	{
		return m_pQueue->Create(queueFamilyIndex) ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateFenceManager(void)
	{
		return m_pFenceManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateSemaphoreManager(void)
	{
		return m_pSemaphoreManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateMemoryManager(void)
	{
		return m_pMemoryManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateStagingBufferManager(void)
	{
		return m_pStagingBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateCommandBufferManager(void)
	{
		return m_pCommandBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateDescriptorSetManager(void)
	{
		return m_pDescriptorSetManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateDescriptorSetLayoutManager(void)
	{
		return m_pDescriptorSetLayoutManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateBufferManager(void)
	{
		return m_pBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateShaderManager(void)
	{
		return m_pShaderManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateSamplerManager(void)
	{
		return m_pSamplerManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateTextureManager(void)
	{
		return m_pTextureManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreatePipelineManager(void)
	{
		return m_pPipelineManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateRenderPassManager(void)
	{
		return m_pRenderPassManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	VkResult CRendererDevice::CreateFrameBufferManager(void)
	{
		return m_pFrameBufferManager->Create() ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED;
	}

	void CRendererDevice::DestroyQueue(void)
	{
		m_pQueue->Destroy();
	}

	void CRendererDevice::DestroyFenceManager(void)
	{
		m_pFenceManager->Destroy();
	}

	void CRendererDevice::DestroySemaphoreManager(void)
	{
		m_pSemaphoreManager->Destroy();
	}

	void CRendererDevice::DestroyMemoryManager(void)
	{
		m_pMemoryManager->Destroy();
	}

	void CRendererDevice::DestroyStagingBufferManager(void)
	{
		m_pStagingBufferManager->Destroy();
	}

	void CRendererDevice::DestroyCommandBufferManager(void)
	{
		m_pCommandBufferManager->Destroy();
	}

	void CRendererDevice::DestroyDescriptorSetManager(void)
	{
		m_pDescriptorSetManager->Destroy();
	}

	void CRendererDevice::DestroyDescriptorSetLayoutManager(void)
	{
		m_pDescriptorSetLayoutManager->Destroy();
	}

	void CRendererDevice::DestroyBufferManager(void)
	{
		m_pBufferManager->Destroy();
	}

	void CRendererDevice::DestroyShaderManager(void)
	{
		m_pShaderManager->Destroy();
	}

	void CRendererDevice::DestroySamplerManager(void)
	{
		m_pSamplerManager->Destroy();
	}

	void CRendererDevice::DestroyTextureManager(void)
	{
		m_pTextureManager->Destroy();
	}

	void CRendererDevice::DestroyPipelineManager(void)
	{
		m_pPipelineManager->Destroy();
	}

	void CRendererDevice::DestroyRenderPassManager(void)
	{
		m_pRenderPassManager->Destroy();
	}

	void CRendererDevice::DestroyFrameBufferManager(void)
	{
		m_pFrameBufferManager->Destroy();
	}

	CRendererFenceManager* CRendererDevice::GetFenceManager(void) const
	{
		return m_pFenceManager;
	}

	CRendererSemaphoreManager* CRendererDevice::GetSemaphoreManager(void) const
	{
		return m_pSemaphoreManager;
	}

	CRendererMemoryManager* CRendererDevice::GetMemoryManager(void) const
	{
		return m_pMemoryManager;
	}

	CRendererStagingBufferManager* CRendererDevice::GetStagingBufferManager(void) const
	{
		return m_pStagingBufferManager;
	}

	CRendererCommandBufferManager* CRendererDevice::GetCommandBufferManager(void) const
	{
		return m_pCommandBufferManager;
	}

	CRendererDescriptorSetManager* CRendererDevice::GetDescriptorSetManager(void) const
	{
		return m_pDescriptorSetManager;
	}

	CRendererDescriptorSetLayoutManager* CRendererDevice::GetDescriptorSetLayoutManager(void) const
	{
		return m_pDescriptorSetLayoutManager;
	}

	CRendererBufferManager* CRendererDevice::GetBufferManager(void) const
	{
		return m_pBufferManager;
	}

	CRendererShaderManager* CRendererDevice::GetShaderManager(void) const
	{
		return m_pShaderManager;
	}

	CRendererSamplerManager* CRendererDevice::GetSamplerManager(void) const
	{
		return m_pSamplerManager;
	}

	CRendererTextureManager* CRendererDevice::GetTextureManager(void) const
	{
		return m_pTextureManager;
	}

	CRendererPipelineManager* CRendererDevice::GetPipelineManager(void) const
	{
		return m_pPipelineManager;
	}

	CRendererRenderPassManager* CRendererDevice::GetRenderPassManager(void) const
	{
		return m_pRenderPassManager;
	}

	CRendererFrameBufferManager* CRendererDevice::GetFrameBufferManager(void) const
	{
		return m_pFrameBufferManager;
	}

	void CRendererDevice::DumpLog(void) const
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
