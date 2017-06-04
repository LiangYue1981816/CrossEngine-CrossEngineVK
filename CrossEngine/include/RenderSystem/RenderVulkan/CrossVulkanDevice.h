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
		friend class CVulkanStagingBuffer;


	protected:
		CVulkanDevice(CVulkanInstance *pVulkan);
		virtual ~CVulkanDevice(void);


	protected:
		BOOL Create(void);
		void Destroy(void);

	protected:
		VkResult EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const;

		VkResult SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &vkPhysicalDevice, uint32_t &queueFamilyIndex) const;
		VkResult CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const;
		VkResult CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const;
		VkResult CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const;

		VkResult CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex);
		VkResult CreateQueue(uint32_t queueFamilyIndex);
		VkResult CreateMemoryManager(void);
		VkResult CreateCommandPoolManager(void);
		VkResult CreateDescriptorSetManager(void);
		VkResult CreateStagingBufferManager(void);

		VkResult CreateBufferManager(void);
		VkResult CreateTextureManager(void);
		VkResult CreateShaderManager(void);
		VkResult CreatePipelineManager(void);
		VkResult CreateRenderPassManager(void);
		VkResult CreateFrameBufferManager(void);

	protected:
		void DestroyDevice(void);
		void DestroyQueue(void);
		void DestroyMemoryManager(void);
		void DestroyCommandPoolManager(void);
		void DestroyDescriptorSetManager(void);
		void DestroyStagingBufferManager(void);

		void DestroyBufferManager(void);
		void DestroyTextureManager(void);
		void DestroyShaderManager(void);
		void DestroyPipelineManager(void);
		void DestroyRenderPassManager(void);
		void DestroyFrameBufferManager(void);

	protected:
		CVulkanMemoryManager* GetMemoryManager(void) const;
		CVulkanStagingBufferManager* GetStagingBufferManager(void) const;

	public:
		CVulkanQueue* GetQueue(void) const;
		CVulkanInstance* GetVulkan(void) const;

	public:
		VkDevice GetDevice(void) const;
		VkPhysicalDevice GetPhysicalDevice(void) const;

	public:
		const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures(void) const;
		const VkPhysicalDeviceProperties& GetPhysicalDeviceProperties(void) const;
		const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties(void) const;

	public:
		CGfxCommandBufferPtr AllocCommandBuffer(uint32_t pool, CommandBufferLevel level);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineCompute *pPipeline);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphics *pPipeline);

		CGfxIndexBufferPtr NewIndexBuffer(void);
		CGfxVertexBufferPtr NewVertexBuffer(void);
		CGfxUniformBufferPtr NewUniformBuffer(void);

		CGfxTexturePtr NewTexture(void);
		CGfxRenderTexturePtr NewRenderTexture(void);

		CGfxShaderPtr NewShader(void);
		CGfxPipelineComputePtr NewPipelineCompute(void);
		CGfxPipelineGraphicsPtr NewPipelineGraphics(void);

		CGfxFrameBufferPtr NewFrameBuffer(void);

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
		CVulkanQueue *m_pQueue;
		CVulkanMemoryManager *m_pMemoryManager;
		CVulkanCommandPoolManager *m_pCommandPoolManager;
		CVulkanDescriptorSetManager *m_pDescriptorSetManager;
		CVulkanStagingBufferManager *m_pStagingBufferManager;

	protected:
		CVulkanBufferManager *m_pBufferManager;
		CVulkanTextureManager *m_pTextureManager;
		CVulkanShaderManager *m_pShaderManager;
		CVulkanPipelineManager *m_pPipelineManager;
		CVulkanRenderPassManager *m_pRenderPassManager;
		CVulkanFrameBufferManager *m_pFrameBufferManager;

	protected:
		CVulkanInstance *m_pVulkan;
	};

}
