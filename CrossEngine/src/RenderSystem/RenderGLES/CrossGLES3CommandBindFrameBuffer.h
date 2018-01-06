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

	class CROSS_EXPORT CGLES3CommandBindFrameBuffer : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindFrameBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexPass)
			: m_indexPass(indexPass)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
		}


	protected:
		virtual void Execute(void) const
		{
			const CGLES3FrameBuffer *pFrameBuffer = (CGLES3FrameBuffer *)((CGfxFrameBuffer *)m_ptrFrameBuffer);
			const CGLES3RenderPass *pRenderPass = (CGLES3RenderPass *)((CGfxRenderPass *)m_ptrRenderPass);

			const GLuint framebuffer = IsNeedMSAA(pFrameBuffer, pRenderPass, m_indexPass) ? (GLuint)pFrameBuffer->GetHandleMSAA() : (GLuint)pFrameBuffer->GetHandle();
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			{
				std::vector<GLenum> drawBuffers;
				std::vector<GLenum> discardBuffers;

				SetRenderColorTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, drawBuffers, discardBuffers);
				SetRenderDepthStencilTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, discardBuffers);

				glReadBuffers(drawBuffers.size(), drawBuffers.data());
				glDrawBuffers(drawBuffers.size(), drawBuffers.data());
				glInvalidateFramebuffer(GL_FRAMEBUFFER, discardBuffers.size(), discardBuffers.data());

				CheckFramebufferStatus(framebuffer);
			}
		}

		BOOL IsNeedMSAA(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				for (const auto &itColorAttachment : pSubPass->colorAttachments) {
					if (pFrameBuffer->GetRenderTextureTarget(itColorAttachment.first) == GL_TEXTURE_2D_MULTISAMPLE) {
						return TRUE;
					}
				}

				if (pFrameBuffer->GetRenderTextureTarget(pSubPass->depthStencilAttachment) == GL_TEXTURE_2D_MULTISAMPLE) {
					return TRUE;
				}
			}

			return FALSE;
		}

		void SetRenderColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass, GLuint framebuffer, std::vector<GLenum> &drawBuffers, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				for (const auto &itColorAttachment : pSubPass->colorAttachments) {
					if (GLuint texture = pFrameBuffer->GetRenderTexture(itColorAttachment.first)) {
						GLuint indexAttachment = drawBuffers.size();
						GLenum attachment = GL_COLOR_ATTACHMENT0 + indexAttachment;
						GLenum target = pFrameBuffer->GetRenderTextureTarget(itColorAttachment.first);

						const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(itColorAttachment.first);
						const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(itColorAttachment.first);

						glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);

						if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
							glClearBufferfv(GL_COLOR, indexAttachment, pClearValue->color.float32);
						}

						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.push_back(attachment);
						}

						drawBuffers.push_back(attachment);
					}
				}
			}
		}

		void SetRenderDepthStencilTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass, GLuint framebuffer, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				if (GLuint texture = pFrameBuffer->GetRenderTexture(pSubPass->depthStencilAttachment)) {
					GLenum target = pFrameBuffer->GetRenderTextureTarget(pSubPass->depthStencilAttachment);
					GLenum format = pFrameBuffer->GetRenderTextureFormat(pSubPass->depthStencilAttachment);

					const VkClearValue *pClearValue = pRenderPass->GetAttachmentClearValue(pSubPass->depthStencilAttachment);
					const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(pSubPass->depthStencilAttachment);

					if (CGLES3Helper::glIsFormatDepthOnly(format)) {
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, texture, 0);

						if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
							glClearBufferfv(GL_DEPTH, 0, (const GLfloat *)&pClearValue->depthStencil.depth);
						}

						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.push_back(GL_DEPTH_ATTACHMENT);
						}
					}

					if (CGLES3Helper::glIsFormatStencilOnly(format)) {
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, target, texture, 0);

						if (pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
							glClearBufferiv(GL_STENCIL, 0, (const GLint *)&pClearValue->depthStencil.stencil);
						}

						if (pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.push_back(GL_STENCIL_ATTACHMENT);
						}
					}

					if (CGLES3Helper::glIsFormatDepthStencil(format)) {
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, target, texture, 0);

						if (pAttachmentDescription->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR || pAttachmentDescription->stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) {
							glClearBufferfi(GL_DEPTH_STENCIL, 0, pClearValue->depthStencil.depth, pClearValue->depthStencil.stencil);
						}

						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE && pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
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
		uint32_t m_indexPass;
	};

}
