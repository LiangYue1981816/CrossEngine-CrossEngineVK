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

	class CROSS_EXPORT CVulkanCommandBeginRenderPass : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandBeginRenderPass(VkCommandBuffer vkCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_contents(contents)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			std::vector<VkClearValue> clearValues;
			for (uint32_t indexAttachment = 0; indexAttachment < m_ptrRenderPass->GetAttachmentCount(); indexAttachment++) {
				clearValues.push_back(*m_ptrRenderPass->GetAttachmentClearValue(indexAttachment));
			}

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.framebuffer = (VkFramebuffer)m_ptrFrameBuffer->GetHandle();
			renderPassBeginInfo.renderPass = (VkRenderPass)m_ptrRenderPass->GetHandle();
			renderPassBeginInfo.renderArea = { 0, 0, m_ptrFrameBuffer->GetWidth(),m_ptrFrameBuffer->GetHeight() };
			renderPassBeginInfo.clearValueCount = clearValues.size();
			renderPassBeginInfo.pClearValues = clearValues.data();
			vkCmdBeginRenderPass(m_vkCommandBuffer, &renderPassBeginInfo, m_contents);
		}


	protected:
		CGfxFrameBufferPtr m_ptrFrameBuffer;
		CGfxRenderPassPtr m_ptrRenderPass;
		VkSubpassContents m_contents;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
