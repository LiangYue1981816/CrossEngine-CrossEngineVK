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
		, m_surface(0)
		, m_format(VK_FORMAT_UNDEFINED)
		, m_transform(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{

	}

	CGLES3Swapchain::~CGLES3Swapchain(void)
	{

	}

	int CGLES3Swapchain::Create(HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		m_hDC = hDC;
		m_transform = transform;


		return NO_ERROR;
	}

	void CGLES3Swapchain::Destroy(void)
	{
		DestroySwapchain();
	}

	int CGLES3Swapchain::CreateSwapchain(uint32_t width, uint32_t height, VkFormat format)
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

	void CGLES3Swapchain::DestroySwapchain(void)
	{
		glDeleteTextures(1, &m_surface);
		m_surface = 0;
	}

	BOOL CGLES3Swapchain::Present(void) const
	{
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
