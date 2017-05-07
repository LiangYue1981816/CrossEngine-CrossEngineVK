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
#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CResource
	{
		friend class CResourceManager;


	public:
		typedef enum {
			SHADER, TEXTURE, MATERIAL, MESH, SKELETON, EFFECT, SOUND
		} TYPE;


	protected:
		CResource(CResourceManager *pResourceManager);
		virtual ~CResource(void);


	public:
		virtual void Init(void);
		virtual void Free(void);

		virtual BOOL IsValid(void) const = 0;

	public:
		virtual TYPE GetType(void) const = 0;

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

		virtual BOOL SaveToFile(const char *szFileName);
		virtual BOOL SaveToFileStream(FILE *pFile);


	protected:
		CStream m_stream;

	protected:
		CResourceManager *m_pResourceManager;
	};

	class CROSS_EXPORT CResourcePtr : public CSharedPtr<CResource>
	{
	public:
		CResourcePtr(void) : CSharedPtr<CResource>() {}
		CResourcePtr(const CResource *pResource) : CSharedPtr<CResource>(pResource) {}
		CResourcePtr(const CResourcePtr &ptrResource) : CSharedPtr<CResource>(ptrResource) {}
		virtual ~CResourcePtr(void) {}


	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}


	public:
		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}
*/