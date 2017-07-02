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
		return TRUE;
	}

	void CGLES3FrameBuffer::Destroy(void)
	{

	}

	BOOL CGLES3FrameBuffer::SetAttachment(uint32_t indexAttachment, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		if (indexAttachment >= m_pDevice->GetPhysicalDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		if (m_width == 0) m_width = width;
		if (m_height == 0) m_height = height;
		if (m_width != width || m_height != height) return FALSE;

		m_attachments[indexAttachment].texture = (GLuint)hImageView;

		return TRUE;
	}

	BOOL CGLES3FrameBuffer::SetPresentAttachment(uint32_t indexAttachment, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		m_attachments[indexAttachment].ptrRenderTexture.Release();
		return SetAttachment(indexAttachment, width, height, hImageView);
	}

	BOOL CGLES3FrameBuffer::SetColorAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
		return SetAttachment(indexAttachment, ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle());
	}

	BOOL CGLES3FrameBuffer::SetDepthStencilAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
		return SetAttachment(indexAttachment, ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle());
	}

	uint32_t CGLES3FrameBuffer::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3FrameBuffer::GetHeight(void) const
	{
		return m_height;
	}

	void CGLES3FrameBuffer::DumpLog(void) const
	{
		if (m_framebuffer) {
			LOGI("\t\tFrameBuffer 0x%x: width = %d height = %d\n", m_framebuffer, m_width, m_height);
			for (const auto &itAttachment : m_attachments) {
				if (const GLuint texture = itAttachment.second.texture) {
					LOGI("\t\t\tAttachment %d: texture = 0x%x\n", itAttachment.first, texture);
				}
			}
		}
	}

}
