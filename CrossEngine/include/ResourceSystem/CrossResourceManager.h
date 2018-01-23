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
		friend class CResourceSystem;
		friend class CResourceManager;
		friend class CResRenderPassManager;
		friend class CResShaderManager;
		friend class CResTextureManager;
		friend class CResComputeManager;
		friend class CResGraphicsManager;
		friend class CResMaterialManager;


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
		BOOL LoadResource(BOOL bSyncLoad, BOOL bSyncPostLoad);
		BOOL Load(BOOL bSyncPostLoad);
		BOOL PostLoad(void);


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
		virtual const CResourcePtr<CResource>& LoadResource(const char *szName, BOOL bSyncLoad = FALSE, BOOL bSyncPostLoad = FALSE);
		virtual const CResourcePtr<CResource>& LoadResource(uint32_t dwName, BOOL bSyncLoad = FALSE, BOOL bSyncPostLoad = FALSE);

	public:
		virtual BOOL PreLoadFromFile(const char *szFileName) = 0;
		virtual BOOL PreLoadFromPath(const char *szPathName) = 0;
		virtual BOOL PreLoadFromPack(const char *szPackName) = 0;

	protected:
		virtual BOOL PreLoadFromFile(const char *szFileName, const char *szExtName);
		virtual BOOL PreLoadFromPath(const char *szPathName, const char *szExtName);
		virtual BOOL PreLoadFromPack(const char *szPackName, const char *szExtName);

		virtual BOOL PreLoad(CResourceHandle *pResourceHandle);

	protected:
		virtual void GarbageCollection(void);


	protected:
		std::map<uint32_t, ZZIP_DIR*> m_pPacks;
		std::map<uint32_t, CResourceHandle*> m_pResources;

	protected:
		pthread_mutex_t m_mutex;
	};

}
