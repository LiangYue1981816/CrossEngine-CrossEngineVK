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

	RESOURCE_TYPE CResRenderTexture::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_RENDER_TEXTURE;
	}

	const CGfxRenderTexturePtr& CResRenderTexture::GetRenderTexture(void) const
	{
		return m_ptrRenderTexture;
	}

	BOOL CResRenderTexture::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrRenderTexture.IsNull() || m_ptrRenderTexture->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	void CResRenderTexture::Init(void)
	{
		CResource::Init();
	}

	void CResRenderTexture::Free(void)
	{
		m_ptrRenderTexture.Release();
		CResource::Free();
	}

	BOOL CResRenderTexture::InternalLoad(BOOL bSyncPostLoad)
	{
		uint32_t size;
		uint32_t mark;

		m_stream << size;
		m_stream << mark;

		if (size != sizeof(RenderTextureParam)) {
			return FALSE;
		}

		if (mark != HashValue("RenderTextureParam")) {
			return FALSE;
		}

		m_stream << m_data;

		return TRUE;
	}

	BOOL CResRenderTexture::InternalPostLoad(void)
	{
		m_ptrRenderTexture = GfxDevice()->NewRenderTexture();

		if (m_data.type == RENDER_TEXTURE_TYPE_COLOR) {
			return m_ptrRenderTexture->CreateColorTarget(m_data.format, m_data.width, m_data.height, m_data.samples, m_minFilter, m_magFilter, m_mipmapMode, m_addressMode);
		}
		else {
			return m_ptrRenderTexture->CreateDepthStencilTarget(m_data.format, m_data.width, m_data.height, m_data.samples, m_minFilter, m_magFilter, m_mipmapMode, m_addressMode);
		}
	}

	void CResRenderTexture::InternalCleanup(void)
	{
		m_stream.Free();
	}

}
