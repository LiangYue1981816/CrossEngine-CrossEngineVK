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

	CTextureManager::CTextureManager(void)
	{

	}

	CTextureManager::~CTextureManager(void)
	{

	}

	RESOURCE_TYPE CTextureManager::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_TEXTURE;
	}

	CResource* CTextureManager::CreateResource(void)
	{
		return SAFE_NEW CTexture(this, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	}

	CResource* CTextureManager::CreateResource(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		return SAFE_NEW CTexture(this, minFilter, magFilter, mipmapMode, addressMode);
	}

	BOOL CTextureManager::PreLoadFromFile(const char *szFileName)
	{
		if (CResourceManager::PreLoadFromFile(szFileName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromFile(szFileName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CTextureManager::PreLoadFromPath(const char *szPathName)
	{
		if (CResourceManager::PreLoadFromPath(szPathName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPath(szPathName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CTextureManager::PreLoadFromPack(const char *szPackName)
	{
		if (CResourceManager::PreLoadFromPack(szPackName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::PreLoadFromPack(szPackName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

}
