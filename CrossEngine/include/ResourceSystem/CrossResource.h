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

	typedef enum {
		RESOURCE_TYPE_SHADER, RESOURCE_TYPE_TEXTURE, RESOURCE_TYPE_MATERIAL, RESOURCE_TYPE_MESH, RESOURCE_TYPE_SKELETON, RESOURCE_TYPE_EFFECT, RESOURCE_TYPE_SOUND, COUNT
	} RESOURCE_TYPE;

	typedef enum {
		RESOURCE_LOAD_STATE_INIT, RESOURCE_LOAD_STATE_PENDING, RESOURCE_LOAD_STATE_LOADING, RESOURCE_LOAD_STATE_SUCCESS, RESOURCE_LOAD_STATE_FAIL
	} RESOURCE_LOAD_STATE;

	class CROSS_EXPORT CResource
	{
		template<class T>
		friend class CResourcePtr;
		friend class CResourceManager;


	protected:
		CResource(CResourceManager *pResourceManager);
		virtual ~CResource(void);

		virtual void Release(void);


	public:
		virtual BOOL IsValid(void) const = 0;
		virtual RESOURCE_TYPE GetType(void) const = 0;

	public:
		virtual void Init(void);
		virtual void Free(void);

	public:
		virtual const CStream* GetStream(void) const;
		virtual CResourceManager* GetResourceManager(void) const;

	public:
		virtual BOOL SetName(const char *szName);
		virtual const char* GetName(void) const;
		virtual DWORD GetHashName(void) const;

		virtual BOOL SetFileName(const char *szFileName);
		virtual const char* GetFileName(void) const;

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);

		virtual BOOL LoadFromFile(const char *szFileName);
		virtual BOOL LoadFromPack(const char *szPackName, const char *szFileName);
		virtual BOOL LoadFromPack(ZZIP_DIR *pPack, const char *szFileName);
		virtual BOOL LoadFromStream(CStream *pStream) = 0;
		virtual BOOL LoadFromStreamPost(CStream *pStream) = 0;

		virtual BOOL SaveToFile(const char *szFileName);
		virtual BOOL SaveToFileStream(FILE *pFile);

		virtual void SetState(RESOURCE_LOAD_STATE state);
		virtual RESOURCE_LOAD_STATE GetState(void) const;


	protected:
		CStream m_stream;
		RESOURCE_LOAD_STATE m_state;

	protected:
		CResourceManager *m_pResourceManager;
	};

	template<class T>
	class CROSS_EXPORT CResourcePtr : public CSharedPtr<T>
	{
	public:
		CResourcePtr(void) : CSharedPtr<T>()
		{

		}
		CResourcePtr(const T *p) : CSharedPtr<T>(p)
		{

		}
		CResourcePtr(const CResourcePtr &ptr) : CSharedPtr<T>(ptr)
		{

		}
		virtual ~CResourcePtr(void)
		{
			Release();
		}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				((CResource *)m_pPointer)->Release();
			}
		}
	};

}
