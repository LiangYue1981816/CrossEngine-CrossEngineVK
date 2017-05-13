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

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const char *szName)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pPack(NULL)

		, m_bIsWaste(TRUE)
	{
		m_szName = (char *)SAFE_ALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		strcpy(m_szName, szName);
	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const char *szName, const char *szFileName)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pPack(NULL)

		, m_bIsWaste(TRUE)
	{
		m_szName = (char *)SAFE_ALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		strcpy(m_szName, szName);

		m_szFileName = (char *)SAFE_ALLOC(strlen(szFileName) + 1, MEMTYPE_HEAP);
		strcpy(m_szFileName, szFileName);
	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const char *szName, const char *szFileName, ZZIP_DIR *pPack)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pPack(pPack)

		, m_bIsWaste(TRUE)
	{
		m_szName = (char *)SAFE_ALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		strcpy(m_szName, szName);

		m_szFileName = (char *)SAFE_ALLOC(strlen(szFileName) + 1, MEMTYPE_HEAP);
		strcpy(m_szFileName, szFileName);
	}

	CResourceHandle::~CResourceHandle(void)
	{
		SAFE_FREE(m_szName);
		SAFE_FREE(m_szFileName);
		m_ptrResource.SetNull();
	}

	BOOL CResourceHandle::IsWaste(void) const
	{
		return m_bIsWaste;
	}

	const char* CResourceHandle::GetName(void) const
	{
		return m_szName;
	}

	const char* CResourceHandle::GetFileName(void) const
	{
		return m_szFileName;
	}

	CResourcePtr<CResource>& CResourceHandle::GetResourcePtr(void)
	{
		return m_ptrResource;
	}

	BOOL CResourceHandle::LoadResource(BOOL bReload)
	{
		try {
			if (m_ptrResource.IsNull() || bReload) {
				if (m_ptrResource.IsNull()) {
					m_ptrResource = CResourcePtr<CResource>(m_pResourceManager->CreateResource());
				}

				if (m_szFileName) {
					if (m_pPack) {
						if (m_ptrResource->LoadFromPack(m_pPack, m_szFileName) == FALSE) {
							throw "Load from pack failed.";
						}
					}
					else {
						if (m_ptrResource->LoadFromFile(m_szFileName) == FALSE) {
							throw "Load from file failed.";
						}
					}
				}
			}

			return m_ptrResource->IsValid();
		}
		catch (const char *szError) {
			LOGE("CResourceHandle::LoadResource(%s): %s\n", bReload ? "TRUE" : "FALSE", szError);
			FreeResource();

			return FALSE;
		}
	}

	void CResourceHandle::FreeResource(void)
	{
		m_ptrResource.SetNull();
	}

	void CResourceHandle::DeleteResource(void)
	{
		if (m_szFileName && m_pPack) {
			// ...
			return;
		}

		if (m_szFileName) {
			// ...
			return;
		}
	}


	static const CResourcePtr<CResource> ptrResourceNull;

	CResourceManager::CResourceManager(void)
	{
		pthread_mutex_init(&m_mutex, NULL);
		Init();
	}

	CResourceManager::~CResourceManager(void)
	{
		Free();
		pthread_mutex_destroy(&m_mutex);
	}

	void CResourceManager::Init(void)
	{
		pthread_mutex_lock(&m_mutex);
		{
			m_packs.clear();
			m_resources.clear();
		}
		pthread_mutex_unlock(&m_mutex);
	}

	void CResourceManager::Free(void)
	{
		pthread_mutex_lock(&m_mutex);
		{
			for (const auto &itPack : m_packs) {
				if (ZZIP_DIR *pPack = itPack.second) {
					zzip_closedir(pPack);
				}
			}

			for (const auto &itResource : m_resources) {
				if (const CResourceHandle *pResource = itResource.second) {
					if (pResource->IsWaste()) {
						LOGI("Unreference Resource: %s\n", pResource->GetFileName());
					}

					SAFE_DELETE(pResource);
				}
			}

			m_packs.clear();
			m_resources.clear();
		}
		pthread_mutex_unlock(&m_mutex);
	}

	void CResourceManager::DestroyResource(CResource *pResource)
	{
		SAFE_DELETE(pResource);
	}

	const CResourcePtr<CResource>& CResourceManager::QueryResource(DWORD dwName, BOOL bReload)
	{
		CResourceHandle *pResource = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			const auto &itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) pResource = itResource->second;
		}
		pthread_mutex_unlock(&m_mutex);

		if (pResource == NULL) {
			return ptrResourceNull;
		}

		if (pResource->LoadResource(bReload) == FALSE) {
			return ptrResourceNull;
		}

		return pResource->GetResourcePtr();
	}

	BOOL CResourceManager::FreeResource(DWORD dwName)
	{
		CResourceHandle *pResource = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			const auto &itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) pResource = itResource->second;
		}
		pthread_mutex_unlock(&m_mutex);

		if (pResource == NULL) {
			return FALSE;
		}

		pResource->FreeResource();
		return TRUE;
	}

	BOOL CResourceManager::DeleteResource(DWORD dwName)
	{
		CResourceHandle *pResource = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			const auto &itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) { pResource = itResource->second; m_resources.erase(itResource); }
		}
		pthread_mutex_unlock(&m_mutex);

		if (pResource == NULL) {
			return FALSE;
		}

		pResource->DeleteResource();
		return TRUE;
	}

	const CResourceManager::ResourceMap& CResourceManager::GetResources(void) const
	{
		return m_resources;
	}

	const CResourcePtr<CResource>& CResourceManager::CopyFrom(const char *szName, const CResource *pCopyFrom)
	{
		if (szName == NULL) {
			return ptrResourceNull;
		}

		if (pCopyFrom == NULL) {
			return ptrResourceNull;
		}

		if (pCopyFrom->IsValid() == FALSE) {
			return ptrResourceNull;
		}

		if (pCopyFrom->GetType() != GetType()) {
			return ptrResourceNull;
		}

		CResourceHandle *pResource = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			DWORD dwName = HashValue(szName);

			if (m_resources[dwName] == NULL) {
				m_resources[dwName] = SAFE_NEW CResourceHandle(this, szName);
			}

			pResource = m_resources[dwName];
		}
		pthread_mutex_unlock(&m_mutex);

		const CResourcePtr<CResource> &ptrResource = pResource->GetResourcePtr();
		ptrResource->CopyFrom(pCopyFrom);
		ptrResource->SetName(szName);

		return ptrResource;
	}

	BOOL CResourceManager::Reload(void)
	{
		pthread_mutex_lock(&m_mutex);
		{
			for (const auto &itResource : m_resources) {
				if (CResourceHandle *pResource = itResource.second) {
					pResource->LoadResource(TRUE);
				}
			}
		}
		pthread_mutex_unlock(&m_mutex);

		return TRUE;
	}

	BOOL CResourceManager::LoadFromFile(const char *szFileName, const char *szExtName)
	{
		if (szFileName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		char szFName[_MAX_STRING];
		char szEName[_MAX_STRING];

		splitfilename(szFileName, szFName, szEName);
		if (stricmp(szExtName, szEName)) return FALSE;

		CResourceHandle *pResource = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			char szName[_MAX_STRING];
			sprintf(szName, "%s%s", szFName, szEName);

			DWORD dwName = HashValue(szName);

			if (m_resources[dwName] == NULL) {
				m_resources[dwName] = pResource = SAFE_NEW CResourceHandle(this, szName, szFileName);
			}
			else {
				LOGW("Exist resource : %s\n", szFileName);
			}
		}
		pthread_mutex_unlock(&m_mutex);

		if (pResource) {
			PreLoad(pResource);
		}

		return TRUE;
	}

	BOOL CResourceManager::LoadFromPath(const char *szPathName, const char *szExtName)
	{
		if (szPathName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

#if defined _PLATFORM_WINDOWS_

		char szFilter[_MAX_STRING];
		sprintf(szFilter, "%s/*", szPathName);

#else

		char szFilter[_MAX_STRING];
		sprintf(szFilter, "%s/", szPathName);

#endif

		struct _finddata_t fileData;
		size_t hFile = (size_t)_findfirst(szFilter, &fileData);

		if (hFile != -1) {
			do {
				if (fileData.attrib&_A_SUBDIR) {
					continue;
				}

				char szFName[_MAX_STRING];
				char szEName[_MAX_STRING];

				splitfilename(fileData.name, szFName, szEName);
				if (stricmp(szExtName, szEName)) continue;

				CResourceHandle *pResource = NULL;

				pthread_mutex_lock(&m_mutex);
				{
					char szName[_MAX_STRING];
					char szFileName[_MAX_STRING];

					sprintf(szName, "%s%s", szFName, szEName);
					sprintf(szFileName, "%s/%s", szPathName, szName);

					DWORD dwName = HashValue(szName);

					if (m_resources[dwName] == NULL) {
						m_resources[dwName] = pResource = SAFE_NEW CResourceHandle(this, szName, szFileName);
					}
					else {
						LOGW("Exist resource : %s\n", szFileName);
					}
				}
				pthread_mutex_unlock(&m_mutex);

				if (pResource) {
					PreLoad(pResource);
				}	
			} while (_findnext(hFile, &fileData) == 0);

			_findclose(hFile);
		}

		return TRUE;
	}

	BOOL CResourceManager::LoadFromPack(const char *szPackName, const char *szExtName)
	{
		if (szPackName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		ZZIP_DIR *pPack = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			DWORD dwName = HashValue(szPackName);

			if (m_packs[dwName] == NULL) {
				m_packs[dwName] = zzip_opendir(szPackName);
			}

			pPack = m_packs[dwName];
		}
		pthread_mutex_unlock(&m_mutex);

		zzip_seekdir(pPack, 0);

		while (ZZIP_DIRENT *pEntry = zzip_readdir(pPack)) {
			char szFName[_MAX_STRING];
			char szEName[_MAX_STRING];

			splitfilename(pEntry->d_name, szFName, szEName);
			if (stricmp(szExtName, szEName)) continue;
			
			CResourceHandle *pResource = NULL;

			pthread_mutex_lock(&m_mutex);
			{
				char szName[_MAX_STRING];
				char szFileName[_MAX_STRING];

				sprintf(szName, "%s%s", szFName, szEName);
				sprintf(szFileName, "%s", pEntry->d_name);

				DWORD dwName = HashValue(szName);

				if (m_resources[dwName] == NULL) {
					m_resources[dwName] = pResource = SAFE_NEW CResourceHandle(this, szName, szFileName, pPack);
				}
				else {
					LOGW("Exist resource : %s\n", szFileName);
				}
			}
			pthread_mutex_unlock(&m_mutex);

			if (pResource) {
				PreLoad(pResource);
			}
		}

		return TRUE;
	}

	BOOL CResourceManager::PreLoad(CResourceHandle *pResource)
	{
		return TRUE;
	}

	void CResourceManager::GarbageCollection(void)
	{
		pthread_mutex_lock(&m_mutex);
		{
			for (const auto &itResource : m_resources) {
				if (CResourceHandle *pResource = itResource.second) {
					if (pResource->m_ptrResource.GetRefCount() == 1) {
						pResource->m_ptrResource.SetNull();
					}
				}
			}
		}
		pthread_mutex_unlock(&m_mutex);
	}

}
