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

	class CROSS_EXPORT CRendererFence;
	class CROSS_EXPORT CRendererFrameBuffer;
	class CROSS_EXPORT CRendererCommandBuffer;
	class CROSS_EXPORT CRendererDescriptorSet;

	class CROSS_EXPORT CRendererQueue;
	class CROSS_EXPORT CRendererFenceManager;
	class CROSS_EXPORT CRendererMemoryManager;
	class CROSS_EXPORT CRendererStagingBufferManager;
	class CROSS_EXPORT CRendererCommandBufferManager;
	class CROSS_EXPORT CRendererDescriptorSetManager;
	class CROSS_EXPORT CRendererDescriptorSetLayoutManager;

	class CROSS_EXPORT CRendererBufferManager;
	class CROSS_EXPORT CRendererShaderManager;
	class CROSS_EXPORT CRendererSamplerManager;
	class CROSS_EXPORT CRendererTextureManager;
	class CROSS_EXPORT CRendererPipelineManager;
	class CROSS_EXPORT CRendererRenderPassManager;
	class CROSS_EXPORT CRendererFrameBufferManager;

	class CROSS_EXPORT CRendererDevice
	{
	protected:
		CRendererDevice(CRenderer *pRenderer);
		virtual ~CRendererDevice(void);


	public:
		CRenderer* GetRenderer(void) const;
		CRendererQueue* GetQueue(void) const;

	public:
		VkDevice GetDevice(void) const;
		VkPhysicalDevice GetPhysicalDevice(void) const;

	public:
		const VkPhysicalDeviceFeatures& GetDeviceFeatures(void) const;
		const VkPhysicalDeviceProperties& GetDeviceProperties(void) const;
		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties(void) const;


	protected:
		virtual BOOL Create(void);
		virtual void Destroy(void);

	protected:
		virtual VkResult EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &devices) const;
		virtual VkPhysicalDevice SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, uint32_t &queueFamilyIndex) = 0;
		virtual VkResult CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex) = 0;
		virtual VkResult CreateQueue(uint32_t queueFamilyIndex);
		virtual VkResult CreateFenceManager(void);
		virtual VkResult CreateMemoryManager(void);
		virtual VkResult CreateStagingBufferManager(void);
		virtual VkResult CreateCommandBufferManager(void);
		virtual VkResult CreateDescriptorSetManager(void);
		virtual VkResult CreateDescriptorSetLayoutManager(void);

		virtual VkResult CreateBufferManager(void);
		virtual VkResult CreateShaderManager(void);
		virtual VkResult CreateSamplerManager(void);
		virtual VkResult CreateTextureManager(void);
		virtual VkResult CreatePipelineManager(void);
		virtual VkResult CreateRenderPassManager(void);
		virtual VkResult CreateFrameBufferManager(void);

	protected:
		virtual void DestroyQueue(void);
		virtual void DestroyFenceManager(void);
		virtual void DestroyMemoryManager(void);
		virtual void DestroyStagingBufferManager(void);
		virtual void DestroyCommandBufferManager(void);
		virtual void DestroyDescriptorSetManager(void);
		virtual void DestroyDescriptorSetLayoutManager(void);

		virtual void DestroyBufferManager(void);
		virtual void DestroyShaderManager(void);
		virtual void DestroySamplerManager(void);
		virtual void DestroyTextureManager(void);
		virtual void DestroyPipelineManager(void);
		virtual void DestroyRenderPassManager(void);
		virtual void DestroyFrameBufferManager(void);


	public:
		CRendererFenceManager* GetFenceManager(void) const;
		CRendererMemoryManager* GetMemoryManager(void) const;
		CRendererStagingBufferManager* GetStagingBufferManager(void) const;
		CRendererCommandBufferManager* GetCommandBufferManager(void) const;
		CRendererDescriptorSetManager* GetDescriptorSetManager(void) const;
		CRendererDescriptorSetLayoutManager* GetDescriptorSetLayoutManager(void) const;

	public:
		CRendererBufferManager* GetBufferManager(void) const;
		CRendererShaderManager* GetShaderManager(void) const;
		CRendererSamplerManager* GetSamplerManager(void) const;
		CRendererTextureManager* GetTextureManager(void) const;
		CRendererPipelineManager* GetPipelineManager(void) const;
		CRendererRenderPassManager* GetRenderPassManager(void) const;
		CRendererFrameBufferManager* GetFrameBufferManager(void) const;

	public:
		void DumpLog(void) const;


	protected:
		VkDevice m_vkDevice;
		VkPhysicalDevice m_vkPhysicalDevice;

	protected:
		VkPhysicalDeviceFeatures m_vkDeviceFeatures;
		VkPhysicalDeviceProperties m_vkDeviceProperties;
		VkPhysicalDeviceMemoryProperties m_vkMemoryProperties;

	protected:
		CRendererQueue *m_pQueue;
		CRendererFenceManager *m_pFenceManager;
		CRendererMemoryManager *m_pMemoryManager;
		CRendererStagingBufferManager *m_pStagingBufferManager;
		CRendererCommandBufferManager *m_pCommandBufferManager;
		CRendererDescriptorSetManager *m_pDescriptorSetManager;
		CRendererDescriptorSetLayoutManager *m_pDescriptorSetLayoutManager;

	protected:
		CRendererBufferManager *m_pBufferManager;
		CRendererShaderManager *m_pShaderManager;
		CRendererSamplerManager *m_pSamplerManager;
		CRendererTextureManager *m_pTextureManager;
		CRendererPipelineManager *m_pPipelineManager;
		CRendererRenderPassManager *m_pRenderPassManager;
		CRendererFrameBufferManager *m_pFrameBufferManager;

	protected:
		CRenderer *m_pRenderer;
	};

}
