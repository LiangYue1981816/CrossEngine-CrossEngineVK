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

	class CROSS_EXPORT CGfxDevice
	{
	protected:
		CGfxDevice(void)
		{

		}
		virtual ~CGfxDevice(void)
		{

		}


	public:
		uint32_t GetVertexSize(uint32_t format) const;
		uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const;
		uint32_t GetVertexAttributeFlag(const char *szName) const;
		VkFormat GetVertexAttributeFormat(uint32_t attribute) const;

	public:
		virtual CGfxCommandBufferPtr AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level) = 0;
		virtual CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline) = 0;
		virtual CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline) = 0;

		virtual CGfxIndexBufferPtr NewIndexBuffer(void) = 0;
		virtual CGfxVertexBufferPtr NewVertexBuffer(void) = 0;
		virtual CGfxUniformBufferPtr NewUniformBuffer(void) = 0;

		virtual CGfxTexturePtr NewTexture(void) = 0;
		virtual CGfxRenderTexturePtr NewRenderTexture(void) = 0;

		virtual CGfxShaderPtr NewShader(void) = 0;
		virtual CGfxPipelineComputePtr NewPipelineCompute(void) = 0;
		virtual CGfxPipelineGraphicsPtr NewPipelineGraphics(void) = 0;

		virtual CGfxRenderPassPtr NewRenderPass(void) = 0;
		virtual CGfxFrameBufferPtr NewFrameBuffer(void) = 0;

	public:
		virtual void DumpLog(void) const = 0;
	};

}
