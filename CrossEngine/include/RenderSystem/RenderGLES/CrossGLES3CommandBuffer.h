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

	class CROSS_EXPORT CGLES3CommandBuffer : public CGfxCommandBuffer
	{
		friend class CGLES3CommandBufferManager;


	protected:
		CGLES3CommandBuffer(CGLES3Device *pDevice, CGLES3CommandBufferManager *pCommandBufferManager);
		virtual ~CGLES3CommandBuffer(void);


	protected:
		void Release(void);
		void Clearup(void);
		void ClearCommands(void);


	public:
		CGLES3Device* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		void Reset(void);
		void Execute(void) const;

	public:
		void BeginPrimary(VkCommandBufferUsageFlags flags);
		void BeginSecondary(VkCommandBufferUsageFlags flags, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0);
		void End(void);

	public:
		void CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents);
		void CmdNextSubpass(VkSubpassContents contents);
		void CmdEndRenderPass(void);

		void CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipeline);
		void CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipeline);
		void CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxPipelineComputePtr &ptrPipeline);
		void CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxPipelineGraphicsPtr &ptrPipeline);
		void CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, size_t offset, uint32_t binding);
		void CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, VkIndexType type);

		void CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
		void CmdSetScissor(int x, int y, uint32_t width, uint32_t height);
		void CmdSetLineWidth(float lineWidth);
		void CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
		void CmdSetBlendConstants(float red, float green, float blue, float alpha);
		void CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds);
		void CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask);
		void CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference, uint32_t compareMask);

		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
		void CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		void CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer);


	protected:
		uint32_t m_indexPass;

		uint32_t m_indexOffset;
		VkIndexType m_indexType;
		VkPrimitiveTopology m_topology;

		VkCompareOp m_frontCompareOp;
		VkCompareOp m_backCompareOp;

	protected:
		CGfxRenderPassPtr m_ptrRenderPass;
		CGfxFrameBufferPtr m_ptrFrameBuffer;

		std::vector<CGfxCommandBase*> m_pCommands;

	protected:
		CGLES3CommandBufferManager *m_pCommandBufferManager;

	protected:
		CGLES3Device *m_pDevice;
	};

}
