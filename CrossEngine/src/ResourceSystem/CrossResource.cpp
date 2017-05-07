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

	CResource::CResource(CResourceManager *pResourceManager)
		: m_pResourceManager(pResourceManager)
	{
		Init();
	}

	CResource::~CResource(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CResource::Init(VOID)
	{
		m_stream.Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CResource::Free(VOID)
	{
		m_stream.Free();
		Init();
	}

	//
	// �����Դ������
	//
	CResourceManager* CResource::GetResourceManager(VOID) const
	{
		return m_pResourceManager;
	}

	//
	// ���������
	//
	const CStream* CResource::GetStream(VOID) const
	{
		return &m_stream;
	}

	//
	// ������Դ��
	//
	BOOL CResource::SetName(const CHAR *szName)
	{
		ASSERT(szName);
		return m_stream.SetName(szName);
	}

	//
	// �����Դ��
	//
	const CHAR* CResource::GetName(VOID) const
	{
		return m_stream.GetName();
	}

	//
	// �����Դ��
	//
	DWORD CResource::GetHashName(VOID) const
	{
		return m_stream.GetHashName();
	}

	//
	// �����ļ���
	//
	BOOL CResource::SetFileName(const CHAR *szFileName)
	{
		ASSERT(szFileName);
		return m_stream.SetFileName(szFileName);
	}

	//
	// ����ļ���
	//
	const CHAR* CResource::GetFileName(VOID) const
	{
		return m_stream.GetFileName();
	}

	//
	// ������Դ
	//
	BOOL CResource::CopyFrom(const CResource *pCopyFrom)
	{
		//
		// 1. ������ȫ���
		//
		if (pCopyFrom == NULL) {
			return FALSE;
		}

		if (pCopyFrom->IsValid() == FALSE) {
			return FALSE;
		}

		if (pCopyFrom->GetType() != GetType()) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Copy from resource: %s\n", pCopyFrom->GetName());

			if (m_stream.CopyFrom(pCopyFrom->GetStream()) == FALSE) throw "Copy stream failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResource::CopyFrom: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ���ļ�������Դ
	//
	BOOL CResource::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. ������ȫ���
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load from file: %s\n", szFileName);

			if (m_stream.LoadFromFile(szFileName) == FALSE) throw "Load file failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResource::LoadFromFile: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CResource::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. ������ȫ���
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load from zip: %s<%s>\n", szZipName, szFileName);

			if (m_stream.LoadFromZip(szZipName, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResource::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CResource::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. ������ȫ���
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load from zip: <%s>\n", szFileName);

			if (m_stream.LoadFromZip(pZipPack, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResource::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ���浽�ļ�
	//
	BOOL CResource::SaveToFile(const CHAR *szFileName)
	{
		//
		// 1. ������ȫ���
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ���浽�ļ�
		//
		FILE *pFile = NULL;

		try {
			WriteLogI("Save to file: %s\n", szFileName);

			//
			// 2.1. ���ļ�
			//
			pFile = fopen(szFileName, "wb");
			if (pFile == NULL) throw "Open file failed.";

			//
			// 2.2. �����ļ�
			//
			if (SaveToFileStream(pFile) == FALSE) {
				throw "Save file failed.";
			}

			//
			// 2.3. �����ļ���
			//
			SetFileName(szFileName);

			//
			// 2.4. �ر��ļ�
			//
			fclose(pFile);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CResource::SaveToFile: %s %s %d\n", szError, __FILE__, __LINE__);

			if (pFile) {
				fclose(pFile);
			}

			return FALSE;
		}
	}

	//
	// ���浽�ļ�������
	//
	BOOL CResource::SaveToFileStream(FILE *pFile)
	{
		return TRUE;
	}

}
*/