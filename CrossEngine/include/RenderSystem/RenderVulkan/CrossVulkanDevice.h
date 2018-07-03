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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CVulkanDevice : public CGfxDevice
	{
		friend class CVulkanInstance;
		friend class CVulkanImage;
		friend class CVulkanBuffer;
		friend class CVulkanStagingBuffer;


	protected:
		CVulkanDevice(CVulkanInstance *pInstance);
		virtual ~CVulkanDevice(void);


	protected:
		int Create(void);
		void Destroy(void);

	protected:
		int EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const;

		int SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &vkPhysicalDevice, uint32_t &queueFamilyIndex) const;
		int CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const;
		int CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const;
		int CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const;

		int CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex);
		int CreateQueue(void);
		int CreateMemoryManager(void);
		int CreateCommandPoolManager(void);
		int CreateDescriptorPoolManager(void);
		int CreateStagingBufferManager(void);

		int CreateBufferManager(void);
		int CreateTextureManager(void);
		int CreateShaderManager(void);
		int CreatePipelineManager(void);
		int CreateMaterialManager(void);
		int CreateRenderPassManager(void);
		int CreateFrameBufferManager(void);

	protected:
		void DestroyDevice(void);
		void DestroyQueue(void);
		void DestroyMemoryManager(void);
		void DestroyCommandPoolManager(void);
		void DestroyDescriptorPoolManager(void);
		void DestroyStagingBufferManager(void);

		void DestroyBufferManager(void);
		void DestroyTextureManager(void);
		void DestroyShaderManager(void);
		void DestroyPipelineManager(void);
		void DestroyMaterialManager(void);
		void DestroyRenderPassManager(void);
		void DestroyFrameBufferManager(void);

	protected:
		CVulkanMemoryManager* GetMemoryManager(void) const;
		CVulkanStagingBufferManager* GetStagingBufferManager(void) const;

	public:
		CGfxInstance* GetInstance(void) const;
		CGfxQueue* GetComputeQueue(void) const;
		CGfxQueue* GetGraphicsQueue(void) const;
		CGfxQueue* GetTransferQueue(void) const;
		uint32_t GetQueueFamilyIndex(void) const;

	public:
		VkDevice GetDevice(void) const;
		VkPhysicalDevice GetPhysicalDevice(void) const;

	public:
		const VkPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;
		const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures(void) const;
		const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties(void) const;

	public:
		void ResetCommandBufferPool(uint32_t pool);
		void AllocCommandBufferPool(uint32_t pool);
		CGfxCommandBufferPtr AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level);

		void ResetDescriptorSetPool(uint32_t pool);
		void AllocDescriptorSetPool(uint32_t pool);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipelineCompute);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipelineGraphics);
		CGfxDescriptorSetLayoutPtr AllocDescriptorSetLayout(uint32_t set);

		CGfxIndexBufferPtr NewIndexBuffer(void);
		CGfxVertexBufferPtr NewVertexBuffer(void);
		CGfxUniformBufferPtr NewUniformBuffer(void);
		CGfxTexturePtr NewTexture(void);
		CGfxRenderTexturePtr NewRenderTexture(void);
		CGfxShaderPtr NewShader(void);
		CGfxMaterialPtr NewMaterial(void);
		CGfxPipelineComputePtr NewPipelineCompute(void);
		CGfxPipelineGraphicsPtr NewPipelineGraphics(uint32_t numAttachments);
		CGfxRenderPassPtr NewRenderPass(uint32_t numAttachments, uint32_t numSubpasses);
		CGfxFrameBufferPtr NewFrameBuffer(uint32_t numAttachments);

	public:
		CVulkanStagingBufferPtr NewStagingBuffer(VkDeviceSize size);

	public:
		void SetShaderCachePath(const char *szPath);
		void AddShaderIncludePath(const char *szPath);
		void AddShaderMacroDefinition(const char *szName);
		void AddShaderMacroDefinition(const char *szName, const char *szValue);
		BOOL PrecompileShader(const char *szSource, size_t length, VkShaderStageFlagBits flags, std::vector<uint32_t> &words);

	public:
		int WaitIdle(void) const;

	public:
		void DumpLog(void) const;


	protected:
		VkDevice m_vkDevice;
		VkPhysicalDevice m_vkPhysicalDevice;

	protected:
		VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
		VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
		VkPhysicalDeviceMemoryProperties m_vkPhysicalDeviceMemoryProperties;

	protected:
		uint32_t m_queueFamilyIndex;
		CVulkanQueue *m_pComputeQueue;
		CVulkanQueue *m_pGraphicsQueue;
		CVulkanQueue *m_pTransferQueue;
		CVulkanMemoryManager *m_pMemoryManager;
		CVulkanCommandPoolManager *m_pCommandPoolManager;
		CVulkanDescriptorPoolManager *m_pDescriptorPoolManager;
		CVulkanStagingBufferManager *m_pStagingBufferManager;

	protected:
		CVulkanBufferManager *m_pBufferManager;
		CVulkanTextureManager *m_pTextureManager;
		CVulkanShaderManager *m_pShaderManager;
		CVulkanPipelineManager *m_pPipelineManager;
		CVulkanMaterialManager *m_pMaterialManager;
		CVulkanRenderPassManager *m_pRenderPassManager;
		CVulkanFrameBufferManager *m_pFrameBufferManager;

	protected:
		CVulkanInstance *m_pInstance;
	};

}
