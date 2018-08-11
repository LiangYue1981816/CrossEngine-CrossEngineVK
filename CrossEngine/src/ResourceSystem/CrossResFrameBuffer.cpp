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
		m_param.attachmentPresent.indexAttachment = -1;
		m_param.attachmentPresent.indexSwapchainImage = -1;

		m_param.attachmentDepthStencil.indexAttachment = -1;
		m_param.attachmentDepthStencil.format = VK_FORMAT_D24_UNORM_S8_UINT;
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
			for (int indexAttachment = 0; indexAttachment < m_param.attachmentColors.size(); indexAttachment++) {
				if (m_param.attachmentColors[indexAttachment].ptrRenderTexture->IsValid() == FALSE) {
					return FALSE;
				}

				if (m_ptrFrameBuffer->SetColorAttachment(m_param.attachmentColors[indexAttachment].indexAttachment, m_param.attachmentColors[indexAttachment].ptrRenderTexture->GetRenderTexture()) == FALSE) {
					return FALSE;
				}
			}

			if (m_param.attachmentPresent.indexAttachment >= 0 &&
				m_param.attachmentPresent.indexSwapchainImage >= 0 && m_param.attachmentPresent.indexSwapchainImage < GfxSwapChain()->GetImageCount()) {
				if (m_ptrFrameBuffer->SetPresentAttachment(m_param.attachmentPresent.indexAttachment, GfxSwapChain()->GetFormat(), GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), GfxSwapChain()->GetImageHandle(m_param.attachmentPresent.indexSwapchainImage)) == FALSE) {
					return FALSE;
				}
			}

			if (m_param.attachmentDepthStencil.indexAttachment >= 0) {
				m_ptrDepthStencilTexture = GfxDevice()->NewRenderTexture();

				if (m_ptrDepthStencilTexture->CreateDepthStencilTarget(m_param.attachmentDepthStencil.format, m_ptrFrameBuffer->GetWidth(), m_ptrFrameBuffer->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT) == FALSE) {
					return FALSE;
				}

				if (m_ptrFrameBuffer->SetDepthStencilAttachment(1, m_ptrDepthStencilTexture) == FALSE) {
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
		m_ptrDepthStencilTexture.Release();
		CResource::InternalLoadFail();
	}

	void CResFrameBuffer::InternalLoadSuccess(void)
	{
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

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("Present")) {
			if (LoadAttachmentPresent(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("DepthStencil")) {
			if (LoadAttachmentDepthStencil(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pFrameBufferNode->FirstChild("Colors")) {
			if (LoadAttachmentColors(pAttachmentNodes, bSyncPostLoad)) {
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

	BOOL CResFrameBuffer::LoadAttachmentPresent(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			m_param.attachmentPresent.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
			m_param.attachmentPresent.indexSwapchainImage = pAttachmentNode->ToElement()->AttributeInt1("index_swapchain_image");

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResFrameBuffer::LoadAttachmentDepthStencil(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			m_param.attachmentDepthStencil.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
			m_param.attachmentDepthStencil.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));

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
				m_param.attachmentColors.emplace_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

}
