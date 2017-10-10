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

	CStream::CStream(void)
	{
		Init();
	}

	CStream::~CStream(void)
	{
		Free();
	}

	BOOL CStream::IsValid(void) const
	{
		return (m_pAddress && m_size) ? TRUE : FALSE;
	}

	void CStream::Init(void)
	{
		m_dwName = INVALID_HASHNAME;
		memset(m_szName, 0, sizeof(m_szName));

		m_pPack = NULL;
		memset(m_szPackName, 0, sizeof(m_szPackName));
		memset(m_szFileName, 0, sizeof(m_szFileName));

		m_bAlloced = FALSE;
		m_pAddress = NULL;

		m_size = 0;
		m_position = 0;
	}

	void CStream::Free(void)
	{
		FreeData();
		Init();
	}

	void CStream::FreeData(void)
	{
		if (m_bAlloced) {
			SAFE_FREE(m_pAddress);
		}

		m_bAlloced = FALSE;
		m_pAddress = NULL;

		m_size = 0;
		m_position = 0;
	}

	BOOL CStream::Alloc(size_t size)
	{
		if (size == 0) {
			return FALSE;
		}

		if (IsValid()) {
			return FALSE;
		}

		m_pAddress = (BYTE *)SAFE_ALLOC(size, MEMTYPE_HEAP);
		if (m_pAddress == NULL) return FALSE;

		m_size = size;
		m_position = 0;

		m_bAlloced = TRUE;

		return TRUE;
	}

	BOOL CStream::SetStream(BYTE *pAddress, size_t size)
	{
		if (pAddress == NULL) {
			return FALSE;
		}

		if (size == 0) {
			return FALSE;
		}

		if (IsValid()) {
			return FALSE;
		}

		m_bAlloced = FALSE;
		m_pAddress = pAddress;

		m_size = size;
		m_position = 0;

		return TRUE;
	}

	BOOL CStream::CopyFrom(const CStream *pStream)
	{
		Free();

		try {
			if (pStream == NULL) {
				throw "Invalid stream.";
			}

			if (pStream->IsValid() == FALSE) {
				throw "Invalid stream.";
			}

			if (Alloc(pStream->GetFullSize()) == FALSE) {
				throw "Alloc stream failed.";
			}

			SetName(pStream->GetName());
			SetPack(pStream->GetPack());
			SetPackName(pStream->GetPackName());
			SetFileName(pStream->GetFileName());

			memcpy(m_pAddress, pStream->GetAddress(), pStream->GetFullSize());

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CStream::CopyFrom(0x%016x): %s\n", pStream, szError);
			Free();

			return FALSE;
		}
	}

	BOOL CStream::LoadFromFile(const char *szFileName)
	{
		FILE *pFile = NULL;
		Free();

		try {
			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			pFile = fopen(szFileName, "rb");
			if (pFile == NULL) {
				throw "Open file failed.";
			}

			if (Alloc(fsize(pFile)) == FALSE) {
				throw "Alloc stream failed.";
			}

			if (m_size != fread(m_pAddress, 1, m_size, pFile)) {
				throw "Read file data failed.";
			}

			SetFileName(szFileName);

			fclose(pFile);

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CStream::LoadFromFile(\"%s\"): %s\n", szFileName ? szFileName : "NULL", szError);
			Free();

			if (pFile) {
				fclose(pFile);
			}

			return FALSE;
		}
	}

	BOOL CStream::LoadFromPack(const char *szPackName, const char *szFileName)
	{
		ZZIP_DIR *pPack = NULL;
		Free();

		try {
			if (szPackName == NULL) {
				throw "Invalid pack name.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			pPack = zzip_opendir(szPackName);
			if (pPack == NULL) {
				throw "Open pack failed.";
			}

			if (LoadFromPack(pPack, szFileName) == FALSE) {
				throw "Load pack file failed.";
			}

			SetPackName(szPackName);
			SetFileName(szFileName);

			zzip_closedir(pPack);

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CStream::LoadFromPack(\"%s\", \"%s\"): %s\n", szPackName ? szPackName : "NULL", szFileName ? szFileName : "NULL", szError);
			Free();

			if (pPack) {
				zzip_closedir(pPack);
			}

			return FALSE;
		}
	}

	BOOL CStream::LoadFromPack(ZZIP_DIR *pPack, const char *szFileName)
	{
		ZZIP_FILE *pFile = NULL;
		ZZIP_STAT zstat;
		Free();

		try {
			if (pPack == NULL) {
				throw "Invalid pack.";
			}

			if (szFileName == NULL) {
				throw "Invalid file name.";
			}

			if (zzip_dir_stat(pPack, szFileName, &zstat, ZZIP_CASEINSENSITIVE) != ZZIP_NO_ERROR) {
				throw "Can not find file.";
			}

			pFile = zzip_file_open(pPack, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS);
			if (pFile == NULL) {
				throw "Open file failed.";
			}

			if (Alloc(zstat.st_size) == FALSE) {
				throw "Alloc stream failed.";
			}

			if (m_size != zzip_file_read(pFile, m_pAddress, m_size)) {
				throw "Read file data failed.";
			}

			SetPack(pPack);
			SetFileName(szFileName);

			zzip_file_close(pFile);

			return TRUE;
		}
		catch (const char *szError) {
			LOGE("CStream::LoadFromPack(0x%016x, \"%s\"): %s\n", pPack, szFileName ? szFileName : "NULL", szError);
			Free();

			if (pFile) {
				zzip_file_close(pFile);
			}

			return FALSE;
		}
	}

	BOOL CStream::Reload(void)
	{
		ZZIP_DIR *pPack = m_pPack;

		char szPackName[_MAX_STRING];
		char szFileName[_MAX_STRING];

		strcpy(szPackName, m_szPackName);
		strcpy(szFileName, m_szFileName);

		if (strlen(szFileName)) {
			if (pPack) {
				return LoadFromPack(pPack, szFileName);
			}

			if (strlen(szPackName)) {
				return LoadFromPack(szPackName, szFileName);
			}

			return LoadFromFile(szFileName);
		}

		return FALSE;
	}

	BOOL CStream::SetName(const char *szName)
	{
		if (szName == NULL) {
			return FALSE;
		}

		strcpy(m_szName, szName);
		m_dwName = HashValue(m_szName);

		return TRUE;
	}

	const char* CStream::GetName(void) const
	{
		return m_szName;
	}

	DWORD CStream::GetHashName(void) const
	{
		return m_dwName;
	}

	BOOL CStream::SetFileName(const char *szFileName)
	{
		if (szFileName == NULL) {
			return FALSE;
		}

		char szName[_MAX_STRING];
		char szFName[_MAX_STRING];
		char szEName[_MAX_STRING];

		strcpy(m_szFileName, szFileName);
		splitfilename(m_szFileName, szFName, szEName);
		sprintf(szName, "%s%s", szFName, szEName);

		SetName(szName);

		return TRUE;
	}

	const char* CStream::GetFileName(void) const
	{
		return m_szFileName;
	}

	BOOL CStream::SetPackName(const char *szPackName)
	{
		if (szPackName == NULL) {
			return FALSE;
		}

		strcpy(m_szPackName, szPackName);

		return TRUE;
	}

	const char* CStream::GetPackName(void) const
	{
		return m_szPackName;
	}

	BOOL CStream::SetPack(ZZIP_DIR *pPack)
	{
		if (pPack == NULL) {
			return FALSE;
		}

		m_pPack = pPack;

		return TRUE;
	}

	ZZIP_DIR* CStream::GetPack(void) const
	{
		return m_pPack;
	}

	size_t CStream::GetFullSize(void) const
	{
		return m_size;
	}

	size_t CStream::GetFreeSize(void) const
	{
		return m_size - m_position;
	}

	size_t CStream::GetCurrentPosition(void) const
	{
		return m_position;
	}

	void* CStream::GetAddress(void) const
	{
		return m_pAddress;
	}

	void* CStream::GetCurrentAddress(void) const
	{
		return m_pAddress + m_position;
	}

	size_t CStream::Read(void *pBuffer, size_t size, size_t count)
	{
		if (pBuffer == NULL) {
			return 0;
		}

		if (size == 0 || count == 0) {
			return 0;
		}

		if (IsValid() == FALSE) {
			return 0;
		}

		size_t readSize;

		readSize = size * count;
		readSize = min(readSize, GetFreeSize());

		memcpy(pBuffer, m_pAddress + m_position, readSize);
		m_position += readSize;

		return readSize / size;
	}

	BOOL CStream::Seek(size_t offset, SEEK_MODE mode)
	{
		if (IsValid() == FALSE) {
			return FALSE;
		}

		switch (mode) {
		case SEEK_MODE_CUR:
			if (m_position + offset > m_size) {
				return FALSE;
			}

			m_position = m_position + offset;

			break;
		case SEEK_MODE_END:
			if (m_size + offset > m_size) {
				return FALSE;
			}

			m_position = m_size + offset;

			break;
		case SEEK_MODE_SET:
			if (offset > m_size) {
				return FALSE;
			}

			m_position = offset;

			break;
		}

		return TRUE;
	}

	BOOL CStream::IsEOF(void) const
	{
		return (GetFreeSize() == 0) ? TRUE : FALSE;
	}

}
