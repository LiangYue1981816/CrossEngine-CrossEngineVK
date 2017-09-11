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
		SetState(RESOURCE_LOAD_STATE_INIT);
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

	BOOL CResource::SetName(const char *szName)
	{
		return m_stream.SetName(szName);
	}

	BOOL CResource::SetFileName(const char *szFileName)
	{
		return m_stream.SetFileName(szFileName);
	}

	DWORD CResource::GetHashName(void) const
	{
		return m_stream.GetHashName();
	}

	const char* CResource::GetName(void) const
	{
		return m_stream.GetName();
	}

	const char* CResource::GetFileName(void) const
	{
		return m_stream.GetFileName();
	}

	void CResource::SetState(RESOURCE_LOAD_STATE state)
	{
		mutex_autolock mutex(m_mutex);
		m_state = state;
	}

	RESOURCE_LOAD_STATE CResource::GetState(void)
	{
		mutex_autolock mutex(m_mutex);
		return m_state;
	}

	BOOL CResource::CopyFrom(const CResource *pCopyFrom)
	{
		try {
			if (pCopyFrom == NULL) {
				throw "Invalid resource.";
			}

			if (pCopyFrom->GetType() != GetType()) {
				throw "Invalid resource type.";
			}

			if (m_stream.CopyFrom(pCopyFrom->GetStream()) == FALSE) {
				throw "Copy stream failed.";
			}

			if (Load() == FALSE) {
				throw "Load failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::CopyFrom(0x%016x): %s\n", pCopyFrom, szError);
			Free();

			return FALSE;
		}
	}

	BOOL CResource::LoadFromFile(const char *szFileName)
	{
		try {
			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromFile(szFileName) == FALSE) {
				throw "Load stream from file failed.";
			}

			if (Load() == FALSE) {
				throw "Load failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromFile(\"%s\"): %s\n", szFileName ? szFileName : "NULL", szError);
			Free();

			return FALSE;
		}
	}

	BOOL CResource::LoadFromPack(const char *szPackName, const char *szFileName)
	{
		try {
			if (szPackName == NULL) {
				throw "Invalid pack name.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromPack(szPackName, szFileName)) {
				throw "Load stream from pack failed.";
			}

			if (Load() == FALSE) {
				throw "Load failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromPack(\"%s\", \"%s\"): %s\n", szPackName ? szPackName : "NULL", szFileName ? szFileName : "NULL", szError);
			Free();

			return FALSE;
		}
	}

	BOOL CResource::LoadFromPack(ZZIP_DIR *pPack, const char *szFileName)
	{
		try {
			if (pPack == NULL) {
				throw "Invalid pack.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (m_stream.LoadFromPack(pPack, szFileName)) {
				throw "Load stream from pack failed.";
			}

			if (Load() == FALSE) {
				throw "Load failed.";
			}

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CResource::LoadFromPack(0x%016x, \"%s\"): %s\n", pPack, szFileName ? szFileName : "NULL", szError);
			Free();

			return FALSE;
		}
	}

}
