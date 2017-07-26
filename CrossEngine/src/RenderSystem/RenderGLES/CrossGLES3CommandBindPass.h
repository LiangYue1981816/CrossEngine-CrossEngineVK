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
		CGLES3CommandBindPass(CGfxFrameBufferPtr &ptrFrameBuffer, CGfxRenderPassPtr &ptrRenderPass, int indexPass)
			: m_indexPass(indexPass)
		{
			m_ptrFrameBuffer = ptrFrameBuffer;
			m_ptrRenderPass = ptrRenderPass;
		}
		virtual ~CGLES3CommandBindPass(void)
		{
			m_ptrFrameBuffer.Release();
			m_ptrRenderPass.Release();
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

				glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)pFrameBuffer->GetHandle());
				glReadBuffer(GL_NONE);

				SetRenderColorTexture(pFrameBuffer, pPass);
				SetRenderDepthStencilTexture(pFrameBuffer, pPass);
				CheckFramebufferStatus();
			}
		}

		void SetRenderColorTexture(const CGLES3FrameBuffer *pFrameBuffer, const GLSubpassInformation* pPass) const
		{
			for (const auto &itColorAttachment : pPass->colorAttachments) {
				GLuint texture = pFrameBuffer->GetRenderTexture(itColorAttachment.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itColorAttachment.first, GL_TEXTURE_2D, texture, 0);
			}
		}

		void SetRenderDepthStencilTexture(const CGLES3FrameBuffer *pFrameBuffer, const GLSubpassInformation* pPass) const
		{
			GLuint texture = pFrameBuffer->GetRenderTexture(pPass->depthStencilAttachment);
			GLenum format = pFrameBuffer->GetRenderTextureFormat(pPass->depthStencilAttachment);

			if (CGLES3Helper::glIsFormatDepthOnly(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
				return;
			}

			if (CGLES3Helper::glIsFormatStencilOnly(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
				return;
			}

			if (CGLES3Helper::glIsFormatDepthStencil(format)) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
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
