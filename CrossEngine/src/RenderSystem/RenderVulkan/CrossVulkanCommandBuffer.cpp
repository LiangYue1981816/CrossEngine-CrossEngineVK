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

	CVulkanCommandBuffer::CVulkanCommandBuffer(CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer)
		: m_pDevice(pDevice)

		, m_vkFence(VK_NULL_HANDLE)
		, m_vkCommandBuffer(vkCommandBuffer)
	{
		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		vkCreateFence(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence);
	}

	CVulkanCommandBuffer::~CVulkanCommandBuffer(void)
	{
		Reset();
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
	}

	void CVulkanCommandBuffer::Clearup(void)
	{

	}

	HANDLE CVulkanCommandBuffer::GetHandle(void) const
	{
		return m_vkCommandBuffer;
	}

	VkFence CVulkanCommandBuffer::GetFence(void) const
	{
		return m_vkFence;
	}

	void CVulkanCommandBuffer::Reset(void)
	{
		FenceWait(UINT64_MAX);
		Clearup();
		vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	BOOL CVulkanCommandBuffer::FenceWait(uint64_t timeout) const
	{
		return vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_FALSE, timeout) == VK_SUCCESS ? TRUE : FALSE;
	}

	void CVulkanCommandBuffer::FenceReset(void) const
	{
		vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence);
	}

	VkResult CVulkanCommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags)
	{
		return vkBeginCommandBufferPrimary(m_vkCommandBuffer, flags);
	}

	VkResult CVulkanCommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
	{
		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.pNext = NULL;
		inheritanceInfo.renderPass = (VkRenderPass)ptrRenderPass->GetHandle();
		inheritanceInfo.subpass = indexSubpass;
		inheritanceInfo.framebuffer = (VkFramebuffer)ptrFrameBuffer->GetHandle();
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

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
	{

	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkRect2D renderArea, VkSubpassContents contents)
	{

	}

	void CVulkanCommandBuffer::CmdNextSubpass(VkSubpassContents contents)
	{
		vkCmdNextSubpass(m_vkCommandBuffer, contents);
	}

	void CVulkanCommandBuffer::CmdEndRenderPass(void)
	{
		vkCmdEndRenderPass(m_vkCommandBuffer);
	}

	void CVulkanCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipelineCompute)
	{
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, (VkPipeline)ptrPipelineCompute->GetHandle());
	}

	void CVulkanCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
	{
		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (VkPipeline)ptrPipelineGraphics->GetHandle());
	}

	void CVulkanCommandBuffer::CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, IndexType indexType)
	{
		vkCmdBindIndexBuffer(m_vkCommandBuffer, (VkBuffer)ptrIndexBuffer->GetHandle(), offset, (VkIndexType)indexType);
	}

	void CVulkanCommandBuffer::CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, size_t offset)
	{
		VkBuffer vkBuffer = (VkBuffer)ptrVertexBuffer->GetHandle();
		vkCmdBindVertexBuffers(m_vkCommandBuffer, 0, 1, &vkBuffer, &offset);
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE layout)
	{
		VkDescriptorSet vkDescriptorSet = (VkDescriptorSet)ptrDescriptorSet->GetHandle();
		vkCmdBindDescriptorSets(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, (VkPipelineLayout)layout, 0, 1, &vkDescriptorSet, 0, NULL);
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE layout)
	{
		VkDescriptorSet vkDescriptorSet = (VkDescriptorSet)ptrDescriptorSet->GetHandle();
		vkCmdBindDescriptorSets(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (VkPipelineLayout)layout, 0, 1, &vkDescriptorSet, 0, NULL);
	}

	void CVulkanCommandBuffer::CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
	{
		VkViewport viewport = { 0 };
		viewport.x = x;
		viewport.y = y;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = minDepth;
		viewport.maxDepth = maxDepth;

		vkCmdSetViewport(m_vkCommandBuffer, 0, 1, &viewport);
	}

	void CVulkanCommandBuffer::CmdSetScissor(int x, int y, uint32_t width, uint32_t height)
	{
		VkRect2D scissor = { 0 };
		scissor.offset.x = x;
		scissor.offset.y = y;
		scissor.extent.width = width;
		scissor.extent.height = height;

		vkCmdSetScissor(m_vkCommandBuffer, 0, 1, &scissor);
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

	void CVulkanCommandBuffer::CmdSetStencilCompareMask(StencilFaceFlags faceMask, uint32_t compareMask)
	{
		vkCmdSetStencilCompareMask(m_vkCommandBuffer, faceMask, compareMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilWriteMask(StencilFaceFlags faceMask, uint32_t writeMask)
	{
		vkCmdSetStencilWriteMask(m_vkCommandBuffer, faceMask, writeMask);
	}

	void CVulkanCommandBuffer::CmdSetStencilReference(StencilFaceFlags faceMask, uint32_t reference)
	{
		vkCmdSetStencilReference(m_vkCommandBuffer, faceMask, reference);
	}

	void CVulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(m_vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CVulkanCommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		vkCmdDispatch(m_vkCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void CVulkanCommandBuffer::CmdExecuteCommands(uint32_t commandBufferCount, const CGfxCommandBuffer* pCommandBuffers)
	{
		std::vector<VkCommandBuffer> commandBuffers;
		for (uint32_t index = 0; index < commandBufferCount; index++) {
			commandBuffers.push_back((VkCommandBuffer)pCommandBuffers[index].GetHandle());
		}
		vkCmdExecuteCommands(m_vkCommandBuffer, commandBuffers.size(), commandBuffers.data());
	}

}
