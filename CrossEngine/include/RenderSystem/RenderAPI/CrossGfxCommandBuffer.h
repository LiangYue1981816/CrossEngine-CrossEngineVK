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

	class CROSS_EXPORT CGfxCommandBuffer
	{
		friend class CGfxCommandBufferPtr;


	protected:
		CGfxCommandBuffer(void)
		{

		}
		virtual ~CGfxCommandBuffer(void)
		{

		}

		virtual void Release(void) = 0;


	public:
		virtual HANDLE GetHandle(void) const = 0;

	public:
		virtual void Reset(void) = 0;

	public:
		virtual int BeginPrimary(VkCommandBufferUsageFlags flags) = 0;
		virtual int BeginSecondary(VkCommandBufferUsageFlags flags, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0) = 0;
		virtual int End(void) = 0;

	public:
		virtual void CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents) = 0;
		virtual void CmdNextSubpass(VkSubpassContents contents) = 0;
		virtual void CmdEndRenderPass(void) = 0;

		virtual void CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipeline) = 0;
		virtual void CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipeline) = 0;
		virtual void CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxPipelineComputePtr &ptrPipeline) = 0;
		virtual void CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxPipelineGraphicsPtr &ptrPipeline) = 0;
		virtual void CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer) = 0;
		virtual void CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, VkIndexType type) = 0;

		virtual void CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) = 0;
		virtual void CmdSetScissor(int x, int y, uint32_t width, uint32_t height) = 0;
		virtual void CmdSetLineWidth(float lineWidth) = 0;
		virtual void CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) = 0;
		virtual void CmdSetBlendConstants(const float blendConstants[4]) = 0;
		virtual void CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) = 0;
		virtual void CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) = 0;
		virtual void CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference, uint32_t compareMask) = 0;

		virtual void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
		virtual void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
		virtual void CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;

		virtual void CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer) = 0;
	};

	class CROSS_EXPORT CGfxCommandBufferPtr : public CSharedPtr<CGfxCommandBuffer>
	{
	public:
		CGfxCommandBufferPtr(void) : CSharedPtr<CGfxCommandBuffer>()
		{

		}
		CGfxCommandBufferPtr(const CGfxCommandBuffer *p) : CSharedPtr<CGfxCommandBuffer>(p)
		{

		}
		CGfxCommandBufferPtr(const CGfxCommandBufferPtr &ptr) : CSharedPtr<CGfxCommandBuffer>(ptr)
		{

		}
		virtual ~CGfxCommandBufferPtr(void)
		{
			Release();
		}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				m_pPointer->Release();
			}	
		}
	};

}
