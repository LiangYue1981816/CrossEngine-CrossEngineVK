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

}
