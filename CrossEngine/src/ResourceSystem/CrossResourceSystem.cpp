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
		m_pResourceManager[RESOURCE_TYPE::RESOURCE_TYPE_TEXTURE] = SAFE_NEW CTextureManager;
		// ...

		event_init(&m_eventExit, 1);
		pthread_mutex_init(&m_mutexPendingList, NULL);
		pthread_mutex_init(&m_mutexPostLoadList, NULL);
	}

	CResourceSystem::~CResourceSystem(void)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			SAFE_DELETE(m_pResourceManager[indexManager]);
		}

		event_destroy(&m_eventExit);
		pthread_mutex_destroy(&m_mutexPendingList);
		pthread_mutex_destroy(&m_mutexPostLoadList);
	}

	BOOL CResourceSystem::Create(void)
	{
		pthread_create(&m_thread, NULL, UpdateLoadThread, this);
		return TRUE;
	}

	void CResourceSystem::Destroy(void)
	{
		event_signal(&m_eventExit);
		pthread_join(m_thread, NULL);
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

	BOOL CResourceSystem::RequestLoad(CResourceHandle *pResource)
	{
		mutex_autolock mutex(m_mutexPendingList);
		m_pendingList.push_back(pResource);

		return TRUE;
	}

	void CResourceSystem::UpdatePostLoad(void)
	{
		CResourceHandle *pResource = NULL;
		{
			mutex_autolock mutex(m_mutexPostLoadList);

			if (m_postLoadList.size()) {
				pResource = m_postLoadList.front();
				m_postLoadList.pop_front();
			}
		}

		if (pResource) {
			pResource->PostLoadResource();
		}
	}

	void* CResourceSystem::UpdateLoadThread(void *pParams)
	{
		CResourceSystem *pResourceSystem = (CResourceSystem *)pParams;

		while (TRUE) {
			if (event_wait_timeout(&pResourceSystem->m_eventExit, 1) == NO_ERROR) {
				break;
			}

			CResourceHandle *pResource = NULL;
			{
				mutex_autolock mutex(pResourceSystem->m_mutexPendingList);

				if (pResourceSystem->m_pendingList.size()) {
					pResource = pResourceSystem->m_pendingList.front();
					pResourceSystem->m_pendingList.pop_front();
				}
			}

			if (pResource && pResource->LoadResource()) {
				mutex_autolock mutex(pResourceSystem->m_mutexPostLoadList);
				pResourceSystem->m_postLoadList.push_back(pResource);
			}
		}

		return NULL;
	}

	void CResourceSystem::GarbageCollection(void)
	{
		for (int indexManager = 0; indexManager < RESOURCE_TYPE::COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->GarbageCollection();
			}
		}
	}

}
