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

	CGLES3Swapchain::CGLES3Swapchain(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_hDC(NULL)

		, m_width(0)
		, m_height(0)
		, m_format(VK_FORMAT_UNDEFINED)
		
		, m_fbo(0)
		, m_surface(0)
	{

	}

	CGLES3Swapchain::~CGLES3Swapchain(void)
	{

	}

	int CGLES3Swapchain::Create(HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		m_hDC = hDC;

		CALL_GL_FUNCTION_RETURN(CreateSurface(width, height, VK_FORMAT_B8G8R8A8_UNORM));
		CALL_GL_FUNCTION_RETURN(CreateFrameBuffer(transform));

		return NO_ERROR;
	}

	void CGLES3Swapchain::Destroy(void)
	{
		DestroySurface();
		DestroyFrameBuffer();
	}

	int CGLES3Swapchain::CreateSurface(uint32_t width, uint32_t height, VkFormat format)
	{
		m_width = width;
		m_height = height;
		m_format = format;

		GLenum type;
		GLenum internalFormat;
		GLenum externalFormat;
		CGLES3Helper::glTranslateFormat(format, internalFormat, externalFormat, type);

		glGenTextures(1, &m_surface);
		glBindTexture(GL_TEXTURE_2D, m_surface);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, type, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		return NO_ERROR;
	}

	int CGLES3Swapchain::CreateFrameBuffer(VkSurfaceTransformFlagBitsKHR transform)
	{
		GLenum attachment = GL_COLOR_ATTACHMENT0;

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_surface, 0);

		glReadBuffers(1, &attachment);
		glDrawBuffers(1, &attachment);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE ? NO_ERROR : -1;
	}

	void CGLES3Swapchain::DestroySurface(void)
	{
		glDeleteTextures(1, &m_surface);
		m_surface = 0;
	}

	void CGLES3Swapchain::DestroyFrameBuffer(void)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}

	void CGLES3Swapchain::RenderSurface(void) const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		{
			glBlitFramebuffer(
				0, 0, m_width, m_height, 
				0, 0, m_width, m_height, 
				GL_COLOR_BUFFER_BIT, 
				GL_NEAREST);
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	BOOL CGLES3Swapchain::Present(void) const
	{
		RenderSurface();

#ifdef PLATFORM_WINDOWS

		::SwapBuffers(m_hDC);
#endif

		return TRUE;
	}

	BOOL CGLES3Swapchain::AcquireNextImage(CGfxFence fence)
	{
		return TRUE;
	}

	CGfxSemaphore CGLES3Swapchain::GetAcquireSemaphore(void) const
	{
		return NULL;
	}

	CGfxSemaphore CGLES3Swapchain::GetRenderDoneSemaphore(void) const
	{
		return NULL;
	}

	uint32_t CGLES3Swapchain::GetImageCount(void) const
	{
		return 1;
	}

	uint32_t CGLES3Swapchain::GetImageIndex(void) const
	{
		return 0;
	}

	HANDLE CGLES3Swapchain::GetImageHandle(int indexImage) const
	{
		return (HANDLE)m_surface;
	}

	uint32_t CGLES3Swapchain::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3Swapchain::GetHeight(void) const
	{
		return m_height;
	}

	VkFormat CGLES3Swapchain::GetFormat(void) const
	{
		return m_format;
	}

}
