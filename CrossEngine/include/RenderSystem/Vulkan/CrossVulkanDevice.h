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

	class CROSS_EXPORT CVulkanDevice
	{
		friend class CVulkanImage;
		friend class CVulkanBuffer;
		friend class CVulkanStagingBuffer;
		friend class CVulkanStagingBufferAutoRelease;


	public:
		enum {
			ATTRIBUTE_POSITION = 0,
			ATTRIBUTE_NORMAL,
			ATTRIBUTE_BINORMAL,
			ATTRIBUTE_COLOR,
			ATTRIBUTE_SKIN_INDEX,
			ATTRIBUTE_SKIN_WEIGHT,
			ATTRIBUTE_TEXCOORD0,
			ATTRIBUTE_TEXCOORD1,
			ATTRIBUTE_TEXCOORD2,
			ATTRIBUTE_TEXCOORD3,
			ATTRIBUTE_TEXCOORD4,
			ATTRIBUTE_TEXCOORD5,
			ATTRIBUTE_TEXCOORD6,
			ATTRIBUTE_TEXCOORD7,
			ATTRIBUTE_COUNT
		};

		static const uint32_t VERTEX_ATTRIBUTE_FLAG_POSITION    = 0x00000001;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_NORMAL      = 0x00000002;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_BINORMAL    = 0x00000004;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_COLOR       = 0x00000008;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX  = 0x00000010;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT = 0x00000020;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD0   = 0x00000040;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD1   = 0x00000080;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD2   = 0x00000100;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD3   = 0x00000200;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD4   = 0x00000400;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD5   = 0x00000800;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD6   = 0x00001000;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD7   = 0x00002000;


	protected:
		CVulkanDevice(CVulkan *pVulkan);
		virtual ~CVulkanDevice(void);


	protected:
		virtual BOOL Create(void);
		virtual void Destroy(void);

	protected:
		virtual VkResult EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const;
		virtual VkPhysicalDevice SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, uint32_t &queueFamilyIndex) = 0;
		virtual VkResult CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex) = 0;

		virtual VkResult CreateQueue(uint32_t queueFamilyIndex);
		virtual VkResult CreateMemoryManager(void);
		virtual VkResult CreateStagingBufferManager(void);

		virtual VkResult CreateFenceManager(void);
		virtual VkResult CreateSemaphoreManager(void);
		virtual VkResult CreateCommandBufferManager(void);
		virtual VkResult CreateDescriptorSetManager(void);

		virtual VkResult CreateBufferManager(void);
		virtual VkResult CreateShaderManager(void);
		virtual VkResult CreateSamplerManager(void);
		virtual VkResult CreateTextureManager(void);
		virtual VkResult CreatePipelineManager(void);
		virtual VkResult CreateRenderPassManager(void);
		virtual VkResult CreateFrameBufferManager(void);

	protected:
		virtual void DestroyQueue(void);
		virtual void DestroyMemoryManager(void);
		virtual void DestroyStagingBufferManager(void);

		virtual void DestroyFenceManager(void);
		virtual void DestroySemaphoreManager(void);
		virtual void DestroyCommandBufferManager(void);
		virtual void DestroyDescriptorSetManager(void);

		virtual void DestroyBufferManager(void);
		virtual void DestroyShaderManager(void);
		virtual void DestroySamplerManager(void);
		virtual void DestroyTextureManager(void);
		virtual void DestroyPipelineManager(void);
		virtual void DestroyRenderPassManager(void);
		virtual void DestroyFrameBufferManager(void);

	public:
		CVulkan* GetVulkan(void) const;
		CVulkanQueue* GetQueue(void) const;

	public:
		VkDevice GetDevice(void) const;
		VkPhysicalDevice GetPhysicalDevice(void) const;

	public:
		const VkPhysicalDeviceFeatures& GetDeviceFeatures(void) const;
		const VkPhysicalDeviceProperties& GetDeviceProperties(void) const;
		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties(void) const;

	protected:
		CVulkanMemoryManager* GetMemoryManager(void) const;
		CVulkanStagingBufferManager* GetStagingBufferManager(void) const;

	public:
		CVulkanCommandBufferPtr AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level);
		CVulkanDescriptorSetPtr AllocDescriptorSet(uint32_t pool, const CVulkanDescriptorSetLayout *pSetLayout);

		CVulkanFencePtr CreateFence(void);
		CVulkanSemaphorePtr CreateSemaphore(void);
		CVulkanBufferPtr CreateBuffer(void);
		CVulkanIndexBufferPtr CreateIndexBuffer(void);
		CVulkanVertexBufferPtr CreateVertexBuffer(void);
		CVulkanUniformBufferPtr CreateUniformBuffer(void);
		CVulkanSamplerPtr CreateSampler(void);
		CVulkanTexturePtr CreateTexture(void);
		CVulkanRenderTexturePtr CreateRenderTexture(void);
		CVulkanShaderPtr CreateShader(void);
		CVulkanPipelineComputePtr CreatePipelineCompute(void);
		CVulkanPipelineGraphicsPtr CreatePipelineGraphics(void);
		CVulkanRenderPassPtr CreateRenderPass(void);
		CVulkanFrameBufferPtr CreateFrameBuffer(void);

	public:
		void DumpLog(void) const;

	public:
		uint32_t GetVertexSize(uint32_t format) const;
		uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const;
		uint32_t GetVertexAttributeFlag(const char *szName) const;
		VkFormat GetVertexAttributeFormat(uint32_t attribute) const;


	protected:
		VkDevice m_vkDevice;
		VkPhysicalDevice m_vkPhysicalDevice;

	protected:
		VkPhysicalDeviceFeatures m_vkDeviceFeatures;
		VkPhysicalDeviceProperties m_vkDeviceProperties;
		VkPhysicalDeviceMemoryProperties m_vkMemoryProperties;

	protected:
		CVulkanQueue *m_pQueue;
		CVulkanMemoryManager *m_pMemoryManager;
		CVulkanStagingBufferManager *m_pStagingBufferManager;

	protected:
		CVulkanFenceManager *m_pFenceManager;
		CVulkanSemaphoreManager *m_pSemaphoreManager;
		CVulkanCommandBufferManager *m_pCommandBufferManager;
		CVulkanDescriptorSetManager *m_pDescriptorSetManager;

	protected:
		CVulkanBufferManager *m_pBufferManager;
		CVulkanShaderManager *m_pShaderManager;
		CVulkanSamplerManager *m_pSamplerManager;
		CVulkanTextureManager *m_pTextureManager;
		CVulkanPipelineManager *m_pPipelineManager;
		CVulkanRenderPassManager *m_pRenderPassManager;
		CVulkanFrameBufferManager *m_pFrameBufferManager;

	protected:
		CVulkan *m_pVulkan;
	};

}
