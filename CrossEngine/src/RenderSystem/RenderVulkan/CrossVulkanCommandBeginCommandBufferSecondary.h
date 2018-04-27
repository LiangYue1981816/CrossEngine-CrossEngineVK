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

	class CROSS_EXPORT CVulkanCommandBeginCommandBufferSecondary : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkCommandBufferUsageFlags flags, uint32_t indexSubPass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_flags(flags)
			, m_indexSubPass(indexSubPass)
			, m_occlusionQueryEnable(occlusionQueryEnable)
			, m_queryFlags(queryFlags)
			, m_pipelineStatistics(pipelineStatistics)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrFrameBuffer.IsNull() || m_ptrFrameBuffer->GetHandle() == NULL) {
				return;
			}

			if (m_ptrRenderPass.IsNull() || m_ptrRenderPass->GetHandle() == NULL) {
				return;
			}

			VkFramebuffer vkFrameBuffer = (VkFramebuffer)m_ptrFrameBuffer->GetHandle();
			VkRenderPass vkRenderPass = (VkRenderPass)m_ptrRenderPass->GetHandle();
			vkBeginCommandBufferSecondary(m_vkCommandBuffer, m_flags, vkFrameBuffer, vkRenderPass, m_indexSubPass, m_occlusionQueryEnable, m_queryFlags, m_pipelineStatistics);
		}


	protected:
		CGfxFrameBufferPtr m_ptrFrameBuffer;
		CGfxRenderPassPtr m_ptrRenderPass;
		VkCommandBufferUsageFlags m_flags;
		uint32_t m_indexSubPass;
		VkBool32 m_occlusionQueryEnable;
		VkQueryControlFlags m_queryFlags;
		VkQueryPipelineStatisticFlags m_pipelineStatistics;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
