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

	CRendererCommandBuffer::CRendererCommandBuffer(CRendererDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level)
		: m_pDevice(pDevice)

		, m_vkCommandBuffer(vkCommandBuffer)
		, m_vkCommandBufferLevel(level)

		, pFreeNext(NULL)
		, pActiveNext(NULL)
		, pActivePrev(NULL)
	{

	}

	CRendererCommandBuffer::~CRendererCommandBuffer(void)
	{

	}

	VkCommandBuffer CRendererCommandBuffer::GetCommandBuffer(void) const
	{
		return m_vkCommandBuffer;
	}

	VkCommandBufferLevel CRendererCommandBuffer::GetCommandBufferLevel(void) const
	{
		return m_vkCommandBufferLevel;
	}

	VkResult CRendererCommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags) const
	{
		VkCommandBufferBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = NULL;
		return vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
	}

	VkResult CRendererCommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, VkFramebuffer vkFramebuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics) const
	{
		VkCommandBufferInheritanceInfo inheritanceInfo;
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.pNext = NULL;
		inheritanceInfo.renderPass = vkRenderPass;
		inheritanceInfo.subpass = indexSubpass;
		inheritanceInfo.framebuffer = vkFramebuffer;
		inheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
		inheritanceInfo.queryFlags = queryFlags;
		inheritanceInfo.pipelineStatistics = pipelineStatistics;

		VkCommandBufferBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = &inheritanceInfo;
		return vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
	}

	VkResult CRendererCommandBuffer::End(void) const
	{
		return vkEndCommandBuffer(m_vkCommandBuffer);
	}

	void CRendererCommandBuffer::CmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const
	{
		vkCmdBindPipeline(m_vkCommandBuffer, pipelineBindPoint, pipeline);
	}

	void CRendererCommandBuffer::CmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const
	{
		vkCmdSetViewport(m_vkCommandBuffer, firstViewport, viewportCount, pViewports);
	}

	void CRendererCommandBuffer::CmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const
	{
		vkCmdSetScissor(m_vkCommandBuffer, firstScissor, scissorCount, pScissors);
	}

	void CRendererCommandBuffer::CmdSetLineWidth(float lineWidth) const
	{
		vkCmdSetLineWidth(m_vkCommandBuffer, lineWidth);
	}

	void CRendererCommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const
	{
		vkCmdSetDepthBias(m_vkCommandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
	}

	void CRendererCommandBuffer::CmdSetBlendConstants(const float blendConstants[4]) const
	{
		vkCmdSetBlendConstants(m_vkCommandBuffer, blendConstants);
	}

	void CRendererCommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) const
	{
		vkCmdSetDepthBounds(m_vkCommandBuffer, minDepthBounds, maxDepthBounds);
	}

	void CRendererCommandBuffer::CmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const
	{
		vkCmdSetStencilCompareMask(m_vkCommandBuffer, faceMask, compareMask);
	}

	void CRendererCommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const
	{
		vkCmdSetStencilWriteMask(m_vkCommandBuffer, faceMask, writeMask);
	}

	void CRendererCommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const
	{
		vkCmdSetStencilReference(m_vkCommandBuffer, faceMask, reference);
	}

	void CRendererCommandBuffer::CmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const
	{
		vkCmdBindDescriptorSets(m_vkCommandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
	}

	void CRendererCommandBuffer::CmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const
	{
		vkCmdBindIndexBuffer(m_vkCommandBuffer, buffer, offset, indexType);
	}

	void CRendererCommandBuffer::CmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const
	{
		vkCmdBindVertexBuffers(m_vkCommandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
	}

	void CRendererCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const
	{
		vkCmdDraw(m_vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CRendererCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
	{
		vkCmdDrawIndexed(m_vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CRendererCommandBuffer::CmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
	{
		vkCmdDrawIndirect(m_vkCommandBuffer, buffer, offset, drawCount, stride);
	}

	void CRendererCommandBuffer::CmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
	{
		vkCmdDrawIndexedIndirect(m_vkCommandBuffer, buffer, offset, drawCount, stride);
	}

	void CRendererCommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
	{
		vkCmdDispatch(m_vkCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void CRendererCommandBuffer::CmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) const
	{
		vkCmdDispatchIndirect(m_vkCommandBuffer, buffer, offset);
	}

	void CRendererCommandBuffer::CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const
	{
		vkCmdCopyBuffer(m_vkCommandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
	}

	void CRendererCommandBuffer::CmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const
	{
		vkCmdCopyImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CRendererCommandBuffer::CmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const
	{
		vkCmdBlitImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
	}

	void CRendererCommandBuffer::CmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
	{
		vkCmdCopyBufferToImage(m_vkCommandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CRendererCommandBuffer::CmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
	{
		vkCmdCopyImageToBuffer(m_vkCommandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
	}

	void CRendererCommandBuffer::CmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const
	{
		vkCmdUpdateBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, dataSize, pData);
	}

	void CRendererCommandBuffer::CmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const
	{
		vkCmdFillBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, size, data);
	}

	void CRendererCommandBuffer::CmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
	{
		vkCmdClearColorImage(m_vkCommandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
	}

	void CRendererCommandBuffer::CmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
	{
		vkCmdClearDepthStencilImage(m_vkCommandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
	}

	void CRendererCommandBuffer::CmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const
	{
		vkCmdClearAttachments(m_vkCommandBuffer, attachmentCount, pAttachments, rectCount, pRects);
	}

	void CRendererCommandBuffer::CmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const
	{
		vkCmdResolveImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CRendererCommandBuffer::CmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) const
	{
		vkCmdSetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CRendererCommandBuffer::CmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const
	{
		vkCmdResetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CRendererCommandBuffer::CmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
	{
		vkCmdWaitEvents(m_vkCommandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CRendererCommandBuffer::CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
	{
		vkCmdPipelineBarrier(m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CRendererCommandBuffer::CmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const
	{
		vkCmdBeginQuery(m_vkCommandBuffer, queryPool, query, flags);
	}

	void CRendererCommandBuffer::CmdEndQuery(VkQueryPool queryPool, uint32_t query) const
	{
		vkCmdEndQuery(m_vkCommandBuffer, queryPool, query);
	}

	void CRendererCommandBuffer::CmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const
	{
		vkCmdResetQueryPool(m_vkCommandBuffer, queryPool, firstQuery, queryCount);
	}

	void CRendererCommandBuffer::CmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const
	{
		vkCmdWriteTimestamp(m_vkCommandBuffer, pipelineStage, queryPool, query);
	}

	void CRendererCommandBuffer::CmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const
	{
		vkCmdCopyQueryPoolResults(m_vkCommandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
	}

	void CRendererCommandBuffer::CmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const
	{
		vkCmdPushConstants(m_vkCommandBuffer, layout, stageFlags, offset, size, pValues);
	}

	void CRendererCommandBuffer::CmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const
	{
		vkCmdBeginRenderPass(m_vkCommandBuffer, pRenderPassBegin, contents);
	}

	void CRendererCommandBuffer::CmdNextSubpass(VkSubpassContents contents) const
	{
		vkCmdNextSubpass(m_vkCommandBuffer, contents);
	}

	void CRendererCommandBuffer::CmdEndRenderPass(void) const
	{
		vkCmdEndRenderPass(m_vkCommandBuffer);
	}

	void CRendererCommandBuffer::CmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const
	{
		vkCmdExecuteCommands(m_vkCommandBuffer, commandBufferCount, pCommandBuffers);
	}

	void CRendererCommandBuffer::CmdSetImageLayout(VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, const VkImageSubresourceRange &range) const
	{
		VkImageMemoryBarrier barrier;
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = NULL;
		barrier.srcAccessMask = CRendererHelper::vkGetAccessMask(oldLayout);
		barrier.dstAccessMask = CRendererHelper::vkGetAccessMask(newLayout);
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = vkImage;
		barrier.subresourceRange = range;

		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}

		vkCmdPipelineBarrier(m_vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, NULL, 0, NULL, 1, &barrier);
	}

}
