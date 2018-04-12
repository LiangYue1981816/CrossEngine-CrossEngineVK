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

	CResRenderTexture::CResRenderTexture(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
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

	BOOL CResRenderTexture::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;

		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pRenderTextureNode = xmlDoc.FirstChild("RenderTexture")) {
				return LoadRenderTexture(pRenderTextureNode);
			}
		}

		return FALSE;
	}

	BOOL CResRenderTexture::InternalPostLoad(void)
	{
		if (m_data.type == RENDER_TEXTURE_TYPE_COLOR) {
			m_ptrRenderTexture = GfxDevice()->NewRenderTexture();
			return m_ptrRenderTexture->CreateColorTarget(m_data.format, m_data.width, m_data.height, m_data.samples, m_data.minFilter, m_data.magFilter, m_data.mipmapMode, m_data.addressMode);
		}
		else {
			m_ptrRenderTexture = GfxDevice()->NewRenderTexture();
			return m_ptrRenderTexture->CreateDepthStencilTarget(m_data.format, m_data.width, m_data.height, m_data.samples, m_data.minFilter, m_data.magFilter, m_data.mipmapMode, m_data.addressMode);
		}
	}

	void CResRenderTexture::InternalLoadFail(void)
	{
		m_ptrRenderTexture.Release();
		CResource::InternalLoadFail();
	}

	void CResRenderTexture::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResRenderTexture::LoadRenderTexture(TiXmlNode *pRenderTextureNode)
	{
		const char *szType = pRenderTextureNode->ToElement()->AttributeString("type");

		if      (stricmp(szType, "color")         == 0) m_data.type = RENDER_TEXTURE_TYPE_COLOR;
		else if (stricmp(szType, "depth_stencil") == 0) m_data.type = RENDER_TEXTURE_TYPE_DEPTH_STENCIL;
		else return FALSE;

		m_data.width = pRenderTextureNode->ToElement()->AttributeInt1("width");
		m_data.height = pRenderTextureNode->ToElement()->AttributeInt1("height");
		m_data.format = CVulkanHelper::StringToFormat(pRenderTextureNode->ToElement()->AttributeString("format"));
		m_data.samples = CVulkanHelper::StringToSampleCountFlagBits(pRenderTextureNode->ToElement()->AttributeString("samples"));
		m_data.minFilter = CVulkanHelper::StringToFilter(pRenderTextureNode->ToElement()->AttributeString("min_filter"));
		m_data.magFilter = CVulkanHelper::StringToFilter(pRenderTextureNode->ToElement()->AttributeString("mag_filter"));
		m_data.mipmapMode = CVulkanHelper::StringToSamplerMipmapMode(pRenderTextureNode->ToElement()->AttributeString("mipmap_mode"));
		m_data.addressMode = CVulkanHelper::StringToSamplerAddressMode(pRenderTextureNode->ToElement()->AttributeString("address_mode"));

		return TRUE;
	}

}
