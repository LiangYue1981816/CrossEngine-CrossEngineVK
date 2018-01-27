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

	CResRenderPass::CResRenderPass(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResRenderPass::~CResRenderPass(void)
	{

	}

	RESOURCE_TYPE CResRenderPass::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_RENDERPASS;
	}

	const CGfxRenderPassPtr& CResRenderPass::GetRenderPass(void) const
	{
		return m_ptrRenderPass;
	}

	BOOL CResRenderPass::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrRenderPass.IsNull() || m_ptrRenderPass->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResRenderPass::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pAttachmentsNode = xmlDoc.FirstChild("Attachments")) {
				if (LoadAttachments(pAttachmentsNode) == FALSE) {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}

			if (TiXmlNode *pSubPassesNode = xmlDoc.FirstChild("SubPasses")) {
				if (LoadSubPasses(pSubPassesNode) == FALSE) {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::InternalPostLoad(void)
	{
		return TRUE;
	}

	void CResRenderPass::InternalLoadFail(void)
	{
		m_ptrRenderPass.Release();
		CResource::InternalLoadFail();
	}

	void CResRenderPass::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResRenderPass::LoadAttachments(TiXmlNode *pAttachmentsNode)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentsNode->FirstChild("Attachment")) {
			do {
				AttachmentParam param;

				const char *szType = pAttachmentNode->ToElement()->AttributeString("type");
				if (stricmp(szType, "present") == 0) {
					param.type = ATTACHMENT_TYPE_PRESENT;
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));
					param.loadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("loadop"));
					param.storeOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("storeop"));
					param.samples = CVulkanHelper::StringToSampleCountFlagBits(pAttachmentNode->ToElement()->AttributeString("samples"));
					param.clearValue.depthStencil.depth = pAttachmentNode->ToElement()->AttributeFloat1("clear_depth");
					param.clearValue.depthStencil.stencil = pAttachmentNode->ToElement()->AttributeInt1("clear_stencil");
					pAttachmentNode->ToElement()->AttributeFloat4("clear_color", param.clearValue.color.float32);
				}
				else if (stricmp(szType, "color") == 0) {
					param.type = ATTACHMENT_TYPE_COLOR;
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));
					param.loadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("loadop"));
					param.storeOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("storeop"));
					param.samples = CVulkanHelper::StringToSampleCountFlagBits(pAttachmentNode->ToElement()->AttributeString("samples"));
					param.finalLayout = CVulkanHelper::StringToImageLayout(pAttachmentNode->ToElement()->AttributeString("layout"));
					param.clearValue.depthStencil.depth = pAttachmentNode->ToElement()->AttributeFloat1("clear_depth");
					param.clearValue.depthStencil.stencil = pAttachmentNode->ToElement()->AttributeInt1("clear_stencil");
					pAttachmentNode->ToElement()->AttributeFloat4("clear_color", param.clearValue.color.float32);
				}
				else if (stricmp(szType, "depthstencil") == 0) {
					param.type = ATTACHMENT_TYPE_DEPTHSTENCIL;
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));
					param.loadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("loadop"));
					param.storeOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("storeop"));
					param.stencilLoadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("stencil_loadop"));
					param.stencilStoreOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("stencil_storeop"));
					param.samples = CVulkanHelper::StringToSampleCountFlagBits(pAttachmentNode->ToElement()->AttributeString("samples"));
					param.finalLayout = CVulkanHelper::StringToImageLayout(pAttachmentNode->ToElement()->AttributeString("layout"));
					param.clearValue.depthStencil.depth = pAttachmentNode->ToElement()->AttributeFloat1("clear_depth");
					param.clearValue.depthStencil.stencil = pAttachmentNode->ToElement()->AttributeInt1("clear_stencil");
					pAttachmentNode->ToElement()->AttributeFloat4("clear_color", param.clearValue.color.float32);
				}
				else {
					return FALSE;
				}

				m_param.attachments.push_back(param);
			} while (pAttachmentNode = pAttachmentNode->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadSubPasses(TiXmlNode *pSubPassesNode)
	{
		if (TiXmlNode *pSubPassNode = pSubPassesNode->FirstChild("SubPass")) {
			do {

			} while (pSubPassNode = pSubPassNode->IterateChildren("SubPass", pSubPassNode));

			return TRUE;
		}

		return FALSE;
	}

}
