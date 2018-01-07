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

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager)
		: m_pResourceManager(pResourceManager)

		, m_pPack(NULL)
		, m_szFileName{ 0 }

		, m_bIsWaste(TRUE)
	{

	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const char *szFileName)
		: m_pResourceManager(pResourceManager)

		, m_pPack(NULL)
		, m_szFileName{ 0 }

		, m_bIsWaste(TRUE)
	{
		strcpy(m_szFileName, szFileName);
	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const char *szFileName, ZZIP_DIR *pPack)
		: m_pResourceManager(pResourceManager)

		, m_pPack(NULL)
		, m_szFileName{ 0 }

		, m_bIsWaste(TRUE)
	{
		m_pPack = pPack;
		strcpy(m_szFileName, szFileName);
	}

	CResourceHandle::~CResourceHandle(void)
	{

	}

	BOOL CResourceHandle::IsWaste(void) const
	{
		return m_bIsWaste;
	}

	const char* CResourceHandle::GetFileName(void) const
	{
		return m_szFileName;
	}

	CResourcePtr<CResource>& CResourceHandle::GetResourcePtr(void)
	{
		return m_ptrResource;
	}

	BOOL CResourceHandle::LoadResource(BOOL bSyncLoad, BOOL bSyncPostLoad)
	{
		m_bIsWaste = FALSE;

		if (m_ptrResource.IsNull()) {
			m_ptrResource = CResourcePtr<CResource>(m_pResourceManager->CreateResource());

			if (bSyncLoad) {
				if (Load(bSyncPostLoad) == FALSE) {
					return FALSE;
				}

				if (bSyncPostLoad) {
					return PostLoad();
				}
				else {
					return ResourceSystem()->RequestPostLoad(this);
				}
			}
			else {
				return ResourceSystem()->RequestLoad(this);
			}
		}

		return TRUE;
	}

	BOOL CResourceHandle::Load(BOOL bSyncPostLoad)
	{
		if (m_pPack) {
			return m_ptrResource->LoadFromPack(m_pPack, m_szFileName, bSyncPostLoad);
		}
		else {
			return m_ptrResource->LoadFromFile(m_szFileName, bSyncPostLoad);
		}
	}

	BOOL CResourceHandle::PostLoad(void)
	{
		return m_ptrResource->PostLoad();
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
		mutex_autolock mutex(m_mutex);

		m_pPacks.clear();
		m_pResources.clear();
	}

	void CResourceManager::Free(void)
	{
		mutex_autolock mutex(m_mutex);

		for (auto &itPack : m_pPacks) {
			zzip_closedir(itPack.second);
		}

		for (auto &itResource : m_pResources) {
			if (itResource.second->IsWaste()) {
				LOGW("Waste Resource: %s\n", itResource.second->GetFileName());
			}

			SAFE_DELETE(itResource.second);
		}

		m_pPacks.clear();
		m_pResources.clear();
	}

	void CResourceManager::DestroyResource(CResource *pResource)
	{
		SAFE_DELETE(pResource);
	}

	const CResourcePtr<CResource>& CResourceManager::LoadResource(const char *szName, BOOL bSyncLoad, BOOL bSyncPostLoad)
	{
		return LoadResource(HashValue(szName), bSyncLoad, bSyncPostLoad);
	}

	const CResourcePtr<CResource>& CResourceManager::LoadResource(uint32_t dwName, BOOL bSyncLoad, BOOL bSyncPostLoad)
	{
		CResourceHandle *pResourceHandle = NULL;
		{
			mutex_autolock mutex(m_mutex);
			const auto &itResource = m_pResources.find(dwName);
			if (itResource != m_pResources.end()) pResourceHandle = itResource->second;
		}

		if (pResourceHandle == NULL) {
			return ptrResourceNull;
		}

		if (pResourceHandle->LoadResource(bSyncLoad, bSyncPostLoad) == FALSE) {
			return ptrResourceNull;
		}

		return pResourceHandle->GetResourcePtr();
	}

	BOOL CResourceManager::PreLoadFromFile(const char *szFileName, const char *szExtName)
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

		char szName[_MAX_STRING];
		sprintf(szName, "%s%s", szFName, szEName);

		uint32_t dwName = HashValue(szName);
		CResourceHandle *pResourceHandle = NULL;
		{
			mutex_autolock mutex(m_mutex);

			if (m_pResources[dwName] == NULL) {
				m_pResources[dwName] = pResourceHandle = SAFE_NEW CResourceHandle(this, szFileName);
			}
			else {
				LOGW("Exist resource : %s\n", szFileName);
			}
		}

		if (pResourceHandle) {
			PreLoad(pResourceHandle);
		}

		return TRUE;
	}

	BOOL CResourceManager::PreLoadFromPath(const char *szPathName, const char *szExtName)
	{
		if (szPathName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

#ifdef PLATFORM_WINDOWS

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
					if (!stricmp(fileData.name, ".")) continue;
					if (!stricmp(fileData.name, "..")) continue;

					char szSubPathName[_MAX_STRING];
					sprintf(szSubPathName, "%s/%s", szPathName, fileData.name);
					PreLoadFromPath(szSubPathName, szExtName);

					continue;
				}

				char szFName[_MAX_STRING];
				char szEName[_MAX_STRING];
				splitfilename(fileData.name, szFName, szEName);
				if (stricmp(szExtName, szEName)) continue;

				char szName[_MAX_STRING];
				char szFileName[_MAX_STRING];
				sprintf(szName, "%s%s", szFName, szEName);
				sprintf(szFileName, "%s/%s", szPathName, szName);

				uint32_t dwName = HashValue(szName);
				CResourceHandle *pResourceHandle = NULL;
				{
					mutex_autolock mutex(m_mutex);

					if (m_pResources[dwName] == NULL) {
						m_pResources[dwName] = pResourceHandle = SAFE_NEW CResourceHandle(this, szFileName);
					}
					else {
						LOGW("Exist resource : %s\n", szFileName);
					}
				}

				if (pResourceHandle) {
					PreLoad(pResourceHandle);
				}
			} while (_findnext(hFile, &fileData) == 0);

			_findclose(hFile);
		}

		return TRUE;
	}

	BOOL CResourceManager::PreLoadFromPack(const char *szPackName, const char *szExtName)
	{
		if (szPackName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		uint32_t dwName = HashValue(szPackName);
		ZZIP_DIR *pPack = NULL;
		{
			mutex_autolock mutex(m_mutex);

			if (m_pPacks[dwName] == NULL) {
				m_pPacks[dwName] = zzip_opendir(szPackName);
			}

			pPack = m_pPacks[dwName];
		}

		zzip_seekdir(pPack, 0);

		while (ZZIP_DIRENT *pEntry = zzip_readdir(pPack)) {
			char szFName[_MAX_STRING];
			char szEName[_MAX_STRING];
			splitfilename(pEntry->d_name, szFName, szEName);
			if (stricmp(szExtName, szEName)) continue;

			char szName[_MAX_STRING];
			char szFileName[_MAX_STRING];
			sprintf(szName, "%s%s", szFName, szEName);
			sprintf(szFileName, "%s", pEntry->d_name);

			uint32_t dwName = HashValue(szName);
			CResourceHandle *pResourceHandle = NULL;
			{
				mutex_autolock mutex(m_mutex);

				if (m_pResources[dwName] == NULL) {
					m_pResources[dwName] = pResourceHandle = SAFE_NEW CResourceHandle(this, szFileName, pPack);
				}
				else {
					LOGW("Exist resource : %s\n", szFileName);
				}
			}

			if (pResourceHandle) {
				PreLoad(pResourceHandle);
			}
		}

		return TRUE;
	}

	BOOL CResourceManager::PreLoad(CResourceHandle *pResourceHandle)
	{
		return TRUE;
	}

	void CResourceManager::GarbageCollection(void)
	{
		mutex_autolock mutex(m_mutex);

		for (auto &itResource : m_pResources) {
			if (itResource.second->m_ptrResource.IsNull() == FALSE && itResource.second->m_ptrResource.GetRefCount() == 1) {
				itResource.second->m_ptrResource.Release();
			}
		}
	}

}
