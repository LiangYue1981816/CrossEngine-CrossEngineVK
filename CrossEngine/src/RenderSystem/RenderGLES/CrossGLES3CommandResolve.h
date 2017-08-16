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

	class CROSS_EXPORT CGLES3CommandResolve : public CGLES3CommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandResolve(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexPass)
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
				const CGLES3Device *pDevice = pRenderPass->GetDevice();

				if (IsNeedResolve(pFrameBuffer, pRenderPass, m_indexPass)) {
					if (IsNeedFrameBuffer(pFrameBuffer, pRenderPass, m_indexPass)) {
						const GLuint framebuffer = (GLuint)pFrameBuffer->GetHandle();
						glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
						{
							std::vector<GLenum> drawBuffers(pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS);
							std::vector<GLenum> discardBuffers(pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS + 1);

							SetRenderColorTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, drawBuffers, discardBuffers);
							SetRenderDepthStencilTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, discardBuffers);

							glReadBuffer(GL_NONE);
							glDrawBuffers(drawBuffers.size(), drawBuffers.data());
							glInvalidateFramebuffer(GL_FRAMEBUFFER, discardBuffers.size(), discardBuffers.data());

							CheckFramebufferStatus(framebuffer);
						}

						glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
					}
					else {
						glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
					}

					const GLuint framebufferMSAA = (GLuint)pFrameBuffer->GetHandleMSAA();
					glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMSAA);
					{
						glBlitFramebuffer(
							0, 0, pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(),
							0, 0, pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(),
							GL_COLOR_BUFFER_BIT,
							GL_LINEAR);
					}
					glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				}
			}
		}

		BOOL IsNeedResolve(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, int indexSubPass) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				return pSubPass->resolveAttachments.size() ? TRUE : FALSE;
			}

			return FALSE;
		}

		BOOL IsNeedFrameBuffer(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, int indexSubPass) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				for (const auto &itResolveAttachment : pSubPass->resolveAttachments) {
					if (pFrameBuffer->GetRenderTexture(itResolveAttachment.first) != 0) {
						return TRUE;
					}
				}
			}

			return FALSE;
		}

		void SetRenderColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, int indexSubPass, GLuint framebuffer, std::vector<GLenum> &drawBuffers, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				for (const auto &itResolveAttachment : pSubPass->resolveAttachments) {
					if (itResolveAttachment.second == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
						GLenum attachment = GL_COLOR_ATTACHMENT0 + itResolveAttachment.first;
						GLenum target = pFrameBuffer->GetRenderTextureTarget(itResolveAttachment.first);
						GLuint texture = pFrameBuffer->GetRenderTexture(itResolveAttachment.first);

						const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(itResolveAttachment.first);
						const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(itResolveAttachment.first);

						if (framebuffer != 0) {
							glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);
						}

						if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
							if (framebuffer != 0) {
								glClearBufferfv(GL_COLOR, itResolveAttachment.first, pClearValue->color.float32);
							}
							else {
								glClearColor(pClearValue->color.float32[0], pClearValue->color.float32[1], pClearValue->color.float32[2], pClearValue->color.float32[3]);
								glClear(GL_COLOR_BUFFER_BIT);
							}
						}

						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							if (framebuffer != 0) {
								discardBuffers[itResolveAttachment.first] = attachment;
							}
							else {
								discardBuffers[itResolveAttachment.first] = GL_COLOR;
							}
						}

						drawBuffers[itResolveAttachment.first] = attachment;
					}
				}
			}
		}

		void SetRenderDepthStencilTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, int indexSubPass, GLuint framebuffer, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				for (const auto &itResolveAttachment : pSubPass->resolveAttachments) {
					if (itResolveAttachment.second == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
						GLenum target = pFrameBuffer->GetRenderTextureTarget(itResolveAttachment.first);
						GLenum format = pFrameBuffer->GetRenderTextureFormat(itResolveAttachment.first);
						GLuint texture = pFrameBuffer->GetRenderTexture(itResolveAttachment.first);

						const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(itResolveAttachment.first);
						const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(itResolveAttachment.first);

						if (CGLES3Helper::glIsFormatDepthOnly(format)) {
							if (framebuffer != 0) {
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, texture, 0);
							}

							if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
								if (framebuffer != 0) {
									glClearBufferfv(GL_DEPTH, 0, (const GLfloat *)&pClearValue->depthStencil.depth);
								}
								else {
									glClearDepthf(pClearValue->depthStencil.depth);
									glClear(GL_DEPTH_BUFFER_BIT);
								}
							}

							if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
								if (framebuffer != 0) {
									discardBuffers[itResolveAttachment.first] = GL_DEPTH_ATTACHMENT;
								}
								else {
									discardBuffers[itResolveAttachment.first] = GL_DEPTH;
								}
							}

							return;
						}

						if (CGLES3Helper::glIsFormatStencilOnly(format)) {
							if (framebuffer != 0) {
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, target, texture, 0);
							}

							if (pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
								if (framebuffer != 0) {
									glClearBufferiv(GL_STENCIL, 0, (const GLint *)&pClearValue->depthStencil.stencil);
								}
								else {
									glClearStencil(pClearValue->depthStencil.stencil);
									glClear(GL_STENCIL_BUFFER_BIT);
								}
							}

							if (pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
								if (framebuffer != 0) {
									discardBuffers[itResolveAttachment.first] = GL_STENCIL_ATTACHMENT;
								}
								else {
									discardBuffers[itResolveAttachment.first] = GL_STENCIL;
								}
							}

							return;
						}

						if (CGLES3Helper::glIsFormatDepthStencil(format)) {
							if (framebuffer != 0) {
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, target, texture, 0);
							}

							if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR || pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
								if (framebuffer != 0) {
									glClearBufferfi(GL_DEPTH_STENCIL, 0, pClearValue->depthStencil.depth, pClearValue->depthStencil.stencil);
								}
								else {
									glClearDepthf(pClearValue->depthStencil.depth);
									glClearStencil(pClearValue->depthStencil.stencil);
									glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
								}
							}

							if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE && pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
								if (framebuffer != 0) {
									discardBuffers[itResolveAttachment.first] = GL_DEPTH_STENCIL_ATTACHMENT;
								}
								else {
									discardBuffers[itResolveAttachment.first] = GL_DEPTH_STENCIL;
								}
							}

							return;
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
		CGfxFrameBufferPtr m_ptrFrameBuffer;
		CGfxRenderPassPtr m_ptrRenderPass;
		int m_indexPass;
	};

}
