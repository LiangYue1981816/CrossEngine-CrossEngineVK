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

	class CROSS_EXPORT CResourceHandle
	{
		friend class CResourceManager;


	protected:
		CResourceHandle(CResourceManager *pResourceManager);
		CResourceHandle(CResourceManager *pResourceManager, const char *szFileName);
		CResourceHandle(CResourceManager *pResourceManager, const char *szFileName, ZZIP_DIR *pPack);
		virtual ~CResourceHandle(void);


	protected:
		BOOL IsWaste(void) const;
		const char* GetFileName(void) const;
		CResourcePtr<CResource>& GetResourcePtr(void);

	protected:
		BOOL LoadResource(BOOL bReload, BOOL bSync);
		BOOL LoadResource(void);
		BOOL LoadResourcePost(void);
		BOOL CopyFrom(const CResource *pCopyFrom);
		BOOL FreeResource(void);


	protected:
		BOOL m_bIsWaste;

	protected:
		ZZIP_DIR *m_pPack;
		char m_szFileName[_MAX_STRING];
		CResourcePtr<CResource> m_ptrResource;

	protected:
		CResourceManager *m_pResourceManager;
	};

	class CROSS_EXPORT CResourceManager
	{
		friend class CResourceSystem;


	public:
		typedef std::map<DWORD, ZZIP_DIR*> PackMap;
		typedef std::map<DWORD, CResourceHandle*> ResourceMap;


	protected:
		CResourceManager(void);
		virtual ~CResourceManager(void);


	protected:
		virtual void Init(void);
		virtual void Free(void);

	public:
		virtual RESOURCE_TYPE GetType(void) const = 0;

	public:
		virtual CResource* CreateResource(void) = 0;
		virtual void DestroyResource(CResource *pResource);

	public:
		virtual const CResourcePtr<CResource>& LoadResource(DWORD dwName, BOOL bReload = FALSE, BOOL bSync = FALSE);
		virtual const CResourcePtr<CResource>& CopyResource(DWORD dwName, const CResource *pCopyFrom);
		virtual BOOL FreeResource(DWORD dwName);

	public:
		virtual BOOL PreLoadFromFile(const char *szFileName) = 0;
		virtual BOOL PreLoadFromPath(const char *szPathName) = 0;
		virtual BOOL PreLoadFromPack(const char *szPackName) = 0;

	protected:
		virtual BOOL PreLoadFromFile(const char *szFileName, const char *szExtName);
		virtual BOOL PreLoadFromPath(const char *szPathName, const char *szExtName);
		virtual BOOL PreLoadFromPack(const char *szPackName, const char *szExtName);

		virtual BOOL Load(CResourceHandle *pResource);

	public:
		virtual void GarbageCollection(void);


	protected:
		PackMap m_packs;
		ResourceMap m_resources;

	protected:
		pthread_mutex_t m_mutex;
	};

}
