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

	CVulkanCommandBuffer::CVulkanCommandBuffer(CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level)
		: m_pDevice(pDevice)

		, m_vkCommandBuffer(vkCommandBuffer)
		, m_vkCommandBufferLevel(level)

		, pFreeNext(NULL)
		, pActiveNext(NULL)
		, pActivePrev(NULL)
	{

	}

	CVulkanCommandBuffer::~CVulkanCommandBuffer(void)
	{

	}

	VkCommandBuffer CVulkanCommandBuffer::GetCommandBuffer(void) const
	{
		return m_vkCommandBuffer;
	}

	VkCommandBufferLevel CVulkanCommandBuffer::GetCommandBufferLevel(void) const
	{
		return m_vkCommandBufferLevel;
	}

	VkResult CVulkanCommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags) const
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = NULL;
		return vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
	}

	VkResult CVulkanCommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics) const
	{
		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.pNext = NULL;
		inheritanceInfo.renderPass = ptrRenderPass->GetRenderPass();
		inheritanceInfo.subpass = indexSubpass;
		inheritanceInfo.framebuffer = ptrFrameBuffer->GetFrameBuffer();
		inheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
		inheritanceInfo.queryFlags = queryFlags;
		inheritanceInfo.pipelineStatistics = pipelineStatistics;

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = &inheritanceInfo;
		return vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
	}

	VkResult CVulkanCommandBuffer::End(void) const
	{
		return vkEndCommandBuffer(m_vkCommandBuffer);
	}

	void CVulkanCommandBuffer::CmdBindPipelineCompute(const CVulkanPipelineComputePtr &ptrPipelineCompute) const
	{
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ptrPipelineCompute->GetPipeline());
	}

	void CVulkanCommandBuffer::CmdBindPipelineGraphics(const CVulkanPipelineGraphicsPtr &ptrPipelineGraphics) const
	{
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ptrPipelineGraphics->GetPipeline());
	}

	void CVulkanCommandBuffer::CmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const
	{
		vkCmdSetViewport(m_vkCommandBuffer, firstViewport, viewportCount, pViewports);
	}

	void CVulkanCommandBuffer::CmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const
	{
		vkCmdSetScissor(m_vkCommandBuffer, firstScissor, scissorCount, pScissors);
	}

	void CVulkanCommandBuffer::CmdSetLineWidth(float lineWidth) const
	{
		vkCmdSetLineWidth(m_vkCommandBuffer, lineWidth);
	}

	void CVulkanCommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const
	{
		vkCmdSetDepthBias(m_vkCommandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
	}

	void CVulkanCommandBuffer::CmdSetBlendConstants(const float blendConstants[4]) const
	{
		vkCmdSetBlendConstants(m_vkCommandBuffer, blendConstants);
	}

	void CVulkanCommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) const
	{
		vkCmdSetDepthBounds(m_vkCommandBuffer, minDepthBounds, maxDepthBounds);
	}

	void CVulkanCommandBuffer::CmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) const
	{
		vkCmdSetStencilCompareMask(m_vkCommandBuffer, faceMask, compareMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) const
	{
		vkCmdSetStencilWriteMask(m_vkCommandBuffer, faceMask, writeMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) const
	{
		vkCmdSetStencilReference(m_vkCommandBuffer, faceMask, reference);
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const
	{
		vkCmdBindDescriptorSets(m_vkCommandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
	}

	void CVulkanCommandBuffer::CmdBindIndexBuffer(const CVulkanIndexBufferPtr &ptrIndexBufer, VkDeviceSize offset, VkIndexType indexType) const
	{
		vkCmdBindIndexBuffer(m_vkCommandBuffer, ptrIndexBufer->GetBuffer(), offset, indexType);
	}

	void CVulkanCommandBuffer::CmdBindVertexBuffer(const CVulkanVertexBufferPtr &ptrVertexBufer, VkDeviceSize offset) const
	{
		VkBuffer vkBuffer = ptrVertexBufer->GetBuffer();
		vkCmdBindVertexBuffers(m_vkCommandBuffer, 0, 1, &vkBuffer, &offset);
	}

	void CVulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const
	{
		vkCmdDraw(m_vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
	{
		vkCmdDrawIndexed(m_vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDrawIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
	{
		vkCmdDrawIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset, drawCount, stride);
	}

	void CVulkanCommandBuffer::CmdDrawIndexedIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
	{
		vkCmdDrawIndexedIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset, drawCount, stride);
	}

	void CVulkanCommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
	{
		vkCmdDispatch(m_vkCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void CVulkanCommandBuffer::CmdDispatchIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset) const
	{
		vkCmdDispatchIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset);
	}

	void CVulkanCommandBuffer::CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const
	{
		vkCmdCopyBuffer(m_vkCommandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const
	{
		vkCmdCopyImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const
	{
		vkCmdBlitImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
	}

	void CVulkanCommandBuffer::CmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
	{
		vkCmdCopyBufferToImage(m_vkCommandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
	{
		vkCmdCopyImageToBuffer(m_vkCommandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const
	{
		vkCmdUpdateBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, dataSize, pData);
	}

	void CVulkanCommandBuffer::CmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const
	{
		vkCmdFillBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, size, data);
	}

	void CVulkanCommandBuffer::CmdClearColorImage(const CVulkanRenderTexturePtr &ptrColorTexture, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
	{
		vkCmdClearColorImage(m_vkCommandBuffer, ptrColorTexture->GetImage(), imageLayout, pColor, rangeCount, pRanges);
	}

	void CVulkanCommandBuffer::CmdClearDepthStencilImage(const CVulkanRenderTexturePtr &ptrDepthStencilTexture, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
	{
		vkCmdClearDepthStencilImage(m_vkCommandBuffer, ptrDepthStencilTexture->GetImage(), imageLayout, pDepthStencil, rangeCount, pRanges);
	}

	void CVulkanCommandBuffer::CmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const
	{
		vkCmdClearAttachments(m_vkCommandBuffer, attachmentCount, pAttachments, rectCount, pRects);
	}

	void CVulkanCommandBuffer::CmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) const
	{
		vkCmdSetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CVulkanCommandBuffer::CmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) const
	{
		vkCmdResetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CVulkanCommandBuffer::CmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
	{
		vkCmdWaitEvents(m_vkCommandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CVulkanCommandBuffer::CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
	{
		vkCmdPipelineBarrier(m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CVulkanCommandBuffer::CmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const
	{
		vkCmdBeginQuery(m_vkCommandBuffer, queryPool, query, flags);
	}

	void CVulkanCommandBuffer::CmdEndQuery(VkQueryPool queryPool, uint32_t query) const
	{
		vkCmdEndQuery(m_vkCommandBuffer, queryPool, query);
	}

	void CVulkanCommandBuffer::CmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const
	{
		vkCmdResetQueryPool(m_vkCommandBuffer, queryPool, firstQuery, queryCount);
	}

	void CVulkanCommandBuffer::CmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const
	{
		vkCmdWriteTimestamp(m_vkCommandBuffer, pipelineStage, queryPool, query);
	}

	void CVulkanCommandBuffer::CmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const
	{
		vkCmdCopyQueryPoolResults(m_vkCommandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
	}

	void CVulkanCommandBuffer::CmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const
	{
		vkCmdPushConstants(m_vkCommandBuffer, layout, stageFlags, offset, size, pValues);
	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkSubpassContents contents) const
	{
		VkRect2D renderArea = { 0, 0, ptrFrameBuffer->GetWidth(),ptrFrameBuffer->GetHeight() };
		CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass, renderArea, contents);
	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkRect2D renderArea, VkSubpassContents contents) const
	{
		std::vector<VkClearValue> clearValues = ptrRenderPass->GetClearValues();
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.framebuffer = ptrFrameBuffer->GetFrameBuffer();
		renderPassBeginInfo.renderPass = ptrRenderPass->GetRenderPass();
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(m_vkCommandBuffer, &renderPassBeginInfo, contents);
	}

	void CVulkanCommandBuffer::CmdNextSubpass(VkSubpassContents contents) const
	{
		vkCmdNextSubpass(m_vkCommandBuffer, contents);
	}

	void CVulkanCommandBuffer::CmdEndRenderPass(void) const
	{
		vkCmdEndRenderPass(m_vkCommandBuffer);
	}

	void CVulkanCommandBuffer::CmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const
	{
		vkCmdExecuteCommands(m_vkCommandBuffer, commandBufferCount, pCommandBuffers);
	}

	void CVulkanCommandBuffer::CmdSetImageLayout(VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, const VkImageSubresourceRange &range) const
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = NULL;
		barrier.srcAccessMask = CVulkanHelper::vkGetAccessMask(oldLayout);
		barrier.dstAccessMask = CVulkanHelper::vkGetAccessMask(newLayout);
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
