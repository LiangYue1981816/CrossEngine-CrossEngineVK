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

	CResFrameBuffer::CResFrameBuffer(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResFrameBuffer::~CResFrameBuffer(void)
	{

	}

	RESOURCE_TYPE CResFrameBuffer::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_FRAME_BUFFER;
	}

	const CGfxRenderPassPtr& CResFrameBuffer::GetRenderPass(void) const
	{
		return m_ptrRenderPass->GetRenderPass();
	}

	const CGfxFrameBufferPtr& CResFrameBuffer::GetFrameBuffer(void) const
	{
		return m_ptrFrameBuffer;
	}

	BOOL CResFrameBuffer::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrFrameBuffer.IsNull() || m_ptrFrameBuffer->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResFrameBuffer::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;

		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pFrameBufferNode = xmlDoc.FirstChild("FrameBuffer")) {
				return LoadFrameBuffer(pFrameBufferNode, bSyncPostLoad);
			}
		}

		return FALSE;
	}

	BOOL CResFrameBuffer::InternalPostLoad(void)
	{
		m_ptrRenderPass = RenderPassManager()->LoadResource(m_param.dwRenderPassName);
		if (m_ptrRenderPass.IsNull() || m_ptrRenderPass->IsValid() == FALSE) return FALSE;

		m_ptrFrameBuffer = GfxDevice()->NewFrameBuffer(m_ptrRenderPass->GetRenderPass()->GetAttachmentCount());
		{
			for (int index = 0; index < m_param.attachmentPresents.size(); index++) {
				if (m_ptrFrameBuffer->SetPresentAttachment(m_param.attachmentPresents[index].indexAttachment, GfxSwapChain()->GetFormat(), GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), GfxSwapChain()->GetImageHandle(m_param.attachmentPresents[index].indexSurface)) == FALSE) {
					return FALSE;
				}
			}

			for (int index = 0; index < m_param.attachmentColors.size(); index++) {
				if (m_param.attachmentColors[index].ptrRenderTexture->IsValid() == FALSE) {
					return FALSE;
				}

				if (m_ptrFrameBuffer->SetColorAttachment(m_param.attachmentColors[index].indexAttachment, m_param.attachmentColors[index].ptrRenderTexture->GetRenderTexture()) == FALSE) {
					return FALSE;
				}
			}

			for (int index = 0; index < m_param.attachmentDepthStencils.size(); index++) {
				if (m_param.attachmentDepthStencils[index].ptrRenderTexture->IsValid() == FALSE) {
					return FALSE;
				}

				if (m_ptrFrameBuffer->SetDepthStencilAttachment(m_param.attachmentDepthStencils[index].indexAttachment, m_param.attachmentDepthStencils[index].ptrRenderTexture->GetRenderTexture()) == FALSE) {
					return FALSE;
				}
			}
		}
		return m_ptrFrameBuffer->Create(m_ptrRenderPass->GetRenderPass()->GetHandle());
	}

	void CResFrameBuffer::InternalLoadFail(void)
	{
		m_ptrRenderPass.Release();
		m_ptrFrameBuffer.Release();

		m_param.attachmentPresents.clear();
		m_param.attachmentColors.clear();
		m_param.attachmentDepthStencils.clear();

		CResource::InternalLoadFail();
	}

	void CResFrameBuffer::InternalLoadSuccess(void)
	{
		m_param.attachmentPresents.clear();
		m_param.attachmentColors.clear();
		m_param.attachmentDepthStencils.clear();

		CResource::InternalLoadSuccess();
	}

	BOOL CResFrameBuffer::LoadFrameBuffer(TiXmlNode *pFrameBufferNode, BOOL bSyncPostLoad)
	{
		BOOL rcode = FALSE;

		if (TiXmlNode *pRenderPassNode = pFrameBufferNode->FirstChild("RenderPass")) {
			if (LoadRenderPass(pRenderPassNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("Presents")) {
			if (LoadAttachmentPresents(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("Colors")) {
			if (LoadAttachmentColors(pAttachmentNodes, bSyncPostLoad)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("DepthStencils")) {
			if (LoadAttachmentDepthStencils(pAttachmentNodes, bSyncPostLoad)) {
				rcode = TRUE;
			}
		}

		return rcode;
	}

	BOOL CResFrameBuffer::LoadRenderPass(TiXmlNode *pRenderPassNode)
	{
		m_param.dwRenderPassName = HashValue(pRenderPassNode->ToElement()->AttributeString("name"));
		return TRUE;
	}

	BOOL CResFrameBuffer::LoadAttachmentPresents(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentPresentParam param;
				{
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.indexSurface = pAttachmentNode->ToElement()->AttributeInt1("index_surface");
				}
				m_param.attachmentPresents.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResFrameBuffer::LoadAttachmentColors(TiXmlNode *pAttachmentNodes, BOOL bSyncPostLoad)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentRenderTextureParam param;
				{
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.ptrRenderTexture = RenderTextureManager()->LoadResource(HashValue(pAttachmentNode->ToElement()->AttributeString("render_texture")), TRUE, bSyncPostLoad);
					if (param.ptrRenderTexture.IsNull()) return FALSE;
				}
				m_param.attachmentColors.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResFrameBuffer::LoadAttachmentDepthStencils(TiXmlNode *pAttachmentNodes, BOOL bSyncPostLoad)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentRenderTextureParam param;
				{
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.ptrRenderTexture = RenderTextureManager()->LoadResource(HashValue(pAttachmentNode->ToElement()->AttributeString("render_texture")), TRUE, bSyncPostLoad);
					if (param.ptrRenderTexture.IsNull()) return FALSE;
				}
				m_param.attachmentDepthStencils.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

}
