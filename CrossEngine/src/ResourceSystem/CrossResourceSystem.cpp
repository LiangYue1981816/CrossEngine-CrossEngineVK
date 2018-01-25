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
		event_init(&m_eventExit, 0);
		pthread_mutex_init(&m_mutexPendingList, NULL);
		pthread_mutex_init(&m_mutexPostLoadList, NULL);
	}

	CResourceSystem::~CResourceSystem(void)
	{
		event_destroy(&m_eventExit);
		pthread_mutex_destroy(&m_mutexPendingList);
		pthread_mutex_destroy(&m_mutexPostLoadList);
	}

	BOOL CResourceSystem::Create(void)
	{
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_RENDERPASS] = SAFE_NEW CResRenderPassManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_SHADER] = SAFE_NEW CResShaderManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_TEXTURE] = SAFE_NEW CResTextureManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_RENDER_TEXTURE] = SAFE_NEW CResRenderTextureManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_COMPUTE] = SAFE_NEW CResComputeManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_GRAPHICS] = SAFE_NEW CResGraphicsManager;
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_MATERIAL] = SAFE_NEW CResMaterialManager;
		// ...

		pthread_create(&m_thread, NULL, WorkThread, this);

		return TRUE;
	}

	void CResourceSystem::Destroy(void)
	{
		event_signal(&m_eventExit);
		pthread_join(m_thread, NULL);

		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_RENDERPASS]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_SHADER]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_TEXTURE]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_RENDER_TEXTURE]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_COMPUTE]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_GRAPHICS]);
		SAFE_DELETE(m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_MATERIAL]);
		// ...
	}

	CResourceManager* CResourceSystem::GetResourceManager(RESOURCE_TYPE type) const
	{
		return m_pResourceManager[type];
	}

	BOOL CResourceSystem::PreLoadResourcePath(const char *szPathName)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->PreLoadFromPath(szPathName);
			}
		}

		return TRUE;
	}

	BOOL CResourceSystem::PreLoadResourcePack(const char *szPackName)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->PreLoadFromPack(szPackName);
			}
		}

		return TRUE;
	}

	BOOL CResourceSystem::RequestLoad(CResourceHandle *pResourceHandle)
	{
		mutex_autolock mutex(&m_mutexPendingList);
		m_pPendingList.push_back(pResourceHandle);

		return TRUE;
	}

	BOOL CResourceSystem::RequestPostLoad(CResourceHandle *pResourceHandle)
	{
		mutex_autolock mutex(&m_mutexPostLoadList);
		m_pPostLoadList.push_back(pResourceHandle);

		return TRUE;
	}

	void CResourceSystem::Load(void)
	{
		CResourceHandle *pResourceHandle = NULL;
		{
			mutex_autolock mutex(&m_mutexPendingList);

			if (m_pPendingList.size()) {
				pResourceHandle = m_pPendingList.front();
				m_pPendingList.pop_front();
			}
		}

		if (pResourceHandle && pResourceHandle->Load(FALSE)) {
			RequestPostLoad(pResourceHandle);
		}
	}

	void CResourceSystem::PostLoad(void)
	{
		mutex_autolock mutex(&m_mutexPostLoadList);

		while (m_pPostLoadList.size()) {
			if (CResourceHandle *pResourceHandle = m_pPostLoadList.front()) {
				pResourceHandle->PostLoad();
			}

			m_pPostLoadList.pop_front();
		}
	}

	void CResourceSystem::GarbageCollection(void)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->GarbageCollection();
			}
		}
	}

	void* CResourceSystem::WorkThread(void *pParams)
	{
		if (CResourceSystem *pResourceSystem = (CResourceSystem *)pParams) {
			while (TRUE) {
				if (event_wait_timeout(&pResourceSystem->m_eventExit, 1) == NO_ERROR) {
					break;
				}

				pResourceSystem->Load();
			}
		}

		return NULL;
	}

}
