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

	CEngine  CEngine::sInstance;
	CEngine* CEngine::GetEngine(void)
	{
		return &sInstance;
	}

	CEngine::CEngine(void)
		: m_pRenderSystem(NULL)
		, m_pResourceSystem(NULL)
	{
		m_pRenderSystem = SAFE_NEW CRenderSystem;
		m_pResourceSystem = SAFE_NEW CResourceSystem;
	}

	CEngine::~CEngine(void)
	{
		SAFE_DELETE(m_pRenderSystem);
		SAFE_DELETE(m_pResourceSystem);
	}

	CRenderSystem* CEngine::GetRenderSystem(void) const
	{
		return m_pRenderSystem;
	}

	CResourceSystem* CEngine::GetResourceSystem(void) const
	{
		return m_pResourceSystem;
	}

	BOOL CEngine::Init(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, RECT rcView, DWORD dwSoundMemPoolSize, INT maxChannels, const CHAR *szLogFileName, BOOL bEditorMode)
	{
		if (m_pRenderSystem->Create(api, hInstance, hWnd, hDC, rcView.right - rcView.left, rcView.bottom - rcView.top) == FALSE) return FALSE;
		// ...

		return TRUE;
	}

	void CEngine::Exit(void)
	{
		m_pRenderSystem->Destroy();
	}

}
