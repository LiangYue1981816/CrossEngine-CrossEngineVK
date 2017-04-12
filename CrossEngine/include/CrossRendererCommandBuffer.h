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

	class CROSS_EXPORT CRendererCommandBuffer
	{
		friend class CRendererCommandPool;


	protected:
		CRendererCommandBuffer(CRendererDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level);
		virtual ~CRendererCommandBuffer(void);


	public:
		VkCommandBuffer GetCommandBuffer(void) const;
		VkCommandBufferLevel GetCommandBufferLevel(void) const;

	public:
		VkResult BeginPrimary(VkCommandBufferUsageFlags flags) const;
		VkResult BeginSecondary(VkCommandBufferUsageFlags flags, VkFramebuffer vkFramebuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0) const;
		VkResult End(void) const;

		void CmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;
		void CmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const;
		void CmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const;
		void CmdSetLineWidth(float lineWidth) const;
		void CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const;
		void CmdSetBlendConstants(const float blendConstants[4]) const;
		void CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) const;
		void CmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const;
		void CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const;
		void CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const;
		void CmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const;
		void CmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const;
		void CmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const;
		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
		void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const;
		void CmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		void CmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		void CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
		void CmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const;
		void CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const;
		void CmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const;
		void CmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const;
		void CmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
		void CmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
		void CmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const;
		void CmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const;
		void CmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
		void CmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
		void CmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const;
		void CmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const;
		void CmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) const;
		void CmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const;
		void CmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
		void CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
		void CmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const;
		void CmdEndQuery(VkQueryPool queryPool, uint32_t query) const;
		void CmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
		void CmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const;
		void CmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const;
		void CmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const;
		void CmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const;
		void CmdNextSubpass(VkSubpassContents contents) const;
		void CmdEndRenderPass(void) const;
		void CmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const;


	protected:
		VkCommandBuffer m_vkCommandBuffer;
		VkCommandBufferLevel m_vkCommandBufferLevel;

	protected:
		CRendererDevice *m_pDevice;

	protected:
		CRendererCommandBuffer *pFreeNext;
		CRendererCommandBuffer *pActiveNext;
		CRendererCommandBuffer *pActivePrev;
	};

}
