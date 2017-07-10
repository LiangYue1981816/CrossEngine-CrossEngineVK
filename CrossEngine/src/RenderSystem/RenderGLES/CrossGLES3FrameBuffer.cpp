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

	CGLES3FrameBuffer::CGLES3FrameBuffer(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxFrameBuffer(pResourceManager)
		, m_pDevice(pDevice)
		, m_framebuffer(0)

		, m_width(0)
		, m_height(0)
	{

	}

	CGLES3FrameBuffer::~CGLES3FrameBuffer(void)
	{

	}

	HANDLE CGLES3FrameBuffer::GetHandle(void) const
	{
		return (HANDLE)m_framebuffer;
	}

	static BOOL CheckFramebufferStatus(GLuint fbo)
	{
		GLenum status;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		{
			status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return status == GL_FRAMEBUFFER_COMPLETE ? TRUE : FALSE;
	}

	BOOL CGLES3FrameBuffer::Create(HANDLE hRenderPass)
	{
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		{
			for (const auto &itAttachment : m_attachments) {
				if (CGLES3Helper::glIsFormatDepthOnly(itAttachment.second.format)) {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, itAttachment.second.texture, 0);
					continue;
				}

				if (CGLES3Helper::glIsFormatStencilOnly(itAttachment.second.format)) {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, itAttachment.second.texture, 0);
					continue;
				}

				if (CGLES3Helper::glIsFormatDepthStencil(itAttachment.second.format)) {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, itAttachment.second.texture, 0);
					continue;
				}

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itAttachment.first, GL_TEXTURE_2D, itAttachment.second.texture, 0);
			}

			glReadBuffer(GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return CheckFramebufferStatus(m_framebuffer);
	}

	void CGLES3FrameBuffer::Destroy(void)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}

	BOOL CGLES3FrameBuffer::SetAttachment(uint32_t indexAttachment, GLenum format, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		if (indexAttachment >= m_pDevice->GetPhysicalDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		if (m_width == 0) m_width = width;
		if (m_height == 0) m_height = height;
		if (m_width != width || m_height != height) return FALSE;

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].texture = (GLuint)hImageView;

		return TRUE;
	}

	BOOL CGLES3FrameBuffer::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		GLenum type;
		GLenum internalFormat;
		GLenum externalFormat;
		CGLES3Helper::glTranslateFormat(format, internalFormat, externalFormat, type);

		m_attachments[indexAttachment].ptrRenderTexture.Release();
		return SetAttachment(indexAttachment, externalFormat, width, height, hImageView);
	}

	BOOL CGLES3FrameBuffer::SetColorAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
		return SetAttachment(indexAttachment, ((CGLES3RenderTexture *)((CGfxRenderTexture *)ptrRenderTexture))->GetFormat(), ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle());
	}

	BOOL CGLES3FrameBuffer::SetDepthStencilAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
		return SetAttachment(indexAttachment, ((CGLES3RenderTexture *)((CGfxRenderTexture *)ptrRenderTexture))->GetFormat(), ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle());
	}

	uint32_t CGLES3FrameBuffer::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3FrameBuffer::GetHeight(void) const
	{
		return m_height;
	}

	GLuint CGLES3FrameBuffer::GetRenderTexture(uint32_t indexAttachment)
	{
		return m_attachments[indexAttachment].texture;
	}

	void CGLES3FrameBuffer::DumpLog(void) const
	{
		if (m_framebuffer) {
			LOGI("\t\tFrameBuffer 0x%x: width = %d height = %d\n", m_framebuffer, m_width, m_height);
			for (const auto &itAttachment : m_attachments) {
				LOGI("\t\t\tAttachment %d: texture = 0x%x format = %s\n", itAttachment.first, itAttachment.second.texture, CGLES3Helper::glEnumToString(itAttachment.second.format));
			}
		}
	}

}
