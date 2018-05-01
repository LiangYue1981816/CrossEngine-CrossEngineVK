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
#include "CrossVulkanCommandBeginCommandBufferPrimary.h"
#include "CrossVulkanCommandBeginCommandBufferSecondary.h"
#include "CrossVulkanCommandBeginRenderPass.h"
#include "CrossVulkanCommandNextSubpass.h"
#include "CrossVulkanCommandEndRenderPass.h"
#include "CrossVulkanCommandEndCommandBuffer.h"
#include "CrossVulkanCommandBindPipelineCompute.h"
#include "CrossVulkanCommandBindPipelineGraphics.h"
#include "CrossVulkanCommandBindIndexBuffer.h"
#include "CrossVulkanCommandBindVertexBuffer.h"
#include "CrossVulkanCommandBindDescriptorSetCompute.h"
#include "CrossVulkanCommandBindDescriptorSetGraphics.h"
#include "CrossVulkanCommandSetViewport.h"
#include "CrossVulkanCommandSetScissor.h"
#include "CrossVulkanCommandSetLineWidth.h"
#include "CrossVulkanCommandSetDepthBias.h"
#include "CrossVulkanCommandSetDepthBounds.h"
#include "CrossVulkanCommandSetBlendConstants.h"
#include "CrossVulkanCommandSetStencilReference.h"
#include "CrossVulkanCommandSetStencilWriteMask.h"
#include "CrossVulkanCommandSetStencilCompareMask.h"
#include "CrossVulkanCommandDraw.h"
#include "CrossVulkanCommandDrawIndexed.h"
#include "CrossVulkanCommandDispatch.h"
#include "CrossVulkanCommandExecuteCommandBuffer.h"


namespace CrossEngine {

	CVulkanCommandBuffer::CVulkanCommandBuffer(CVulkanCommandPool *pCommandPool, CVulkanDevice *pDevice, VkCommandBuffer vkCommandBuffer)
		: m_pDevice(pDevice)
		, m_pCommandPool(pCommandPool)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_vkFence(VK_NULL_HANDLE)
	{
		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		vkCreateFence(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkFence);
	}

	CVulkanCommandBuffer::~CVulkanCommandBuffer(void)
	{
		Reset();
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
	}

	void CVulkanCommandBuffer::Release(void)
	{
		m_pCommandPool->FreeCommandBuffer(this);
	}

	CVulkanDevice* CVulkanCommandBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanCommandBuffer::GetHandle(void) const
	{
		return m_vkCommandBuffer;
	}

	CGfxFence CVulkanCommandBuffer::GetFence(void) const
	{
		return m_vkFence;
	}

	void CVulkanCommandBuffer::Reset(void)
	{
		FenceWait(UINT64_MAX);

		Clearup();
		ClearCommands();

		vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	void CVulkanCommandBuffer::Clearup(void)
	{
		m_ptrRenderPass.Release();
		m_ptrFrameBuffer.Release();
		m_ptrPipelineCompute.Release();
		m_ptrPipelineGraphics.Release();
	}

	void CVulkanCommandBuffer::ClearCommands(void)
	{
		for (auto &itCommand : m_pCommands) {
			SAFE_DELETE(itCommand);
		}

		m_pCommands.clear();
	}

	BOOL CVulkanCommandBuffer::FenceWait(uint64_t timeout) const
	{
		return vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_FALSE, timeout) == VK_SUCCESS ? TRUE : FALSE;
	}

	void CVulkanCommandBuffer::FenceReset(void) const
	{
		vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence);
	}

	void CVulkanCommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBeginCommandBufferPrimary(m_vkCommandBuffer, flags));
	}

	void CVulkanCommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
	{
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBeginCommandBufferSecondary(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, flags, indexSubPass, occlusionQueryEnable, queryFlags, pipelineStatistics));
	}

	void CVulkanCommandBuffer::End(void)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandEndCommandBuffer(m_vkCommandBuffer));
	}

	void CVulkanCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
	{
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBeginRenderPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, contents));
	}

	void CVulkanCommandBuffer::CmdNextSubpass(VkSubpassContents contents)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandNextSubpass(m_vkCommandBuffer, contents));
	}

	void CVulkanCommandBuffer::CmdEndRenderPass(void)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandEndRenderPass(m_vkCommandBuffer));
	}

	void CVulkanCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipeline)
	{
		m_ptrPipelineCompute = ptrPipeline;
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindPipelineCompute(m_vkCommandBuffer, ptrPipeline));
	}

	void CVulkanCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		m_ptrPipelineGraphics = ptrPipeline;
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindPipelineGraphics(m_vkCommandBuffer, ptrPipeline));
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindDescriptorSetCompute(m_vkCommandBuffer, ptrDescriptorSet, m_ptrPipelineCompute));
	}

	void CVulkanCommandBuffer::CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindDescriptorSetGraphics(m_vkCommandBuffer, ptrDescriptorSet, m_ptrPipelineGraphics));
	}

	void CVulkanCommandBuffer::CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindVertexBuffer(m_vkCommandBuffer, ptrVertexBuffer));
	}

	void CVulkanCommandBuffer::CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandBindIndexBuffer(m_vkCommandBuffer, ptrIndexBuffer));
	}

	void CVulkanCommandBuffer::CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetViewport(m_vkCommandBuffer, x, y, width, height, minDepth, maxDepth));
	}

	void CVulkanCommandBuffer::CmdSetScissor(int x, int y, uint32_t width, uint32_t height)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetScissor(m_vkCommandBuffer, x, y, width, height));
	}

	void CVulkanCommandBuffer::CmdSetLineWidth(float lineWidth)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetLineWidth(m_vkCommandBuffer, lineWidth));
	}

	void CVulkanCommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetDepthBias(m_vkCommandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor));
	}

	void CVulkanCommandBuffer::CmdSetBlendConstants(float red, float green, float blue, float alpha)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetBlendConstants(m_vkCommandBuffer, red, green, blue, alpha));
	}

	void CVulkanCommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetDepthBounds(m_vkCommandBuffer, minDepthBounds, maxDepthBounds));
	}

	void CVulkanCommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetStencilWriteMask(m_vkCommandBuffer, faceMask, writeMask));
	}

	void CVulkanCommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference, uint32_t compareMask)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetStencilReference(m_vkCommandBuffer, faceMask, reference));
		m_pCommands.push_back(SAFE_NEW CVulkanCommandSetStencilCompareMask(m_vkCommandBuffer, faceMask, compareMask));
	}

	void CVulkanCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandDraw(m_vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance));
	}

	void CVulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstVertex, uint32_t firstInstance)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandDrawIndexed(m_vkCommandBuffer, indexCount, instanceCount, firstIndex, firstVertex, firstInstance));
	}

	void CVulkanCommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandDispatch(m_vkCommandBuffer, groupCountX, groupCountY, groupCountZ));
	}

	void CVulkanCommandBuffer::CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer)
	{
		m_pCommands.push_back(SAFE_NEW CVulkanCommandExecuteCommandBuffer(m_vkCommandBuffer, ptrCommandBuffer));
	}

}
