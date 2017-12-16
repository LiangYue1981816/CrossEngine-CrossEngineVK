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

	CResShaderManager::CResShaderManager(void)
	{

	}

	CResShaderManager::~CResShaderManager(void)
	{

	}

	RESOURCE_TYPE CResShaderManager::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_SHADER;
	}

	CResource* CResShaderManager::CreateResource(void)
	{
		return SAFE_NEW CResShader(this);
	}

	BOOL CResShaderManager::PreLoadFromFile(const char *szFileName)
	{
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CResShaderManager::PreLoadFromPath(const char *szPathName)
	{
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CResShaderManager::PreLoadFromPack(const char *szPackName)
	{
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, SHADER_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CResShaderManager::PreLoad(CResourceHandle *pResourceHandle)
	{
		return pResourceHandle->LoadResource(TRUE, TRUE);
	}

	void CResShaderManager::GarbageCollection(void)
	{

	}

}
