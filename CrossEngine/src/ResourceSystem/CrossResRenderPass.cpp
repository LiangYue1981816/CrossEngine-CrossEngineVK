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
			for (int indexAttachment = 0; indexAttachment < m_param.attachmentPresents.size(); indexAttachment++) {
				m_ptrRenderPass->SetPresentAttachment(
					m_param.attachmentPresents[indexAttachment].indexAttachment,
					m_param.attachmentPresents[indexAttachment].format,
					m_param.attachmentPresents[indexAttachment].loadOp,
					m_param.attachmentPresents[indexAttachment].storeOp,
					m_param.attachmentPresents[indexAttachment].clearValue,
					m_param.attachmentPresents[indexAttachment].samples);
			}

			for (int indexAttachment = 0; indexAttachment < m_param.attachmentColors.size(); indexAttachment++) {
				m_ptrRenderPass->SetColorAttachment(
					m_param.attachmentColors[indexAttachment].indexAttachment,
					m_param.attachmentColors[indexAttachment].format,
					m_param.attachmentColors[indexAttachment].loadOp,
					m_param.attachmentColors[indexAttachment].storeOp,
					m_param.attachmentColors[indexAttachment].clearValue,
					m_param.attachmentColors[indexAttachment].samples,
					m_param.attachmentColors[indexAttachment].finalLayout);
			}

			for (int indexAttachment = 0; indexAttachment < m_param.attachmentDepthStencils.size(); indexAttachment++) {
				m_ptrRenderPass->SetDepthStencilAttachment(
					m_param.attachmentDepthStencils[indexAttachment].indexAttachment,
					m_param.attachmentDepthStencils[indexAttachment].format,
					m_param.attachmentDepthStencils[indexAttachment].loadOp,
					m_param.attachmentDepthStencils[indexAttachment].storeOp,
					m_param.attachmentDepthStencils[indexAttachment].stencilLoadOp,
					m_param.attachmentDepthStencils[indexAttachment].stencilStoreOp,
					m_param.attachmentDepthStencils[indexAttachment].clearValue,
					m_param.attachmentDepthStencils[indexAttachment].samples,
					m_param.attachmentDepthStencils[indexAttachment].finalLayout);
			}

			for (int indexPass = 0; indexPass < m_param.subpasses.size(); indexPass++) {
				if (m_param.subpasses[indexPass].inputColorReference >= 0) {
					m_ptrRenderPass->SetSubpassInputColorReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].inputColorReference);
				}

				if (m_param.subpasses[indexPass].inputDepthStencilReference >= 0) {
					m_ptrRenderPass->SetSubpassInputDepthStencilReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].inputDepthStencilReference);
				}

				if (m_param.subpasses[indexPass].outputColorReference >= 0) {
					m_ptrRenderPass->SetSubpassOutputColorReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].outputColorReference);
				}

				if (m_param.subpasses[indexPass].outputDepthStencilReference >= 0) {
					m_ptrRenderPass->SetSubpassOutputDepthStencilReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].outputDepthStencilReference);
				}

				if (m_param.subpasses[indexPass].resolveColorReference >= 0) {
					m_ptrRenderPass->SetSubpassResolveColorReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].resolveColorReference, m_param.subpasses[indexPass].resolveColorImageLayout);
				}

				if (m_param.subpasses[indexPass].preserveReference >= 0) {
					m_ptrRenderPass->SetSubpassPreserveReference(m_param.subpasses[indexPass].indexSubPass, m_param.subpasses[indexPass].preserveReference);
				}
			}

			for (int indexDependency = 0; indexDependency < m_param.dependencies.size(); indexDependency++) {
				m_ptrRenderPass->SetSubpassDependency(
					m_param.dependencies[indexDependency].indexDependency,
					m_param.dependencies[indexDependency].indexSrcSubpass,
					m_param.dependencies[indexDependency].indexDstSubpass,
					m_param.dependencies[indexDependency].srcStageMask,
					m_param.dependencies[indexDependency].dstStageMask,
					m_param.dependencies[indexDependency].srcAccessMask,
					m_param.dependencies[indexDependency].dstAccessMask,
					m_param.dependencies[indexDependency].dependencyFlags);
			}
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
				m_param.attachmentPresents.emplace_back(param);
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
				m_param.attachmentColors.emplace_back(param);
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
				m_param.attachmentDepthStencils.emplace_back(param);
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
				m_param.subpasses.emplace_back(param);
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
				m_param.dependencies.emplace_back(param);
			} while (pDependencyNode = pDependencyNodes->IterateChildren("Dependency", pDependencyNode));

			return TRUE;
		}

		return FALSE;
	}

}
