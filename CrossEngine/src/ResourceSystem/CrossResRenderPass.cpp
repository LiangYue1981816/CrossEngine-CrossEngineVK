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
		return RESOURCE_TYPE::RESOURCE_TYPE_RENDER_PASS;
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

			if (TiXmlNode *pDependenciesNode = xmlDoc.FirstChild("Dependencies")) {
				if (LoadDependencies(pDependenciesNode) == FALSE) {
					return FALSE;
				}
			}

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::InternalPostLoad(void)
	{
		m_ptrRenderPass = GfxDevice()->NewRenderPass(m_param.attachments.size(), m_param.subpasses.size());

		for (int index = 0; index < m_param.attachments.size(); index++) {
			switch (m_param.attachments[index].type) {
			case ATTACHMENT_TYPE_PRESENT:
				m_ptrRenderPass->SetPresentAttachment(
					m_param.attachments[index].indexAttachment, 
					m_param.attachments[index].format, 
					m_param.attachments[index].loadOp, 
					m_param.attachments[index].storeOp, 
					m_param.attachments[index].clearValue, 
					m_param.attachments[index].samples);
				break;

			case ATTACHMENT_TYPE_COLOR:
				m_ptrRenderPass->SetColorAttachment(
					m_param.attachments[index].indexAttachment, 
					m_param.attachments[index].format, 
					m_param.attachments[index].loadOp, 
					m_param.attachments[index].storeOp, 
					m_param.attachments[index].clearValue, 
					m_param.attachments[index].samples, 
					m_param.attachments[index].finalLayout);
				break;

			case ATTACHMENT_TYPE_DEPTHSTENCIL:
				m_ptrRenderPass->SetDepthStencilAttachment(
					m_param.attachments[index].indexAttachment, 
					m_param.attachments[index].format, 
					m_param.attachments[index].loadOp, 
					m_param.attachments[index].storeOp, 
					m_param.attachments[index].stencilLoadOp, 
					m_param.attachments[index].stencilStoreOp, 
					m_param.attachments[index].clearValue, 
					m_param.attachments[index].samples, 
					m_param.attachments[index].finalLayout);
				break;
			}
		}

		for (int index = 0; index < m_param.subpasses.size(); index++) {
			m_ptrRenderPass->SetSubpassInputColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].inputColorReference);
			m_ptrRenderPass->SetSubpassInputDepthStencilReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].inputDepthStencilReference);
			m_ptrRenderPass->SetSubpassOutputColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].outputColorReference);
			m_ptrRenderPass->SetSubpassOutputDepthStencilReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].outputDepthStencilReference);
			m_ptrRenderPass->SetSubpassResolveColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].resolveColorReference, m_param.subpasses[index].resolveColorImageLayout);
			m_ptrRenderPass->SetSubpassPreserveReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].preserveReference);
		}

		for (int index = 0; index < m_param.dependencies.size(); index++) {
			m_ptrRenderPass->SetSubpassDependency(
				m_param.dependencies[index].indexDependency,
				m_param.dependencies[index].indexSrcSubpass,
				m_param.dependencies[index].indexDstSubpass,
				m_param.dependencies[index].srcStageMask,
				m_param.dependencies[index].dstStageMask,
				m_param.dependencies[index].srcAccessMask,
				m_param.dependencies[index].dstAccessMask,
				m_param.dependencies[index].dependencyFlags);
		}
		
		return m_ptrRenderPass->Create();
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
				SubPassParam param;

				param.indexSubPass = pSubPassNode->ToElement()->AttributeInt1("index");
				param.inputColorReference = pSubPassesNode->ToElement()->AttributeInt1("input_color_reference");
				param.inputDepthStencilReference = pSubPassesNode->ToElement()->AttributeInt1("input_depth_stencil_reference");
				param.outputColorReference = pSubPassesNode->ToElement()->AttributeInt1("output_color_reference");
				param.outputDepthStencilReference = pSubPassesNode->ToElement()->AttributeInt1("output_depth_stencil_reference");
				param.preserveReference = pSubPassesNode->ToElement()->AttributeInt1("preserve_reference");
				param.resolveColorReference = pSubPassesNode->ToElement()->AttributeInt1("resolve_color_reference");
				param.resolveColorImageLayout = CVulkanHelper::StringToImageLayout(pSubPassesNode->ToElement()->AttributeString("resolve_color_image_layout"));

				m_param.subpasses.push_back(param);
			} while (pSubPassNode = pSubPassNode->IterateChildren("SubPass", pSubPassNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadDependencies(TiXmlNode *pDependenciesNode)
	{
		if (TiXmlNode *pDependencyNode = pDependenciesNode->FirstChild("Dependency")) {
			do {
				DependencyParam param;

				param.indexDependency = pDependencyNode->ToElement()->AttributeInt1("index");
				param.indexSrcSubpass = pDependencyNode->ToElement()->AttributeInt1("index_src_subpass");
				param.indexDstSubpass = pDependencyNode->ToElement()->AttributeInt1("index_dst_subpass");
				param.srcStageMask = CVulkanHelper::StringToPipelineStageFlags(pDependencyNode->ToElement()->AttributeString("src_stage_mask"));
				param.dstStageMask = CVulkanHelper::StringToPipelineStageFlags(pDependencyNode->ToElement()->AttributeString("dst_stage_mask"));
				param.srcAccessMask = CVulkanHelper::StringToAccessFlags(pDependencyNode->ToElement()->AttributeString("src_access_mask"));
				param.dstAccessMask = CVulkanHelper::StringToAccessFlags(pDependencyNode->ToElement()->AttributeString("dst_access_mask"));
				param.dependencyFlags = CVulkanHelper::StringToDependencyFlags(pDependencyNode->ToElement()->AttributeString("flags"));

				m_param.dependencies.push_back(param);
			} while (pDependencyNode = pDependencyNode->IterateChildren("Dependency", pDependencyNode));

			return TRUE;
		}

		return FALSE;
	}

}
