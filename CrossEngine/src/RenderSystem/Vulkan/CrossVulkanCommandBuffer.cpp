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

	CVulkanCommandBuffer::CVulkanCommandBuffer(CVulkanCommandPool *pCommandPool, CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level)
		: m_pDevice(pDevice)
		, m_pCommandPool(pCommandPool)

		, m_vkCommandBuffer(vkCommandBuffer)
		, m_vkCommandBufferLevel(level)

		, pFreeNext(NULL)
		, pActiveNext(NULL)
		, pActivePrev(NULL)
	{

	}

	CVulkanCommandBuffer::~CVulkanCommandBuffer(void)
	{
		ClearResources();
	}

	VkCommandBuffer CVulkanCommandBuffer::GetCommandBuffer(void) const
	{
		return m_vkCommandBuffer;
	}

	VkCommandBufferLevel CVulkanCommandBuffer::GetCommandBufferLevel(void) const
	{
		return m_vkCommandBufferLevel;
	}

	CVulkanCommandPool* CVulkanCommandBuffer::GetCommandPool(void) const
	{
		return m_pCommandPool;
	}

	void CVulkanCommandBuffer::ClearResources(void)
	{
		for (auto &itFrameBuffer : m_ptrFrameBuffers) {
			CVulkanFrameBufferPtr &ptrFrameBuffer = itFrameBuffer.second;
			ptrFrameBuffer.SetNull();
		}

		for (auto &itRenderPass : m_ptrRenderPasses) {
			CVulkanRenderPassPtr &ptrRenderPass = itRenderPass.second;
			ptrRenderPass.SetNull();
		}

		for (auto &itPipeline : m_ptrPipelineComputes) {
			CVulkanPipelineComputePtr &ptrPipeline = itPipeline.second;
			ptrPipeline.SetNull();
		}

		for (auto &itPipeline : m_ptrPipelineGraphics) {
			CVulkanPipelineGraphicsPtr &ptrPipeline = itPipeline.second;
			ptrPipeline.SetNull();
		}

		for (auto &itBuffer : m_ptrIndexBuffers) {
			CVulkanIndexBufferPtr &ptrBuffer = itBuffer.second;
			ptrBuffer.SetNull();
		}

		for (auto &itBuffer : m_ptrVertexBuffers) {
			CVulkanVertexBufferPtr &ptrBuffer = itBuffer.second;
			ptrBuffer.SetNull();
		}

		for (auto &itBuffer : m_ptrBuffers) {
			CVulkanBufferPtr &ptrBuffer = itBuffer.second;
			ptrBuffer.SetNull();
		}

		for (auto &itTexture : m_ptrRenderTextures) {
			CVulkanRenderTexturePtr &ptrTexture = itTexture.second;
			ptrTexture.SetNull();
		}

		m_ptrFrameBuffers.clear();
		m_ptrRenderPasses.clear();
		m_ptrPipelineComputes.clear();
		m_ptrPipelineGraphics.clear();
		m_ptrIndexBuffers.clear();
		m_ptrVertexBuffers.clear();
		m_ptrBuffers.clear();
		m_ptrRenderTextures.clear();
	}

	void CVulkanCommandBuffer::Reset(void)
	{
		ClearResources();
		vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	VkResult CVulkanCommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.flags = flags;
		beginInfo.pInheritanceInfo = NULL;
		return vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
	}

	VkResult CVulkanCommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
	{
		m_ptrFrameBuffers[ptrFrameBuffer->GetFrameBuffer()] = ptrFrameBuffer;
		m_ptrRenderPasses[ptrRenderPass->GetRenderPass()] = ptrRenderPass;

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

	VkResult CVulkanCommandBuffer::End(void)
	{
		return vkEndCommandBuffer(m_vkCommandBuffer);
	}

	void CVulkanCommandBuffer::CmdBindPipelineCompute(const CVulkanPipelineComputePtr &ptrPipelineCompute)
	{
		m_ptrPipelineComputes[ptrPipelineCompute->GetPipeline()] = ptrPipelineCompute;
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ptrPipelineCompute->GetPipeline());
	}

	void CVulkanCommandBuffer::CmdBindPipelineGraphics(const CVulkanPipelineGraphicsPtr &ptrPipelineGraphics)
	{
		m_ptrPipelineGraphics[ptrPipelineGraphics->GetPipeline()] = ptrPipelineGraphics;
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ptrPipelineGraphics->GetPipeline());
	}

	void CVulkanCommandBuffer::CmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
	{
		vkCmdSetViewport(m_vkCommandBuffer, firstViewport, viewportCount, pViewports);
	}

	void CVulkanCommandBuffer::CmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
	{
		vkCmdSetScissor(m_vkCommandBuffer, firstScissor, scissorCount, pScissors);
	}

	void CVulkanCommandBuffer::CmdSetLineWidth(float lineWidth)
	{
		vkCmdSetLineWidth(m_vkCommandBuffer, lineWidth);
	}

	void CVulkanCommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		vkCmdSetDepthBias(m_vkCommandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
	}

	void CVulkanCommandBuffer::CmdSetBlendConstants(const float blendConstants[4])
	{
		vkCmdSetBlendConstants(m_vkCommandBuffer, blendConstants);
	}

	void CVulkanCommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds)
	{
		vkCmdSetDepthBounds(m_vkCommandBuffer, minDepthBounds, maxDepthBounds);
	}

	void CVulkanCommandBuffer::CmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask)
	{
		vkCmdSetStencilCompareMask(m_vkCommandBuffer, faceMask, compareMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask)
	{
		vkCmdSetStencilWriteMask(m_vkCommandBuffer, faceMask, writeMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference)
	{
		vkCmdSetStencilReference(m_vkCommandBuffer, faceMask, reference);
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
	{
		vkCmdBindDescriptorSets(m_vkCommandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
	}

	void CVulkanCommandBuffer::CmdBindIndexBuffer(const CVulkanIndexBufferPtr &ptrIndexBuffer, VkDeviceSize offset, VkIndexType indexType)
	{
		m_ptrIndexBuffers[ptrIndexBuffer->GetBuffer()] = ptrIndexBuffer;
		vkCmdBindIndexBuffer(m_vkCommandBuffer, ptrIndexBuffer->GetBuffer(), offset, indexType);
	}

	void CVulkanCommandBuffer::CmdBindVertexBuffer(const CVulkanVertexBufferPtr &ptrVertexBuffer, VkDeviceSize offset)
	{
		m_ptrVertexBuffers[ptrVertexBuffer->GetBuffer()] = ptrVertexBuffer;

		VkBuffer vkBuffer = ptrVertexBuffer->GetBuffer();
		vkCmdBindVertexBuffers(m_vkCommandBuffer, 0, 1, &vkBuffer, &offset);
	}

	void CVulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(m_vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDrawIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
	{
		m_ptrBuffers[ptrBuffer->GetBuffer()] = ptrBuffer;
		vkCmdDrawIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset, drawCount, stride);
	}

	void CVulkanCommandBuffer::CmdDrawIndexedIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
	{
		m_ptrBuffers[ptrBuffer->GetBuffer()] = ptrBuffer;
		vkCmdDrawIndexedIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset, drawCount, stride);
	}

	void CVulkanCommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		vkCmdDispatch(m_vkCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void CVulkanCommandBuffer::CmdDispatchIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset)
	{
		m_ptrBuffers[ptrBuffer->GetBuffer()] = ptrBuffer;
		vkCmdDispatchIndirect(m_vkCommandBuffer, ptrBuffer->GetBuffer(), offset);
	}

	void CVulkanCommandBuffer::CmdClearColorImage(const CVulkanRenderTexturePtr &ptrColorTexture, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
	{
		m_ptrRenderTextures[ptrColorTexture->GetImage()] = ptrColorTexture;
		vkCmdClearColorImage(m_vkCommandBuffer, ptrColorTexture->GetImage(), imageLayout, pColor, rangeCount, pRanges);
	}

	void CVulkanCommandBuffer::CmdClearDepthStencilImage(const CVulkanRenderTexturePtr &ptrDepthStencilTexture, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
	{
		m_ptrRenderTextures[ptrDepthStencilTexture->GetImage()] = ptrDepthStencilTexture;
		vkCmdClearDepthStencilImage(m_vkCommandBuffer, ptrDepthStencilTexture->GetImage(), imageLayout, pDepthStencil, rangeCount, pRanges);
	}

	void CVulkanCommandBuffer::CmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
	{
		vkCmdClearAttachments(m_vkCommandBuffer, attachmentCount, pAttachments, rectCount, pRects);
	}

	void CVulkanCommandBuffer::CmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask)
	{
		vkCmdSetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CVulkanCommandBuffer::CmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask)
	{
		vkCmdResetEvent(m_vkCommandBuffer, event, stageMask);
	}

	void CVulkanCommandBuffer::CmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
	{
		vkCmdWaitEvents(m_vkCommandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CVulkanCommandBuffer::CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
	{
		vkCmdPipelineBarrier(m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
	}

	void CVulkanCommandBuffer::CmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
	{
		vkCmdPushConstants(m_vkCommandBuffer, layout, stageFlags, offset, size, pValues);
	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
	{
		m_ptrFrameBuffers[ptrFrameBuffer->GetFrameBuffer()] = ptrFrameBuffer;
		m_ptrRenderPasses[ptrRenderPass->GetRenderPass()] = ptrRenderPass;

		VkRect2D renderArea = { 0, 0, ptrFrameBuffer->GetWidth(),ptrFrameBuffer->GetHeight() };
		CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass, renderArea, contents);
	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkRect2D renderArea, VkSubpassContents contents)
	{
		m_ptrFrameBuffers[ptrFrameBuffer->GetFrameBuffer()] = ptrFrameBuffer;
		m_ptrRenderPasses[ptrRenderPass->GetRenderPass()] = ptrRenderPass;

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

	void CVulkanCommandBuffer::CmdNextSubpass(VkSubpassContents contents)
	{
		vkCmdNextSubpass(m_vkCommandBuffer, contents);
	}

	void CVulkanCommandBuffer::CmdEndRenderPass(void)
	{
		vkCmdEndRenderPass(m_vkCommandBuffer);
	}

	void CVulkanCommandBuffer::CmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
	{
		vkCmdExecuteCommands(m_vkCommandBuffer, commandBufferCount, pCommandBuffers);
	}

	void CVulkanCommandBuffer::CmdSetImageLayout(VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, const VkImageSubresourceRange &range)
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

	void CVulkanCommandBuffer::CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
	{
		vkCmdCopyBuffer(m_vkCommandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
	{
		vkCmdCopyImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
	{
		vkCmdBlitImage(m_vkCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
	}

	void CVulkanCommandBuffer::CmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
	{
		vkCmdCopyBufferToImage(m_vkCommandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
	{
		vkCmdCopyImageToBuffer(m_vkCommandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
	}

	void CVulkanCommandBuffer::CmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
	{
		vkCmdUpdateBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, dataSize, pData);
	}

	void CVulkanCommandBuffer::CmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
	{
		vkCmdFillBuffer(m_vkCommandBuffer, dstBuffer, dstOffset, size, data);
	}

	void CVulkanCommandBuffer::CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
	{
		vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
	}

	void CVulkanCommandBuffer::CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
	{
		vkCmdEndQuery(commandBuffer, queryPool, query);
	}

	void CVulkanCommandBuffer::CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
	{
		vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
	}

	void CVulkanCommandBuffer::CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
	{
		vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
	}

	void CVulkanCommandBuffer::CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
	{
		vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
	}

}
