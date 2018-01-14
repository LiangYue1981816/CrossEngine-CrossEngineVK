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

	CResRenderTexture::CResRenderTexture(CResourceManager *pResourceManager, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
		: CResource(pResourceManager)
		, m_minFilter(minFilter)
		, m_magFilter(magFilter)
		, m_mipmapMode(mipmapMode)
		, m_addressMode(addressMode)
	{

	}

	CResRenderTexture::~CResRenderTexture(void)
	{

	}

	const CGfxRenderTexturePtr& CResRenderTexture::GetRenderTexture(void) const
	{
		return m_ptrRenderTexture;
	}

	RESOURCE_TYPE CResRenderTexture::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_RENDER_TEXTURE;
	}

	BOOL CResRenderTexture::IsValid(void) const
	{
		return TRUE;
	}

	void CResRenderTexture::Init(void)
	{

	}

	void CResRenderTexture::Free(void)
	{

	}

	BOOL CResRenderTexture::InternalLoad(BOOL bSyncPostLoad)
	{
		return TRUE;
	}

	BOOL CResRenderTexture::InternalPostLoad(void)
	{
		return TRUE;
	}

	void CResRenderTexture::InternalCleanup(void)
	{

	}

}
