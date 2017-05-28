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

	int CRender::lastErrorCode = 0;

	CRender::CRender(const char *szCachePath)
		: m_szCachePath{ 0 }
	{
		ASSERT(szCachePath);
		strcpy(m_szCachePath, szCachePath);
	}

	CRender::~CRender(void)
	{

	}

	void CRender::SetLastError(int err)
	{
		lastErrorCode = err;
	}

	int CRender::GetLastError(void)
	{
		return lastErrorCode;
	}

	BOOL CRender::Create(HINSTANCE hInstance, HWND hWnd, uint32_t width, uint32_t height)
	{
		return TRUE;
	}

	void CRender::Destroy(void)
	{

	}

	const char* CRender::GetCachePath(void) const
	{
		return m_szCachePath;
	}

}
