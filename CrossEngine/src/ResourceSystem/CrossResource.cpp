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
	// 初始化资源
	//
	VOID CResource::Init(VOID)
	{
		m_stream.Init();
	}

	//
	// 释放资源
	//
	VOID CResource::Free(VOID)
	{
		m_stream.Free();
		Init();
	}

	//
	// 获得资源管理器
	//
	CResourceManager* CResource::GetResourceManager(VOID) const
	{
		return m_pResourceManager;
	}

	//
	// 获得数据流
	//
	const CStream* CResource::GetStream(VOID) const
	{
		return &m_stream;
	}

	//
	// 设置资源名
	//
	BOOL CResource::SetName(const CHAR *szName)
	{
		ASSERT(szName);
		return m_stream.SetName(szName);
	}

	//
	// 获得资源名
	//
	const CHAR* CResource::GetName(VOID) const
	{
		return m_stream.GetName();
	}

	//
	// 获得资源名
	//
	DWORD CResource::GetHashName(VOID) const
	{
		return m_stream.GetHashName();
	}

	//
	// 设置文件名
	//
	BOOL CResource::SetFileName(const CHAR *szFileName)
	{
		ASSERT(szFileName);
		return m_stream.SetFileName(szFileName);
	}

	//
	// 获得文件名
	//
	const CHAR* CResource::GetFileName(VOID) const
	{
		return m_stream.GetFileName();
	}

	//
	// 复制资源
	//
	BOOL CResource::CopyFrom(const CResource *pCopyFrom)
	{
		//
		// 1. 参数安全检查
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
		// 2. 释放资源
		//
		Free();

		//
		// 3. 复制资源
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
	// 从文件加载资源
	//
	BOOL CResource::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 从压缩包加载资源
	//
	BOOL CResource::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 从压缩包加载资源
	//
	BOOL CResource::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 保存到文件
	//
	BOOL CResource::SaveToFile(const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 保存到文件
		//
		FILE *pFile = NULL;

		try {
			WriteLogI("Save to file: %s\n", szFileName);

			//
			// 2.1. 打开文件
			//
			pFile = fopen(szFileName, "wb");
			if (pFile == NULL) throw "Open file failed.";

			//
			// 2.2. 保存文件
			//
			if (SaveToFileStream(pFile) == FALSE) {
				throw "Save file failed.";
			}

			//
			// 2.3. 设置文件名
			//
			SetFileName(szFileName);

			//
			// 2.4. 关闭文件
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
	// 保存到文件数据流
	//
	BOOL CResource::SaveToFileStream(FILE *pFile)
	{
		return TRUE;
	}

}
*/