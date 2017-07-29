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

	class CROSS_EXPORT CGLES3Instance : public CGfxInstance
	{
	public:
		CGLES3Instance(const char *szCachePath);
		virtual ~CGLES3Instance(void);


	public:
		static int GetLastError(void);

	public:
		BOOL Create(HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height);
		void Destroy(void);

	protected:
		int CreateDevice(void);
		int CreateSwapchain(HDC hDC, uint32_t width, uint32_t height);

	protected:
		void DestroyDevice(void);
		void DestroySwapchain(void);

	public:
		CGfxDevice* GetDevice(void) const;
		CGfxSwapchain* GetSwapchain(void) const;


	protected:
		CGLES3Device *m_pDevice;
		CGLES3Swapchain *m_pSwapchain;
	};

}
