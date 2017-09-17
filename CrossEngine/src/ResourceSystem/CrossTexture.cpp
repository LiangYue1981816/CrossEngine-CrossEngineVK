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

	CTexture::CTexture(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		m_ptrGfxTexture = GfxDevice()->NewTexture();
	}

	CTexture::~CTexture(void)
	{
		m_ptrGfxTexture.Release();
	}

	RESOURCE_TYPE CTexture::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_TEXTURE;
	}

	const CGfxTexturePtr& CTexture::GetGfxTexture(void) const
	{
		return m_ptrGfxTexture;
	}

	BOOL CTexture::Load(void)
	{
		m_texture = (gli::texture2d)gli::load((const char *)m_stream.GetAddress(), m_stream.GetFullSize());
		m_stream.Free();

		return m_texture.empty() ? FALSE : TRUE;
	}

	BOOL CTexture::PostLoad(void)
	{
		BOOL rcode = m_ptrGfxTexture->CreateTexture2D(m_texture, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
		m_texture.clear();

		return rcode;
	}

}
