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

	class CROSS_EXPORT CVulkanCommandBuffer : public CGfxCommandBuffer
	{
		friend class CVulkanCommandPool;


	protected:
		CVulkanCommandBuffer(CVulkanCommandPool *pCommandPool, CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer);
		virtual ~CVulkanCommandBuffer(void);


	protected:
		void Release(void);
		void Clearup(void);

	public:
		HANDLE GetHandle(void) const;
		VkFence GetFence(void) const;

	public:
		void Reset(void);
		BOOL FenceWait(uint64_t timeout) const;
		void FenceReset(void) const;

	public:
		VkResult BeginPrimary(VkCommandBufferUsageFlags flags);
		VkResult BeginSecondary(VkCommandBufferUsageFlags flags, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0);
		VkResult End(void);

	public:
		void CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents);
		void CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkRect2D renderArea, VkSubpassContents contents);
		void CmdNextSubpass(VkSubpassContents contents);
		void CmdEndRenderPass(void);

		void CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipeline);
		void CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipeline);
		void CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE layout);
		void CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE layout);
		void CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, size_t offset);
		void CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, IndexType indexType);

		void CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
		void CmdSetScissor(int x, int y, uint32_t width, uint32_t height);
		void CmdSetLineWidth(float lineWidth);
		void CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
		void CmdSetBlendConstants(const float blendConstants[4]);
		void CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds);
		void CmdSetStencilCompareMask(StencilFaceFlags faceMask, uint32_t compareMask);
		void CmdSetStencilWriteMask(StencilFaceFlags faceMask, uint32_t writeMask);
		void CmdSetStencilReference(StencilFaceFlags faceMask, uint32_t reference);

		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
		void CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		void CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer);


	protected:
		VkFence m_vkFence;
		VkCommandBuffer m_vkCommandBuffer;
		CVulkanCommandPool *m_pCommandPool;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
