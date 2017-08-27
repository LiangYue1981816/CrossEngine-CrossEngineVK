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

	class CROSS_EXPORT CGLES3Device : public CGfxDevice
	{
		friend class CGLES3Instance;


	protected:
		CGLES3Device(CGLES3Instance *pInstance);
		virtual ~CGLES3Device(void);


	protected:
		int Create(void);
		void Destroy(void);

	protected:
		int CreateDevice(void);
		int CreateQueue(void);
		int CreateCommandManager(void);
		int CreateDescriptorSetManager(void);

		int CreateBufferManager(void);
		int CreateTextureManager(void);
		int CreateShaderManager(void);
		int CreatePipelineManager(void);
		int CreateRenderPassManager(void);
		int CreateFrameBufferManager(void);

	protected:
		void DestroyDevice(void);
		void DestroyQueue(void);
		void DestroyCommandManager(void);
		void DestroyDescriptorSetManager(void);

		void DestroyBufferManager(void);
		void DestroyTextureManager(void);
		void DestroyShaderManager(void);
		void DestroyPipelineManager(void);
		void DestroyRenderPassManager(void);
		void DestroyFrameBufferManager(void);

	protected:
		CGfxQueue* GetQueue(void) const;
		CGfxInstance* GetInstance(void) const;

	public:
		const GLPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;

	public:
		CGfxCommandBufferPtr AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline);

		CGfxIndexBufferPtr NewIndexBuffer(void);
		CGfxVertexBufferPtr NewVertexBuffer(void);
		CGfxUniformBufferPtr NewUniformBuffer(void);

		CGfxTexturePtr NewTexture(void);
		CGfxRenderTexturePtr NewRenderTexture(void);

		CGfxShaderPtr NewShader(void);
		CGfxPipelineComputePtr NewPipelineCompute(void);
		CGfxPipelineGraphicsPtr NewPipelineGraphics(void);

		CGfxRenderPassPtr NewRenderPass(void);
		CGfxFrameBufferPtr NewFrameBuffer(void);

	public:
		void SetShaderCachePath(const char *szPath);
		void AddShaderIncludePath(const char *szPath);
		void AddShaderMacroDefinition(const char *szName);
		void AddShaderMacroDefinition(const char *szName, const char *szValue);

	public:
		int Submit(const CGfxCommandBuffer *pCommandBuffer) const;
		int Submit(const CGfxCommandBuffer *pCommandBuffer, CGfxSemaphore waitSemaphore, VkPipelineStageFlags waitStageFlags, CGfxSemaphore signalSemaphore) const;
		int WaitIdle(void) const;

	public:
		void DumpLog(void) const;


	protected:
		CGLES3DeviceProperties m_properties;

	protected:
		CGLES3Queue *m_pQueue;
		CGLES3CommandBufferManager *m_pCommandBufferManager;
		CGLES3DescriptorSetManager *m_pDescriptorSetManager;

	protected:
		CGLES3BufferManager *m_pBufferManager;
		CGLES3TextureManager *m_pTextureManager;
		CGLES3ShaderManager *m_pShaderManager;
		CGLES3PipelineManager *m_pPipelineManager;
		CGLES3RenderPassManager *m_pRenderPassManager;
		CGLES3FrameBufferManager *m_pFrameBufferManager;

	protected:
		CGLES3Instance *m_pInstance;
	};

}
