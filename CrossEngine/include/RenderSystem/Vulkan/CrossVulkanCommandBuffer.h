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

	class CROSS_EXPORT CVulkanCommandBuffer
	{
		friend class CVulkanCommandPool;


	protected:
		CVulkanCommandBuffer(CVulkanCommandPool *pCommandPool, CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level);
		virtual ~CVulkanCommandBuffer(void);


	public:
		VkCommandBuffer GetCommandBuffer(void) const;
		VkCommandBufferLevel GetCommandBufferLevel(void) const;

		CVulkanFencePtr& GetFence(void);
		CVulkanCommandPool* GetCommandPool(void) const;
		

	public:
		void ClearResources(void);
		void Reset(void);

	public:
		VkResult BeginPrimary(VkCommandBufferUsageFlags flags);
		VkResult BeginSecondary(VkCommandBufferUsageFlags flags, const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0);
		VkResult End(void);

		void CmdBindPipelineCompute(const CVulkanPipelineComputePtr &ptrPipelineCompute);
		void CmdBindPipelineGraphics(const CVulkanPipelineGraphicsPtr &ptrPipelineGraphics);
		void CmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);
		void CmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);
		void CmdSetLineWidth(float lineWidth);
		void CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
		void CmdSetBlendConstants(const float blendConstants[4]);
		void CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds);
		void CmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask);
		void CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask);
		void CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference);
		void CmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
		void CmdBindIndexBuffer(const CVulkanIndexBufferPtr &ptrIndexBuffer, VkDeviceSize offset, VkIndexType indexType);
		void CmdBindVertexBuffer(const CVulkanVertexBufferPtr &ptrVertexBuffer, VkDeviceSize offset);
		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
		void CmdDrawIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
		void CmdDrawIndexedIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
		void CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
		void CmdDispatchIndirect(const CVulkanBufferPtr &ptrBuffer, VkDeviceSize offset);
		void CmdClearColorImage(const CVulkanRenderTexturePtr &ptrColorTexture, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
		void CmdClearDepthStencilImage(const CVulkanRenderTexturePtr &ptrDepthStencilTexture, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
		void CmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
		void CmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask);
		void CmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask);
		void CmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
		void CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
		void CmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
		void CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkSubpassContents contents);
		void CmdBeginRenderPass(const CVulkanFrameBufferPtr &ptrFrameBuffer, const CVulkanRenderPassPtr &ptrRenderPass, VkRect2D renderArea, VkSubpassContents contents);
		void CmdNextSubpass(VkSubpassContents contents);
		void CmdEndRenderPass(void);
		void CmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);

		void CmdSetImageLayout(VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, const VkImageSubresourceRange &range);
		void CmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
		void CmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
		void CmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
		void CmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);
		void CmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
		void CmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
		void CmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);

		void CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
		void CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
		void CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
		void CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
		void CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);


	protected:
		VkCommandBuffer m_vkCommandBuffer;
		VkCommandBufferLevel m_vkCommandBufferLevel;
		CVulkanCommandPool *m_pCommandPool;

	protected:
		CVulkanFencePtr m_ptrFence;

	protected:
		std::map<VkFramebuffer, CVulkanFrameBufferPtr> m_ptrFrameBuffers;
		std::map<VkRenderPass, CVulkanRenderPassPtr> m_ptrRenderPasses;
		std::map<VkPipeline, CVulkanPipelineComputePtr> m_ptrPipelineComputes;
		std::map<VkPipeline, CVulkanPipelineGraphicsPtr> m_ptrPipelineGraphics;
		std::map<VkBuffer, CVulkanIndexBufferPtr> m_ptrIndexBuffers;
		std::map<VkBuffer, CVulkanVertexBufferPtr> m_ptrVertexBuffers;
		std::map<VkBuffer, CVulkanBufferPtr> m_ptrBuffers;
		std::map<VkImage, CVulkanRenderTexturePtr> m_ptrRenderTextures;

	protected:
		CVulkanDevice *m_pDevice;

	protected:
		CVulkanCommandBuffer *pFreeNext;
		CVulkanCommandBuffer *pActiveNext;
		CVulkanCommandBuffer *pActivePrev;
	};

}
