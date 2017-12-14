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

	CResource::CResource(CResourceManager *pResourceManager)
		: m_pResourceManager(pResourceManager)
		, m_bIsLoaded(FALSE)
	{
		Init();
	}

	CResource::~CResource(void)
	{
		Free();
	}

	void CResource::Release(void)
	{
		m_pResourceManager->DestroyResource(this);
	}

	void CResource::Init(void)
	{
		m_stream.Init();
	}

	void CResource::Free(void)
	{
		m_stream.Free();
		Init();
	}

	const CStream* CResource::GetStream(void) const
	{
		return &m_stream;
	}

	CResourceManager* CResource::GetResourceManager(void) const
	{
		return m_pResourceManager;
	}

	BOOL CResource::IsLoaded(void) const
	{
		return m_bIsLoaded;
	}

	BOOL CResource::LoadFromFile(const char *szFileName, BOOL bSync)
	{
		try {
			if (IsLoaded()) {
				return TRUE;
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromFile(szFileName) == FALSE) {
				throw "Load stream from file failed.";
			}

			if (InternalLoad(bSync) == FALSE) {
				throw "InternalLoad failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromFile(\"%s\"): %s\n", szFileName ? szFileName : "NULL", szError);

			InternalCleanup();
			Free();

			m_bIsLoaded = TRUE;

			return FALSE;
		}
	}

	BOOL CResource::LoadFromPack(const char *szPackName, const char *szFileName, BOOL bSync)
	{
		try {
			if (IsLoaded()) {
				return TRUE;
			}

			if (szPackName == NULL) {
				throw "Invalid pack name.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromPack(szPackName, szFileName)) {
				throw "Load stream from pack failed.";
			}

			if (InternalLoad(bSync) == FALSE) {
				throw "InternalLoad failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromPack(\"%s\", \"%s\"): %s\n", szPackName ? szPackName : "NULL", szFileName ? szFileName : "NULL", szError);

			InternalCleanup();
			Free();

			m_bIsLoaded = TRUE;

			return FALSE;
		}
	}

	BOOL CResource::LoadFromPack(ZZIP_DIR *pPack, const char *szFileName, BOOL bSync)
	{
		try {
			if (IsLoaded()) {
				return TRUE;
			}

			if (pPack == NULL) {
				throw "Invalid pack.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromPack(pPack, szFileName)) {
				throw "Load stream from pack failed.";
			}

			if (InternalLoad(bSync) == FALSE) {
				throw "InternalLoad failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromPack(0x%016x, \"%s\"): %s\n", pPack, szFileName ? szFileName : "NULL", szError);

			InternalCleanup();
			Free();

			m_bIsLoaded = TRUE;

			return FALSE;
		}
	}

	BOOL CResource::PostLoad(void)
	{
		try {
			if (IsLoaded()) {
				return TRUE;
			}

			if (InternalPostLoad() == FALSE) {
				throw "InternalPostLoad failed.";
			}

			InternalCleanup();

			m_bIsLoaded = TRUE;

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::PostLoad(): %s\n", szError);

			InternalCleanup();
			Free();

			m_bIsLoaded = TRUE;

			return FALSE;
		}
	}

}
