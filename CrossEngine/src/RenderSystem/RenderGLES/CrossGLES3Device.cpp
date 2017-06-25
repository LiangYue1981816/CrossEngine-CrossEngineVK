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

	CGLES3Device::CGLES3Device(void)
	{

	}

	CGLES3Device::~CGLES3Device(void)
	{

	}

	CGfxCommandBufferPtr CGLES3Device::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return CGfxCommandBufferPtr(NULL);
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxIndexBufferPtr CGLES3Device::NewIndexBuffer(void)
	{
		return CGfxIndexBufferPtr(NULL);
	}

	CGfxVertexBufferPtr CGLES3Device::NewVertexBuffer(void)
	{
		return CGfxVertexBufferPtr(NULL);
	}

	CGfxUniformBufferPtr CGLES3Device::NewUniformBuffer(void)
	{
		return CGfxUniformBufferPtr(NULL);
	}

	CGfxTexturePtr CGLES3Device::NewTexture(void)
	{
		return CGfxTexturePtr(NULL);
	}

	CGfxRenderTexturePtr CGLES3Device::NewRenderTexture(void)
	{
		return CGfxRenderTexturePtr(NULL);
	}

	CGfxShaderPtr CGLES3Device::NewShader(void)
	{
		return CGfxShaderPtr(NULL);
	}

	CGfxPipelineComputePtr CGLES3Device::NewPipelineCompute(void)
	{
		return CGfxPipelineComputePtr(NULL);
	}

	CGfxPipelineGraphicsPtr CGLES3Device::NewPipelineGraphics(void)
	{
		return CGfxPipelineGraphicsPtr(NULL);
	}

	CGfxRenderPassPtr CGLES3Device::NewRenderPass(void)
	{
		return CGfxRenderPassPtr(NULL);
	}

	CGfxFrameBufferPtr CGLES3Device::NewFrameBuffer(void)
	{
		return CGfxFrameBufferPtr(NULL);
	}

	void CGLES3Device::DumpLog(void) const
	{

	}

}
