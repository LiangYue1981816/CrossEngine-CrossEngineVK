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

	BOOL CGLES3FrameBuffer::Create(HANDLE hRenderPass)
	{
		glGenFramebuffers(1, &m_framebuffer);
		return TRUE;
	}

	void CGLES3FrameBuffer::Destroy(void)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}

	BOOL CGLES3FrameBuffer::SetAttachment(uint32_t indexAttachment, GLenum format, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		if (indexAttachment >= (uint32_t)m_pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS) {
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

	GLuint CGLES3FrameBuffer::GetRenderTexture(uint32_t indexAttachment) const
	{
		const auto &itAttachment = m_attachments.find(indexAttachment);
		return itAttachment != m_attachments.end() ? itAttachment->second.texture : 0;
	}

	GLenum CGLES3FrameBuffer::GetRenderTextureFormat(uint32_t indexAttachment) const
	{
		const auto &itAttachment = m_attachments.find(indexAttachment);
		return itAttachment != m_attachments.end() ? itAttachment->second.format : GL_INVALID_ENUM;
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
