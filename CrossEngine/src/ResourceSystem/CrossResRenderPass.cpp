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
			if (TiXmlNode *pRenderPassNode = xmlDoc.FirstChild("RenderPass")) {
				return LoadRenderPass(pRenderPassNode);
			}
		}

		return FALSE;
	}

	BOOL CResRenderPass::InternalPostLoad(void)
	{
		m_ptrRenderPass = GfxDevice()->NewRenderPass(m_param.attachmentPresents.size() + m_param.attachmentColors.size() + m_param.attachmentDepthStencils.size(), m_param.subpasses.size());
		{
			for (int index = 0; index < m_param.attachmentPresents.size(); index++) {
				m_ptrRenderPass->SetPresentAttachment(
					m_param.attachmentPresents[index].indexAttachment, 
					m_param.attachmentPresents[index].format, 
					m_param.attachmentPresents[index].loadOp, 
					m_param.attachmentPresents[index].storeOp, 
					m_param.attachmentPresents[index].clearValue, 
					m_param.attachmentPresents[index].samples);
			}

			for (int index = 0; index < m_param.attachmentColors.size(); index++) {
				m_ptrRenderPass->SetColorAttachment(
					m_param.attachmentColors[index].indexAttachment, 
					m_param.attachmentColors[index].format, 
					m_param.attachmentColors[index].loadOp, 
					m_param.attachmentColors[index].storeOp, 
					m_param.attachmentColors[index].clearValue, 
					m_param.attachmentColors[index].samples, 
					m_param.attachmentColors[index].finalLayout);
			}

			for (int index = 0; index < m_param.attachmentDepthStencils.size(); index++) {
				m_ptrRenderPass->SetDepthStencilAttachment(
					m_param.attachmentDepthStencils[index].indexAttachment, 
					m_param.attachmentDepthStencils[index].format, 
					m_param.attachmentDepthStencils[index].loadOp, 
					m_param.attachmentDepthStencils[index].storeOp, 
					m_param.attachmentDepthStencils[index].stencilLoadOp, 
					m_param.attachmentDepthStencils[index].stencilStoreOp, 
					m_param.attachmentDepthStencils[index].clearValue, 
					m_param.attachmentDepthStencils[index].samples, 
					m_param.attachmentDepthStencils[index].finalLayout);
			}

			for (int index = 0; index < m_param.subpasses.size(); index++) {
				if (m_param.subpasses[index].inputColorReference >= 0) {
					m_ptrRenderPass->SetSubpassInputColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].inputColorReference);
				}

				if (m_param.subpasses[index].inputDepthStencilReference >= 0) {
					m_ptrRenderPass->SetSubpassInputDepthStencilReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].inputDepthStencilReference);
				}

				if (m_param.subpasses[index].outputColorReference >= 0) {
					m_ptrRenderPass->SetSubpassOutputColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].outputColorReference);
				}

				if (m_param.subpasses[index].outputDepthStencilReference >= 0) {
					m_ptrRenderPass->SetSubpassOutputDepthStencilReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].outputDepthStencilReference);
				}

				if (m_param.subpasses[index].resolveColorReference >= 0) {
					m_ptrRenderPass->SetSubpassResolveColorReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].resolveColorReference, m_param.subpasses[index].resolveColorImageLayout);
				}

				if (m_param.subpasses[index].preserveReference >= 0) {
					m_ptrRenderPass->SetSubpassPreserveReference(m_param.subpasses[index].indexSubPass, m_param.subpasses[index].preserveReference);
				}
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
		}
		return m_ptrRenderPass->Create();
	}

	void CResRenderPass::InternalLoadFail(void)
	{
		m_ptrRenderPass.Release();

		m_param.attachmentPresents.clear();
		m_param.attachmentColors.clear();
		m_param.attachmentDepthStencils.clear();
		m_param.subpasses.clear();
		m_param.dependencies.clear();

		CResource::InternalLoadFail();
	}

	void CResRenderPass::InternalLoadSuccess(void)
	{
		m_param.attachmentPresents.clear();
		m_param.attachmentColors.clear();
		m_param.attachmentDepthStencils.clear();
		m_param.subpasses.clear();
		m_param.dependencies.clear();

		CResource::InternalLoadSuccess();
	}

	BOOL CResRenderPass::LoadRenderPass(TiXmlNode *pRenderPassNode)
	{
		BOOL rcode = FALSE;

		if (TiXmlNode *pSubPassNodes = pRenderPassNode->FirstChild("SubPasses")) {
			if (LoadSubPasses(pSubPassNodes) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pDependencyNodes = pRenderPassNode->FirstChild("Dependencies")) {
			if (LoadDependencies(pDependencyNodes) == FALSE) {
				return FALSE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pRenderPassNode->FirstChild("Presents")) {
			if (LoadAttachmentPresents(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pRenderPassNode->FirstChild("Colors")) {
			if (LoadAttachmentColors(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		if (TiXmlNode *pAttachmentNodes = pRenderPassNode->FirstChild("DepthStencils")) {
			if (LoadAttachmentDepthStencils(pAttachmentNodes)) {
				rcode = TRUE;
			}
		}

		return rcode;
	}

	BOOL CResRenderPass::LoadAttachmentPresents(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentPresentParam param;
				{
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));
					param.loadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("loadop"));
					param.storeOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("storeop"));
					param.samples = CVulkanHelper::StringToSampleCountFlagBits(pAttachmentNode->ToElement()->AttributeString("samples"));
					pAttachmentNode->ToElement()->AttributeFloat4("clear_color", param.clearValue.color.float32);
				}
				m_param.attachmentPresents.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadAttachmentColors(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentColorTextureParam param;
				{
					param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
					param.format = CVulkanHelper::StringToFormat(pAttachmentNode->ToElement()->AttributeString("format"));
					param.loadOp = CVulkanHelper::StringToAttachmentLoadOp(pAttachmentNode->ToElement()->AttributeString("loadop"));
					param.storeOp = CVulkanHelper::StringToAttachmentStoreOp(pAttachmentNode->ToElement()->AttributeString("storeop"));
					param.samples = CVulkanHelper::StringToSampleCountFlagBits(pAttachmentNode->ToElement()->AttributeString("samples"));
					param.finalLayout = CVulkanHelper::StringToImageLayout(pAttachmentNode->ToElement()->AttributeString("layout"));
					pAttachmentNode->ToElement()->AttributeFloat4("clear_color", param.clearValue.color.float32);
				}
				m_param.attachmentColors.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadAttachmentDepthStencils(TiXmlNode *pAttachmentNodes)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentNodes->FirstChild("Attachment")) {
			do {
				AttachmentDepthStencilTextureParam param;
				{
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
				}
				m_param.attachmentDepthStencils.push_back(param);
			} while (pAttachmentNode = pAttachmentNodes->IterateChildren("Attachment", pAttachmentNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadSubPasses(TiXmlNode *pSubPassNodes)
	{
		if (TiXmlNode *pSubPassNode = pSubPassNodes->FirstChild("SubPass")) {
			do {
				SubPassParam param;
				{
					param.indexSubPass = pSubPassNode->ToElement()->AttributeInt1("index");
					param.inputColorReference = pSubPassNode->ToElement()->AttributeInt1("input_color_reference");
					param.inputDepthStencilReference = pSubPassNode->ToElement()->AttributeInt1("input_depth_stencil_reference");
					param.outputColorReference = pSubPassNode->ToElement()->AttributeInt1("output_color_reference");
					param.outputDepthStencilReference = pSubPassNode->ToElement()->AttributeInt1("output_depth_stencil_reference");
					param.preserveReference = pSubPassNode->ToElement()->AttributeInt1("preserve_reference");
					param.resolveColorReference = pSubPassNode->ToElement()->AttributeInt1("resolve_color_reference");
					param.resolveColorImageLayout = CVulkanHelper::StringToImageLayout(pSubPassNode->ToElement()->AttributeString("resolve_color_image_layout"));
				}
				m_param.subpasses.push_back(param);
			} while (pSubPassNode = pSubPassNodes->IterateChildren("SubPass", pSubPassNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResRenderPass::LoadDependencies(TiXmlNode *pDependencyNodes)
	{
		if (TiXmlNode *pDependencyNode = pDependencyNodes->FirstChild("Dependency")) {
			do {
				DependencyParam param;
				{
					param.indexDependency = pDependencyNode->ToElement()->AttributeInt1("index");
					param.indexSrcSubpass = pDependencyNode->ToElement()->AttributeInt1("index_src_subpass");
					param.indexDstSubpass = pDependencyNode->ToElement()->AttributeInt1("index_dst_subpass");
					param.srcStageMask = CVulkanHelper::StringToPipelineStageFlags(pDependencyNode->ToElement()->AttributeString("src_stage_mask"));
					param.dstStageMask = CVulkanHelper::StringToPipelineStageFlags(pDependencyNode->ToElement()->AttributeString("dst_stage_mask"));
					param.srcAccessMask = CVulkanHelper::StringToAccessFlags(pDependencyNode->ToElement()->AttributeString("src_access_mask"));
					param.dstAccessMask = CVulkanHelper::StringToAccessFlags(pDependencyNode->ToElement()->AttributeString("dst_access_mask"));
					param.dependencyFlags = CVulkanHelper::StringToDependencyFlags(pDependencyNode->ToElement()->AttributeString("flags"));
				}
				m_param.dependencies.push_back(param);
			} while (pDependencyNode = pDependencyNodes->IterateChildren("Dependency", pDependencyNode));

			return TRUE;
		}

		return FALSE;
	}

}
