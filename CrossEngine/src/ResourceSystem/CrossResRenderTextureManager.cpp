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

	CResRenderTextureManager::CResRenderTextureManager(void)
	{

	}

	CResRenderTextureManager::~CResRenderTextureManager(void)
	{

	}

	RESOURCE_TYPE CResRenderTextureManager::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_RENDER_TEXTURE;
	}

	CResource* CResRenderTextureManager::CreateResource(void)
	{
		return SAFE_NEW CResRenderTexture(this);
	}

	BOOL CResRenderTextureManager::PreLoadFromFile(const char *szFileName)
	{
		return CResourceManager::PreLoadFromFile(szFileName, RENDERTEXTURE_EXT_NAME);
	}

	BOOL CResRenderTextureManager::PreLoadFromPath(const char *szPathName)
	{
		return CResourceManager::PreLoadFromPath(szPathName, RENDERTEXTURE_EXT_NAME);
	}

	BOOL CResRenderTextureManager::PreLoadFromPack(const char *szPackName)
	{
		return CResourceManager::PreLoadFromPack(szPackName, RENDERTEXTURE_EXT_NAME);
	}

}
