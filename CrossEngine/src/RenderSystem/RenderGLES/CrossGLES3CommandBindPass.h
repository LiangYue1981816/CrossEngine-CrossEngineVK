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

	class CROSS_EXPORT CGLES3CommandBindPass : public CGLES3CommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexPass)
			: m_indexPass(indexPass)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrFrameBuffer.IsNull() || m_ptrRenderPass.IsNull()) {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else {
				const CGLES3FrameBuffer *pFrameBuffer = (CGLES3FrameBuffer *)((CGfxFrameBuffer *)m_ptrFrameBuffer);
				const CGLES3RenderPass *pRenderPass = (CGLES3RenderPass *)((CGfxRenderPass *)m_ptrRenderPass);
				const GLSubpassInformation* pPass = pRenderPass->GetSubpass(m_indexPass);

				if (IsValid(pFrameBuffer, pPass)) {
					glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)pFrameBuffer->GetHandle());
					{
						SetRenderColorTexture(pFrameBuffer, pRenderPass, pPass);
						SetRenderDepthStencilTexture(pFrameBuffer, pRenderPass, pPass);
					}
					glReadBuffer(GL_NONE);
					CheckFramebufferStatus();
				}
			}
		}

		BOOL IsValid(const CGLES3FrameBuffer *pFrameBuffer, const GLSubpassInformation* pPass) const
		{
			for (const auto &itColorAttachment : pPass->colorAttachments) {
				if (pFrameBuffer->GetRenderTexture(itColorAttachment.first) != 0) {
					return TRUE;
				}
			}

			return FALSE;
		}

		void SetRenderColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, const GLSubpassInformation* pPass) const
		{
			std::vector<GLenum> drawBuffers;

			for (const auto &itColorAttachment : pPass->colorAttachments) {
				GLenum drawbuffer = GL_DRAW_BUFFER0 + itColorAttachment.first;
				GLenum attachment = GL_COLOR_ATTACHMENT0 + itColorAttachment.first;
				GLuint texture = pFrameBuffer->GetRenderTexture(itColorAttachment.first);

				const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(itColorAttachment.first);
				const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachment(itColorAttachment.first);

				drawBuffers.push_back(attachment);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

				if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
					glClearBufferfv(GL_COLOR, drawbuffer, pClearValue->color.float32);
				}
			}

			glDrawBuffers(drawBuffers.size(), drawBuffers.data());
		}

		void SetRenderDepthStencilTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, const GLSubpassInformation* pPass) const
		{
			GLuint texture = pFrameBuffer->GetRenderTexture(pPass->depthStencilAttachment);
			GLenum format = pFrameBuffer->GetRenderTextureFormat(pPass->depthStencilAttachment);

			const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(pPass->depthStencilAttachment);
			const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachment(pPass->depthStencilAttachment);

			if (CGLES3Helper::glIsFormatDepthOnly(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

				if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
					glClearBufferfv(GL_DEPTH, 0, (const GLfloat *)&pClearValue->depthStencil.depth);
				}

				return;
			}

			if (CGLES3Helper::glIsFormatStencilOnly(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

				if (pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
					glClearBufferiv(GL_STENCIL, 0, (const GLint *)&pClearValue->depthStencil.stencil);
				}

				return;
			}

			if (CGLES3Helper::glIsFormatDepthStencil(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

				if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR || pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
					glClearBufferfi(GL_DEPTH_STENCIL, 0, pClearValue->depthStencil.depth, pClearValue->depthStencil.stencil);
				}

				return;
			}
		}

		BOOL CheckFramebufferStatus(void) const
		{
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			switch (status)
			{
			case GL_FRAMEBUFFER_COMPLETE:
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				LOGE("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\n");
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				LOGE("[ERROR] Framebuffer incomplete: No image is attached to FBO.\n");
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				LOGE("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\n");
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				LOGE("[ERROR] Framebuffer incomplete: Multisample.\n");
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				LOGE("[ERROR] Framebuffer incomplete: Unsupported by FBO implementation.\n");
				break;

			default:
				LOGE("[ERROR] Framebuffer incomplete: Unknown error.\n");
				break;
			}

			return status == GL_FRAMEBUFFER_COMPLETE ? TRUE : FALSE;
		}


	protected:
		CGfxFrameBufferPtr m_ptrFrameBuffer;
		CGfxRenderPassPtr m_ptrRenderPass;
		int m_indexPass;
	};

}
