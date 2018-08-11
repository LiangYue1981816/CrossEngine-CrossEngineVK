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

	class CROSS_EXPORT CGLES3CommandInvalidateFrameBuffer : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandInvalidateFrameBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexPass)
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

			const GLuint framebuffer = IsNeedMSAA(pFrameBuffer, pRenderPass, m_indexPass) ? (GLuint)pFrameBuffer->GetHandleMSAA() : (GLuint)pFrameBuffer->GetHandle();
			GLBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			{
				std::vector<GLenum> discardBuffers;

				GetDiscardColorTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, discardBuffers);
				GetDiscardDepthStencilTexture(pFrameBuffer, pRenderPass, m_indexPass, framebuffer, discardBuffers);

				glInvalidateFramebuffer(GL_FRAMEBUFFER, discardBuffers.size(), discardBuffers.data());
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

		void GetDiscardColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass, GLuint framebuffer, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				GLuint indexAttachment = 0;

				for (const auto &itColorAttachment : pSubPass->colorAttachments) {
					if (GLuint texture = pFrameBuffer->GetRenderTexture(itColorAttachment.first)) {
						GLenum attachment = GL_COLOR_ATTACHMENT0 + indexAttachment;
						const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(itColorAttachment.first);

						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.emplace_back(attachment);
						}

						indexAttachment++;
					}
				}
			}
		}

		void GetDiscardDepthStencilTexture(const CGLES3FrameBuffer *pFrameBuffer, const CGLES3RenderPass *pRenderPass, uint32_t indexSubPass, GLuint framebuffer, std::vector<GLenum> &discardBuffers) const
		{
			if (const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(indexSubPass)) {
				if (GLuint texture = pFrameBuffer->GetRenderTexture(pSubPass->depthStencilAttachment)) {
					GLenum format = pFrameBuffer->GetRenderTextureFormat(pSubPass->depthStencilAttachment);
					const VkAttachmentDescription *pAttachmentDescription = pRenderPass->GetAttachmentDescription(pSubPass->depthStencilAttachment);

					if (CGLES3Helper::glIsFormatDepthOnly(format)) {
						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.emplace_back(GL_DEPTH_ATTACHMENT);
						}
					}

					if (CGLES3Helper::glIsFormatStencilOnly(format)) {
						if (pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.emplace_back(GL_STENCIL_ATTACHMENT);
						}
					}

					if (CGLES3Helper::glIsFormatDepthStencil(format)) {
						if (pAttachmentDescription->storeOp == VK_ATTACHMENT_STORE_OP_DONT_CARE && pAttachmentDescription->stencilStoreOp == VK_ATTACHMENT_STORE_OP_DONT_CARE) {
							discardBuffers.emplace_back(GL_DEPTH_STENCIL_ATTACHMENT);
						}
					}
				}
			}
		}


	protected:
		uint32_t m_indexPass;
		CGfxRenderPassPtr m_ptrRenderPass;
		CGfxFrameBufferPtr m_ptrFrameBuffer;
	};

}
