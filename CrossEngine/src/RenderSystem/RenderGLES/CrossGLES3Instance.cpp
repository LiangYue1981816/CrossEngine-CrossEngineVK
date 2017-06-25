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
#include "glew.h"


namespace CrossEngine {

	CGLES3Instance::CGLES3Instance(void)
	{

	}

	CGLES3Instance::~CGLES3Instance(void)
	{

	}

	BOOL CGLES3Instance::Create(HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height)
	{
#ifdef PLATFORM_WINDOWS
		int pixelFormat;

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
			1,                              // version number 
			PFD_DRAW_TO_WINDOW |            // support window 
			PFD_SUPPORT_OPENGL |            // support OpenGL 
			PFD_DOUBLEBUFFER,               // double buffered 
			PFD_TYPE_RGBA,                  // RGBA type 
			32,                             // 32-bit color depth 
			0, 0, 0, 0, 0, 0,               // color bits ignored 
			0,                              // no alpha buffer 
			0,                              // shift bit ignored 
			0,                              // no accumulation buffer 
			0, 0, 0, 0,                     // accum bits ignored 
			24,                             // 24-bit z-buffer 
			8,                              // 8-bit stencil buffer 
			0,                              // no auxiliary buffer 
			PFD_MAIN_PLANE,                 // main layer 
			0,                              // reserved 
			0, 0, 0                         // layer masks ignored 
		};

		pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDescriptor);
		SetPixelFormat(hDC, pixelFormat, &pixelFormatDescriptor);

		HGLRC hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		glewInit();
#endif

		return TRUE;
	}

	void CGLES3Instance::Destroy(void)
	{

	}

	CGfxDevice* CGLES3Instance::GetDevice(void) const
	{
		return NULL;
	}

	CGfxSwapchain* CGLES3Instance::GetSwapchain(void) const
	{
		return NULL;
	}

}
