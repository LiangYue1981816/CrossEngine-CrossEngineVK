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

	CVulkanDevice::CVulkanDevice(CVulkanInstance *pVulkan)
		: m_pVulkan(pVulkan)

		, m_vkDevice(VK_NULL_HANDLE)
		, m_vkPhysicalDevice(VK_NULL_HANDLE)

		, m_pQueue(NULL)
		, m_pMemoryManager(NULL)
		, m_pCommandPoolManager(NULL)
		, m_pDescriptorSetManager(NULL)
		, m_pStagingBufferManager(NULL)

		, m_pBufferManager(NULL)
		, m_pTextureManager(NULL)
		, m_pShaderManager(NULL)
		, m_pPipelineManager(NULL)
		, m_pRenderPassManager(NULL)
		, m_pFrameBufferManager(NULL)
	{
		m_vkPhysicalDeviceFeatures = {};
		m_vkPhysicalDeviceProperties = {};
		m_vkPhysicalDeviceMemoryProperties = {};

		m_pQueue = SAFE_NEW CVulkanQueue(this);
		m_pMemoryManager = SAFE_NEW CVulkanMemoryManager(this);
		m_pCommandPoolManager = SAFE_NEW CVulkanCommandPoolManager(this);
		m_pDescriptorSetManager = SAFE_NEW CVulkanDescriptorSetManager(this);
		m_pStagingBufferManager = SAFE_NEW CVulkanStagingBufferManager(this);

		m_pBufferManager = SAFE_NEW CVulkanBufferManager(this);
		m_pTextureManager = SAFE_NEW CVulkanTextureManager(this);
		m_pShaderManager = SAFE_NEW CVulkanShaderManager(this);
		m_pPipelineManager = SAFE_NEW CVulkanPipelineManager(this);
		m_pRenderPassManager = SAFE_NEW CVulkanRenderPassManager(this);
		m_pFrameBufferManager = SAFE_NEW CVulkanFrameBufferManager(this);
	}

	CVulkanDevice::~CVulkanDevice(void)
	{
		SAFE_DELETE(m_pQueue);
		SAFE_DELETE(m_pMemoryManager);
		SAFE_DELETE(m_pCommandPoolManager);
		SAFE_DELETE(m_pDescriptorSetManager);
		SAFE_DELETE(m_pStagingBufferManager);

		SAFE_DELETE(m_pBufferManager);
		SAFE_DELETE(m_pTextureManager);
		SAFE_DELETE(m_pShaderManager);
		SAFE_DELETE(m_pPipelineManager);
		SAFE_DELETE(m_pRenderPassManager);
		SAFE_DELETE(m_pFrameBufferManager);
	}

	int CVulkanDevice::Create(void)
	{
		uint32_t queueFamilyIndex;
		std::vector<VkPhysicalDevice> devices;

		CALL_VK_FUNCTION_RETURN(EnumeratePhysicalDevices(devices));
		CALL_VK_FUNCTION_RETURN(SelectPhysicalDevices(devices, m_vkPhysicalDevice, queueFamilyIndex));
		CALL_VK_FUNCTION_RETURN(CreateDevice(m_vkPhysicalDevice, queueFamilyIndex));

		CALL_VK_FUNCTION_RETURN(CreateQueue(queueFamilyIndex));
		CALL_VK_FUNCTION_RETURN(CreateMemoryManager());
		CALL_VK_FUNCTION_RETURN(CreateCommandPoolManager());
		CALL_VK_FUNCTION_RETURN(CreateDescriptorSetManager());
		CALL_VK_FUNCTION_RETURN(CreateStagingBufferManager());

		CALL_VK_FUNCTION_RETURN(CreateBufferManager());
		CALL_VK_FUNCTION_RETURN(CreateTextureManager());
		CALL_VK_FUNCTION_RETURN(CreateShaderManager());
		CALL_VK_FUNCTION_RETURN(CreatePipelineManager());
		CALL_VK_FUNCTION_RETURN(CreateRenderPassManager());
		CALL_VK_FUNCTION_RETURN(CreateFrameBufferManager());

		CVulkanHelper::vkSetupFormat(m_vkPhysicalDevice);

		return VK_SUCCESS;
	}

	void CVulkanDevice::Destroy(void)
	{
		if (m_vkDevice == VK_NULL_HANDLE || m_vkPhysicalDevice == VK_NULL_HANDLE) {
			return;
		}

		vkDeviceWaitIdle(m_vkDevice);

		DestroyFrameBufferManager();
		DestroyRenderPassManager();
		DestroyPipelineManager();
		DestroyShaderManager();
		DestroyTextureManager();
		DestroyBufferManager();

		DestroyStagingBufferManager();
		DestroyDescriptorSetManager();
		DestroyCommandPoolManager();
		DestroyMemoryManager();
		DestroyQueue();
		DestroyDevice();
	}

	int CVulkanDevice::EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const
	{
		devices.clear();

		uint32_t numDevices;
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pVulkan->GetInstance(), &numDevices, NULL));

		ASSERT(numDevices > 0);
		devices.resize(numDevices);
		CALL_VK_FUNCTION_RETURN(vkEnumeratePhysicalDevices(m_pVulkan->GetInstance(), &numDevices, devices.data()));

		return VK_SUCCESS;
	}

	int CVulkanDevice::SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &vkPhysicalDevice, uint32_t &queueFamilyIndex) const
	{
		return VK_SUCCESS;
	}

	int CVulkanDevice::CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const
	{
		return VK_SUCCESS;
	}

	int CVulkanDevice::CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const
	{
		return VK_SUCCESS;
	}

	int CVulkanDevice::CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const
	{
		return VK_SUCCESS;
	}

	int CVulkanDevice::CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex)
	{
		return VK_SUCCESS;
	}

	int CVulkanDevice::CreateQueue(uint32_t queueFamilyIndex)
	{
		return m_pQueue->Create(queueFamilyIndex);
	}

	int CVulkanDevice::CreateMemoryManager(void)
	{
		return m_pMemoryManager->Create();
	}

	int CVulkanDevice::CreateCommandPoolManager(void)
	{
		return m_pCommandPoolManager->Create();
	}

	int CVulkanDevice::CreateDescriptorSetManager(void)
	{
		return m_pDescriptorSetManager->Create();
	}

	int CVulkanDevice::CreateStagingBufferManager(void)
	{
		return m_pStagingBufferManager->Create();
	}

	int CVulkanDevice::CreateBufferManager(void)
	{
		return m_pBufferManager->Create();
	}

	int CVulkanDevice::CreateTextureManager(void)
	{
		return m_pTextureManager->Create();
	}

	int CVulkanDevice::CreateShaderManager(void)
	{
		return m_pShaderManager->Create();
	}

	int CVulkanDevice::CreatePipelineManager(void)
	{
		return m_pPipelineManager->Create();
	}

	int CVulkanDevice::CreateRenderPassManager(void)
	{
		return m_pRenderPassManager->Create();
	}

	int CVulkanDevice::CreateFrameBufferManager(void)
	{
		return m_pFrameBufferManager->Create();
	}

	void CVulkanDevice::DestroyDevice(void)
	{
		if (m_vkDevice && m_vkPhysicalDevice) {
			vkDestroyDevice(m_vkDevice, m_pVulkan->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkDevice = VK_NULL_HANDLE;
		m_vkPhysicalDevice = VK_NULL_HANDLE;
	}

	void CVulkanDevice::DestroyQueue(void)
	{
		m_pQueue->Destroy();
	}

	void CVulkanDevice::DestroyMemoryManager(void)
	{
		m_pMemoryManager->Destroy();
	}

	void CVulkanDevice::DestroyCommandPoolManager(void)
	{
		m_pCommandPoolManager->Destroy();
	}

	void CVulkanDevice::DestroyDescriptorSetManager(void)
	{
		m_pDescriptorSetManager->Destroy();
	}

	void CVulkanDevice::DestroyStagingBufferManager(void)
	{
		m_pStagingBufferManager->Destroy();
	}

	void CVulkanDevice::DestroyBufferManager(void)
	{
		m_pBufferManager->Destroy();
	}

	void CVulkanDevice::DestroyTextureManager(void)
	{
		m_pTextureManager->Destroy();
	}

	void CVulkanDevice::DestroyShaderManager(void)
	{
		m_pShaderManager->Destroy();
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

	CVulkanMemoryManager* CVulkanDevice::GetMemoryManager(void) const
	{
		return m_pMemoryManager;
	}

	CVulkanStagingBufferManager* CVulkanDevice::GetStagingBufferManager(void) const
	{
		return m_pStagingBufferManager;
	}

	CVulkanQueue* CVulkanDevice::GetQueue(void) const
	{
		return m_pQueue;
	}

	CVulkanInstance* CVulkanDevice::GetVulkan(void) const
	{
		return m_pVulkan;
	}

	VkDevice CVulkanDevice::GetDevice(void) const
	{
		return m_vkDevice;
	}

	VkPhysicalDevice CVulkanDevice::GetPhysicalDevice(void) const
	{
		return m_vkPhysicalDevice;
	}

	const VkPhysicalDeviceFeatures& CVulkanDevice::GetPhysicalDeviceFeatures(void) const
	{
		return m_vkPhysicalDeviceFeatures;
	}

	const VkPhysicalDeviceProperties& CVulkanDevice::GetPhysicalDeviceProperties(void) const
	{
		return m_vkPhysicalDeviceProperties;
	}

	const VkPhysicalDeviceMemoryProperties& CVulkanDevice::GetPhysicalDeviceMemoryProperties(void) const
	{
		return m_vkPhysicalDeviceMemoryProperties;
	}

	CGfxCommandBufferPtr CVulkanDevice::AllocCommandBuffer(uint32_t pool, CommandBufferLevel level)
	{
		return CGfxCommandBufferPtr(NULL);
	}

	CGfxDescriptorSetPtr CVulkanDevice::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxDescriptorSetPtr CVulkanDevice::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxIndexBufferPtr CVulkanDevice::NewIndexBuffer(void)
	{
		return CGfxIndexBufferPtr(NULL);
	}

	CGfxVertexBufferPtr CVulkanDevice::NewVertexBuffer(void)
	{
		return CGfxVertexBufferPtr(NULL);
	}

	CGfxUniformBufferPtr CVulkanDevice::NewUniformBuffer(void)
	{
		return CGfxUniformBufferPtr(NULL);
	}

	CGfxTexturePtr CVulkanDevice::NewTexture(void)
	{
		return CGfxTexturePtr(NULL);
	}

	CGfxRenderTexturePtr CVulkanDevice::NewRenderTexture(void)
	{
		return CGfxRenderTexturePtr(NULL);
	}

	CGfxShaderPtr CVulkanDevice::NewShader(void)
	{
		return CGfxShaderPtr(NULL);
	}

	CGfxPipelineComputePtr CVulkanDevice::NewPipelineCompute(void)
	{
		return CGfxPipelineComputePtr(NULL);
	}

	CGfxPipelineGraphicsPtr CVulkanDevice::NewPipelineGraphics(void)
	{
		return CGfxPipelineGraphicsPtr(NULL);
	}

	CGfxRenderPassPtr CVulkanDevice::NewRenderPass(void)
	{
		return CGfxRenderPassPtr(NULL);
	}

	CGfxFrameBufferPtr CVulkanDevice::NewFrameBuffer(void)
	{
		return CGfxFrameBufferPtr(NULL);
	}

	void CVulkanDevice::DumpLog(void) const
	{
		m_pMemoryManager->DumpLog("Device Memory ...");
		m_pDescriptorSetManager->DumpLog("DescriptorSet ...");
		m_pBufferManager->DumpLog("Buffer ...");
		m_pTextureManager->DumpLog("Texture ...");
		m_pShaderManager->DumpLog("Shader ...");
		m_pPipelineManager->DumpLog("Pipeline ...");
		m_pRenderPassManager->DumpLog("RenderPass ...");
		m_pFrameBufferManager->DumpLog("FrameBuffer ...");
	}

}
