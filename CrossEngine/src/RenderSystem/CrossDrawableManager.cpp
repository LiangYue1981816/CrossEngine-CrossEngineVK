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

	CDrawableManager::CDrawableManager(void)
	{

	}

	CDrawableManager::~CDrawableManager(void)
	{
		FreeDrawableAll();
	}

	CDrawable* CDrawableManager::AllocDrawable(DRAWABLE_TYPE type)
	{
		CDrawable *pDrawable = NULL;

		switch (type) {
		case DRAWABLE_TYPE_PARTICAL:
			break;

		case DRAWABLE_TYPE_SKIN_MESH:
			break;

		case DRAWABLE_TYPE_STATIC_MESH:
			pDrawable = SAFE_NEW CDrawableStaticMesh;
			m_pDrawables[pDrawable] = pDrawable;
			break;
		}

		return pDrawable;
	}

	void CDrawableManager::FreeDrawable(CDrawable *pDrawable)
	{
		if (pDrawable) {
			m_pDrawables.erase(pDrawable);
			SAFE_DELETE(pDrawable);
		}
	}

	void CDrawableManager::FreeDrawableAll(void)
	{
		for (auto &itDrawable : m_pDrawables) {
			SAFE_DELETE(itDrawable.second);
		}

		m_pDrawables.clear();
	}

}
