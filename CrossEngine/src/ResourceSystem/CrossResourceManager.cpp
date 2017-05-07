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
/*
#include "_CrossEngine.h"


namespace CrossEngine {

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pZipPack(NULL)

		, m_bIsWaste(TRUE)
	{
		m_szName = (CHAR *)SAFE_MALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		ASSERT(m_szName);
		strcpy(m_szName, szName);
	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pZipPack(NULL)

		, m_bIsWaste(TRUE)
	{
		m_szName = (CHAR *)SAFE_MALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		ASSERT(m_szName);
		strcpy(m_szName, szName);

		m_szFileName = (CHAR *)SAFE_MALLOC(strlen(szFileName) + 1, MEMTYPE_HEAP);
		ASSERT(m_szFileName);
		strcpy(m_szFileName, szFileName);
	}

	CResourceHandle::CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName, ZZIP_DIR *pZipPack)
		: m_pResourceManager(pResourceManager)

		, m_szName(NULL)
		, m_szFileName(NULL)
		, m_pZipPack(NULL)

		, m_bIsWaste(TRUE)
	{
		m_szName = (CHAR *)SAFE_MALLOC(strlen(szName) + 1, MEMTYPE_HEAP);
		ASSERT(m_szName);
		strcpy(m_szName, szName);

		m_szFileName = (CHAR *)SAFE_MALLOC(strlen(szFileName) + 1, MEMTYPE_HEAP);
		ASSERT(m_szFileName);
		strcpy(m_szFileName, szFileName);

		m_pZipPack = pZipPack;
	}

	CResourceHandle::~CResourceHandle(VOID)
	{
		SAFE_FREE(m_szName);
		SAFE_FREE(m_szFileName);

		m_ptrResource.SetNull();
	}

	//
	// 垃圾资源
	//
	BOOL CResourceHandle::IsWaste(VOID) const
	{
		return m_bIsWaste;
	}

	//
	// 获得资源名
	//
	const CHAR* CResourceHandle::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// 获得资源文件名
	//
	const CHAR* CResourceHandle::GetFileName(VOID) const
	{
		return m_szFileName;
	}

	//
	// 获得资源指针
	//
	CResourcePtr& CResourceHandle::GetResourcePtr(VOID)
	{
		return m_ptrResource;
	}

	//
	// 加载资源
	//
	BOOL CResourceHandle::LoadResource(BOOL bReload)
	{
		try {
			m_bIsWaste = FALSE;

			if (m_ptrResource.IsNull() || bReload) {
				//
				// 1. 创建资源共享指针
				//
				if (m_ptrResource.IsNull()) {
					m_ptrResource = CResourcePtr(m_pResourceManager->CreateResource());
				}

				//
				// 2. 加载资源
				//
				if (m_szFileName) {
					if (m_pZipPack) {
						if (m_ptrResource->LoadFromZip(m_pZipPack, m_szFileName) == FALSE) {
							throw m_szFileName;
						}
					}
					else {
						if (m_ptrResource->LoadFromFile(m_szFileName) == FALSE) {
							throw m_szFileName;
						}
					}
				}
			}

			return m_ptrResource->IsValid();
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResourceHandle::LoadResource: %s %s %d\n", szError, __FILE__, __LINE__);
			FreeResource();

			return FALSE;
		}
	}

	//
	// 释放资源
	//
	VOID CResourceHandle::FreeResource(VOID)
	{
		m_ptrResource.SetNull();
	}

	//
	// 删除资源
	//
	VOID CResourceHandle::DeleteResource(VOID)
	{
		//
		// 1. 删除压缩包内文件
		//
		if (m_szFileName && m_pZipPack) {
			// ...
			return;
		}

		//
		// 2. 删除文件
		//
		if (m_szFileName) {
			// ...
			return;
		}
	}


	CResourceManager::CResourceManager(VOID)
	{
		pthread_mutex_init(&m_mutex, NULL);
		Init();
	}

	CResourceManager::~CResourceManager(VOID)
	{
		Free();
		pthread_mutex_destroy(&m_mutex);
	}

	//
	// 初始化资源管理器
	//
	VOID CResourceManager::Init(VOID)
	{
		pthread_mutex_lock(&m_mutex);
		{
			m_packs.clear();
			m_resources.clear();
		}
		pthread_mutex_unlock(&m_mutex);
	}

	//
	// 释放资源管理器
	//
	VOID CResourceManager::Free(VOID)
	{
		pthread_mutex_lock(&m_mutex);
		{
			//
			// 1. 释放所有资源
			//
			for (ResourceMap::const_iterator itResource = m_resources.begin(); itResource != m_resources.end(); ++itResource) {
				const CResourceHandle *pResource = itResource->second;
				ASSERT(pResource);

				if (pResource->IsWaste()) {
					WriteLogI("Waste Resource: %s\n", pResource->GetFileName());
				}

				SAFE_DELETE(pResource);
			}

			m_resources.clear();

			//
			// 2. 关闭所有压缩包
			//
			for (PackMap::const_iterator itPack = m_packs.begin(); itPack != m_packs.end(); ++itPack) {
				ZZIP_DIR *pZipPack = itPack->second;
				ASSERT(pZipPack);

				zzip_closedir(pZipPack);
			}

			m_packs.clear();
		}
		pthread_mutex_unlock(&m_mutex);
	}

	//
	// 销毁资源
	//
	VOID CResourceManager::DestroyResource(CResource * pResource)
	{
		ASSERT(pResource);
		SAFE_DELETE(pResource);
	}

	//
	// 查找资源
	//
	const CResourcePtr& CResourceManager::QueryResource(const CHAR *szName, BOOL bReload)
	{
		ASSERT(szName);
		return QueryResource(HashValue(szName), bReload);
	}

	//
	// 查找资源
	//
	const CResourcePtr& CResourceManager::QueryResource(DWORD dwName, BOOL bReload)
	{
		static const CResourcePtr ptrResourceNull;
		CResourceHandle *pResource = NULL;

		//
		// 1. 查找资源
		//
		pthread_mutex_lock(&m_mutex);
		{
			ResourceMap::const_iterator itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) pResource = itResource->second;
		}
		pthread_mutex_unlock(&m_mutex);

		//
		// 2. 加载资源
		//
		if (pResource) {
			pResource->LoadResource(bReload);
			return pResource->GetResourcePtr();
		}

		return ptrResourceNull;
	}

	//
	// 释放资源
	//
	BOOL CResourceManager::FreeResource(const CHAR *szName)
	{
		ASSERT(szName);
		return FreeResource(HashValue(szName));
	}

	//
	// 释放资源
	//
	BOOL CResourceManager::FreeResource(DWORD dwName)
	{
		CResourceHandle *pResource = NULL;

		//
		// 1. 查找资源
		//
		pthread_mutex_lock(&m_mutex);
		{
			ResourceMap::const_iterator itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) pResource = itResource->second;
		}
		pthread_mutex_unlock(&m_mutex);

		//
		// 2. 释放资源
		//
		if (pResource) {
			pResource->FreeResource();
			return TRUE;
		}

		return FALSE;
	}

	//
	// 删除资源
	//
	BOOL CResourceManager::DeleteResource(const CHAR *szName)
	{
		ASSERT(szName);
		return DeleteResource(HashValue(szName));
	}

	//
	// 删除资源
	//
	BOOL CResourceManager::DeleteResource(DWORD dwName)
	{
		CResourceHandle *pResource = NULL;

		//
		// 1. 查找资源
		//
		pthread_mutex_lock(&m_mutex);
		{
			ResourceMap::const_iterator itResource = m_resources.find(dwName);
			if (itResource != m_resources.end()) { pResource = itResource->second; m_resources.erase(itResource); }
		}
		pthread_mutex_unlock(&m_mutex);

		//
		// 2. 删除资源
		//
		if (pResource) {
			pResource->DeleteResource();
			SAFE_DELETE(pResource);
			return TRUE;
		}

		return FALSE;
	}

	//
	// 获得资源集合
	//
	const CResourceManager::ResourceMap& CResourceManager::GetResources(VOID) const
	{
		return m_resources;
	}

	//
	// 复制资源
	//
	const CResourcePtr& CResourceManager::CopyFrom(const CHAR *szName, const CResource *pCopyFrom)
	{
		static const CResourcePtr ptrResourceNull;
		CResourceHandle *pResource = NULL;

		//
		// 1. 参数安全检查
		//
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

		//
		// 2. 查找资源
		//
		pthread_mutex_lock(&m_mutex);
		{
			DWORD dwName = HashValue(szName);
			ResourceMap::const_iterator itResource = m_resources.find(dwName);

			if (itResource != m_resources.end()) {
				pResource = itResource->second;
			}
			else {
				pResource = SAFE_NEW(MEMTYPE_HEAP) CResourceHandle(this, szName);
				m_resources[dwName] = pResource;
			}
		}
		pthread_mutex_unlock(&m_mutex);

		//
		// 3. 复制资源
		//
		const CResourcePtr &ptrResource = pResource->GetResourcePtr();

		ptrResource->CopyFrom(pCopyFrom);
		ptrResource->SetName(szName);

		return ptrResource;
	}

	//
	// 重新加载资源
	//
	BOOL CResourceManager::Reload(VOID)
	{
		pthread_mutex_lock(&m_mutex);
		{
			for (ResourceMap::const_iterator itResource = m_resources.begin(); itResource != m_resources.end(); ++itResource) {
				CResourceHandle *pResource = itResource->second;
				ASSERT(pResource);

				pResource->LoadResource(TRUE);
			}
		}
		pthread_mutex_unlock(&m_mutex);

		return TRUE;
	}

	//
	// 从文件加载
	//
	BOOL CResourceManager::LoadFromFile(const CHAR *szFileName, const CHAR *szExtName)
	{
		CResourceHandle *pResource = NULL;

		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		//
		// 2. 检查文件类型
		//
		CHAR szFName[_MAX_STRING];
		CHAR szEName[_MAX_STRING];

		splitfilename(szFileName, szFName, szEName);
		if (stricmp(szExtName, szEName)) return FALSE;

		//
		// 3. 查找资源
		//
		pthread_mutex_lock(&m_mutex);
		{
			CHAR szName[_MAX_STRING];
			sprintf(szName, "%s%s", szFName, szEName);

			DWORD dwName = HashValue(szName);
			ResourceMap::const_iterator itResource = m_resources.find(dwName);

			if (itResource != m_resources.end()) {
				pResource = itResource->second;
				WriteLogW("Exist resource name = %s\n", szFileName);
			}
			else {
				pResource = SAFE_NEW(MEMTYPE_HEAP) CResourceHandle(this, szName, szFileName);
				m_resources[dwName] = pResource;
			}
		}
		pthread_mutex_unlock(&m_mutex);

		//
		// 4. 预加载资源
		//
		return PreLoad(pResource);
	}

	//
	// 从文件夹加载
	//
	BOOL CResourceManager::LoadFromPath(const CHAR *szPathName, const CHAR *szExtName)
	{
		//
		// 1. 参数安全检查
		//
		if (szPathName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		//
		// 2. 遍历文件夹
		//
#if defined _PLATFORM_WINDOWS_

		CHAR szFilter[_MAX_STRING];
		sprintf(szFilter, "%s/*", szPathName);

#else

		CHAR szFilter[_MAX_STRING];
		sprintf(szFilter, "%s/", szPathName);

#endif

		struct _finddata_t fileData;
		size_t hFile = (size_t)_findfirst(szFilter, &fileData);

		if (hFile != -1) {
			do {
				CResourceHandle *pResource = NULL;

				//
				// 2.1. 目录检查
				//
				if (fileData.attrib&_A_SUBDIR) {
					continue;
				}

				//
				// 2.2. 检查文件类型
				//
				CHAR szFName[_MAX_STRING];
				CHAR szEName[_MAX_STRING];

				splitfilename(fileData.name, szFName, szEName);
				if (stricmp(szExtName, szEName)) continue;

				//
				// 2.3. 查找资源
				//
				pthread_mutex_lock(&m_mutex);
				{
					CHAR szName[_MAX_STRING];
					CHAR szFileName[_MAX_STRING];

					sprintf(szName, "%s%s", szFName, szEName);
					sprintf(szFileName, "%s/%s", szPathName, szName);

					DWORD dwName = HashValue(szName);
					ResourceMap::const_iterator itResource = m_resources.find(dwName);

					if (itResource != m_resources.end()) {
						pResource = itResource->second;
						WriteLogW("Exist resource name = %s\n", szFileName);
					}
					else {
						pResource = SAFE_NEW(MEMTYPE_HEAP) CResourceHandle(this, szName, szFileName);
						m_resources[dwName] = pResource;
					}
				}
				pthread_mutex_unlock(&m_mutex);

				//
				// 2.4. 预加载资源
				//
				PreLoad(pResource);
			} while (_findnext(hFile, &fileData) == 0);

			_findclose(hFile);
		}

		return TRUE;
	}

	//
	// 从压缩包加载
	//
	BOOL CResourceManager::LoadFromZip(const CHAR *szZipName, const CHAR *szExtName)
	{
		ZZIP_DIR *pZipPack = NULL;
		ZZIP_DIRENT *pZipEntry = NULL;

		//
		// 1. 参数安全检查
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szExtName == NULL) {
			return FALSE;
		}

		//
		// 2. 打开压缩包文件
		//
		pthread_mutex_lock(&m_mutex);
		{
			DWORD dwPackName = HashValue(szZipName);
			PackMap::const_iterator itPack = m_packs.find(dwPackName);

			if (itPack != m_packs.end()) {
				pZipPack = itPack->second;
			}
			else {
				pZipPack = zzip_opendir(szZipName);
				m_packs[dwPackName] = pZipPack;
			}
		}
		pthread_mutex_unlock(&m_mutex);

		zzip_seekdir(pZipPack, 0);

		//
		// 3. 遍历压缩包
		//
		while (pZipEntry = zzip_readdir(pZipPack)) {
			CResourceHandle *pResource = NULL;

			//
			// 3.1. 检查文件类型
			//
			CHAR szFName[_MAX_STRING];
			CHAR szEName[_MAX_STRING];

			splitfilename(pZipEntry->d_name, szFName, szEName);
			if (stricmp(szExtName, szEName)) continue;

			//
			// 3.2. 查找资源
			//
			pthread_mutex_lock(&m_mutex);
			{
				CHAR szName[_MAX_STRING];
				CHAR szFileName[_MAX_STRING];

				sprintf(szName, "%s%s", szFName, szEName);
				sprintf(szFileName, "%s", pZipEntry->d_name);

				DWORD dwName = HashValue(szName);
				ResourceMap::const_iterator itResource = m_resources.find(dwName);

				if (itResource != m_resources.end()) {
					pResource = itResource->second;
					WriteLogW("Exist resource name = %s<%s>\n", szZipName, szFileName);
				}
				else {
					pResource = SAFE_NEW(MEMTYPE_HEAP) CResourceHandle(this, szName, szFileName, pZipPack);
					m_resources[dwName] = pResource;
				}
			}
			pthread_mutex_unlock(&m_mutex);

			//
			// 3.3. 预加载资源
			//
			PreLoad(pResource);
		}

		return TRUE;
	}

	//
	// 预加载资源
	//
	BOOL CResourceManager::PreLoad(CResourceHandle *pResource)
	{
		return TRUE;
	}

	//
	// 垃圾回收
	//
	VOID CResourceManager::GarbageCollection(VOID)
	{
		pthread_mutex_lock(&m_mutex);
		{
			for (ResourceMap::const_iterator itResource = m_resources.begin(); itResource != m_resources.end(); ++itResource) {
				CResourceHandle *pResource = itResource->second;
				ASSERT(pResource);

				// 注意: 这里直接使用m_ptrResource成员变量为了避免调用GetResourcePtr()函数
				//       引起的引用计数增加, 从而导致无法资源回收。
				if (DWORD *pdwRefCount = pResource->m_ptrResource.GetRefCount()) {
					if (1 == *pdwRefCount) { // 只有资源管理器持有资源
						pResource->m_ptrResource.SetNull();
					}
				}
			}
		}
		pthread_mutex_unlock(&m_mutex);
	}

}
*/