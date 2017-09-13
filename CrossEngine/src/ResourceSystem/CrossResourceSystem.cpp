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

	CResourceSystem::CResourceSystem(void)
		: m_pResourceManager{ NULL }
	{
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_SHADER] = SAFE_NEW CShaderManager;
	}

	CResourceSystem::~CResourceSystem(void)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			SAFE_DELETE(m_pResourceManager[indexManager]);
		}
	}

	BOOL CResourceSystem::Create(void)
	{
		return TRUE;
	}

	void CResourceSystem::Destroy(void)
	{

	}

	CResourceManager* CResourceSystem::GetResourceManager(RESOURCE_TYPE type) const
	{
		return m_pResourceManager[type];
	}

	BOOL CResourceSystem::PreLoadResourcePath(const char *szPathName)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			m_pResourceManager[indexManager]->PreLoadFromPath(szPathName);
		}

		return TRUE;
	}

	BOOL CResourceSystem::PreLoadResourcePack(const char *szPackName)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			m_pResourceManager[indexManager]->PreLoadFromPack(szPackName);
		}

		return TRUE;
	}

	void CResourceSystem::GarbageCollection(void)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			m_pResourceManager[indexManager]->GarbageCollection();
		}
	}

}
