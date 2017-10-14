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

	class CROSS_EXPORT CGLES3Swapchain : public CGfxSwapchain
	{
		friend class CGLES3Instance;


	protected:
		CGLES3Swapchain(CGLES3Device *pDevice);
		virtual ~CGLES3Swapchain(void);


	protected:
		int Create(HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		void Destroy(void);

	protected:
		int CreateSurface(uint32_t width, uint32_t height, VkFormat format);
		int CreateProgram(VkSurfaceTransformFlagBitsKHR transform);

	protected:
		void DestroySurface(void);
		void DestroyProgram(void);

	protected:
		void RenderSurface(void) const;

	public:
		BOOL Present(void) const;
		BOOL AcquireNextImage(CGfxFence fence);

		CGfxSemaphore GetAcquireSemaphore(void) const;
		CGfxSemaphore GetRenderDoneSemaphore(void) const;

	public:
		uint32_t GetImageCount(void) const;
		uint32_t GetImageIndex(void) const;
		HANDLE GetImageHandle(int indexImage) const;

	public:
		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		VkFormat GetFormat(void) const;


	protected:
		HDC m_hDC;

	protected:
		uint32_t m_width;
		uint32_t m_height;
		VkFormat m_format;

	protected:
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_ibo;
		GLuint m_program;
		GLuint m_surface;

	protected:
		CGLES3Device *m_pDevice;
	};

}
