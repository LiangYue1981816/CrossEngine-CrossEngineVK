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

	class CROSS_EXPORT CGLES3CommandResolve : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandResolve(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexPass)
			: m_indexPass(indexPass)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
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

			const CGLES3FrameBuffer *pFrameBuffer = (CGLES3FrameBuffer *)((CGfxFrameBuffer *)m_ptrFrameBuffer);
			const CGLES3RenderPass *pRenderPass = (CGLES3RenderPass *)((CGfxRenderPass *)m_ptrRenderPass);

			if (IsNeedResolve(pFrameBuffer, pRenderPass, m_indexPass)) {
				const GLuint framebuffer = (GLuint)pFrameBuffer->GetHandle();
				GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
				{
					eastl::vector<GLenum> drawBuffers;
					SetRenderColorTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, drawBuffers);

					GLReadBuffers(GL_DRAW_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());
					GLDrawBuffers(GL_DRAW_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());

					CheckFramebufferStatus(framebuffer);
				}

				const GLuint framebufferMSAA = (GLuint)pFrameBuffer->GetHandleMSAA();
				GLBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMSAA);
				{
					glBlitFramebuffer(
						0, 0, pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(),
						0, 0, pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(),
						GL_COLOR_BUFFER_BIT,
						GL_NEAREST);
				}
				GLBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			}
		}

		BOOL IsNeedResolve(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				return pSubPass->resolveAttachments.size() ? TRUE : FALSE;
			}

			return FALSE;
		}

		void SetRenderColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass, GLuint framebuffer, eastl::vector<GLenum> &drawBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				GLuint indexAttachment = 0;

				for (const auto &itResolveAttachment : pSubPass->resolveAttachments) {
					if (itResolveAttachment.second == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL || itResolveAttachment.second == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
						if (GLuint texture = pFrameBuffer->GetRenderTexture(itResolveAttachment.first)) {
							GLenum target = pFrameBuffer->GetRenderTextureTarget(itResolveAttachment.first);
							GLenum attachment = GL_COLOR_ATTACHMENT0 + indexAttachment;

							const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(itResolveAttachment.first);
							const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(itResolveAttachment.first);

							GLBindFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);

							if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
								glClearBufferfv(GL_COLOR, indexAttachment, pClearValue->color.float32);
							}

							drawBuffers.emplace_back(attachment);
							indexAttachment++;
						}
					}
				}
			}
		}

		BOOL CheckFramebufferStatus(GLuint framebuffer) const
		{
			if (framebuffer != 0) {
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

			return TRUE;
		}


	protected:
		uint32_t m_indexPass;
		CGfxRenderPassPtr m_ptrRenderPass;
		CGfxFrameBufferPtr m_ptrFrameBuffer;
	};

}
