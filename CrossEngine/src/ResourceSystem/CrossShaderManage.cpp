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

	static const CResourcePtr<CResource> ptrResourceNull;

	CShaderManager::CShaderManager(void)
	{

	}

	CShaderManager::~CShaderManager(void)
	{

	}

	RESOURCE_TYPE CShaderManager::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_SHADER;
	}

	CResource* CShaderManager::CreateResource(void)
	{
		return SAFE_NEW CShader(this);
	}

	BOOL CShaderManager::PreLoadFromFile(const char *szFileName)
	{
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CShaderManager::PreLoadFromPath(const char *szPathName)
	{
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CShaderManager::PreLoadFromPack(const char *szPackName)
	{
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	const CResourcePtr<CResource>& CShaderManager::CopyResource(DWORD dwName, const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return ptrResourceNull;
	}

	BOOL CShaderManager::Load(CResourceHandle *pResource)
	{
		return TRUE;
	}

	void CShaderManager::GarbageCollection(void)
	{

	}

}
