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

	CResGraphics::CResGraphics(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResGraphics::~CResGraphics(void)
	{

	}

	RESOURCE_TYPE CResGraphics::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_GRAPHICS;
	}

	const CGfxRenderPassPtr& CResGraphics::GetRenderPass(void) const
	{
		return m_ptrRenderPass->GetRenderPass();
	}

	const CGfxPipelineGraphicsPtr& CResGraphics::GetPipeline(void) const
	{
		return m_ptrPipeline;
	}

	BOOL CResGraphics::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrPipeline.IsNull() || m_ptrPipeline->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResGraphics::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pShaderNode = xmlDoc.FirstChild("Shader")) {
				if (LoadShader(pShaderNode) == FALSE) {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}

			if (TiXmlNode *pRenderPassNode = xmlDoc.FirstChild("RenderPass")) {
				if (LoadRenderPass(pRenderPassNode) == FALSE) {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}

			if (TiXmlNode *pAssemblyNode = xmlDoc.FirstChild("Assembly")) {
				if (LoadAssembly(pAssemblyNode) == FALSE) {
					return FALSE;
				}
			}

			if (TiXmlNode *pRasterizationNode = xmlDoc.FirstChild("Rasterization")) {
				if (LoadRasterization(pRasterizationNode) == FALSE) {
					return FALSE;
				}
			}

			if (TiXmlNode *pMultisampleNode = xmlDoc.FirstChild("Multisample")) {
				if (LoadMultisample(pMultisampleNode) == FALSE) {
					return FALSE;
				}
			}

			if (TiXmlNode *pDepthNode = xmlDoc.FirstChild("Depth")) {
				if (LoadDepth(pDepthNode) == FALSE) {
					return FALSE;
				}
			}

			if (TiXmlNode *pStencilNode = xmlDoc.FirstChild("Stencil")) {
				if (LoadStencil(pStencilNode) == FALSE) {
					return FALSE;
				}
			}

			if (TiXmlNode *pBlendNodes = xmlDoc.FirstChild("Blend")) {
				if (LoadBlend(pBlendNodes) == FALSE) {
					return FALSE;
				}
			}

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResGraphics::InternalPostLoad(void)
	{
		m_ptrRenderPass = RenderPassManager()->LoadResource(m_param.renderpass.dwName);
		if (m_ptrRenderPass.IsNull() || m_ptrRenderPass->IsValid() == FALSE) return FALSE;

		m_ptrPipeline = GfxDevice()->NewPipelineGraphics(m_ptrRenderPass->GetRenderPass()->GetSubpassOutputAttachmentCount(m_param.renderpass.indexSubPass));
		{
			if (SetShaders() == FALSE) return FALSE;
			if (SetAssemblyState() == FALSE) return FALSE;
			if (SetTessellationState() == FALSE) return FALSE;
			if (SetRasterizationState() == FALSE) return FALSE;
			if (SetMultisampleState() == FALSE) return FALSE;
			if (SetDepthStencilState() == FALSE) return FALSE;
			if (SetColorBlendState() == FALSE) return FALSE;
		}
		return m_ptrPipeline->Create(m_ptrRenderPass->GetRenderPass()->GetHandle(), m_param.renderpass.indexSubPass);
	}

	void CResGraphics::InternalLoadFail(void)
	{
		m_ptrPipeline.Release();
		m_ptrRenderPass.Release();
		CResource::InternalLoadFail();
	}

	void CResGraphics::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResGraphics::LoadShader(TiXmlNode *pShaderNode)
	{
		m_param.shader.dwVertexName = HashValue(pShaderNode->ToElement()->AttributeString("vertex"));
		m_param.shader.dwFragmentName = HashValue(pShaderNode->ToElement()->AttributeString("fragment"));
		return TRUE;
	}

	BOOL CResGraphics::LoadRenderPass(TiXmlNode *pRenderPassNode)
	{
		m_param.renderpass.dwName = HashValue(pRenderPassNode->ToElement()->AttributeString("renderpass"));
		m_param.renderpass.indexSubPass = pRenderPassNode->ToElement()->AttributeInt1("subpass");
		return TRUE;
	}

	BOOL CResGraphics::LoadAssembly(TiXmlNode *pAssemblyNode)
	{
		m_param.assembly.topology = CVulkanHelper::StringToPrimitiveTopology(pAssemblyNode->ToElement()->AttributeString("topology"));
		return TRUE;
	}

	BOOL CResGraphics::LoadRasterization(TiXmlNode *pRasterizationNode)
	{
		m_param.rasterization.cullMode = CVulkanHelper::StringToCullModeFlags(pRasterizationNode->ToElement()->AttributeString("cull_mode"));
		m_param.rasterization.polygonMode = CVulkanHelper::StringToPolygonMode(pRasterizationNode->ToElement()->AttributeString("polygon_mode"));
		m_param.rasterization.bDepthBiasEnable = pRasterizationNode->ToElement()->AttributeBool("enable_depth_bias");
		m_param.rasterization.depthBiasSlopeFactor = pRasterizationNode->ToElement()->AttributeBool("depth_bias_slope_factor");
		m_param.rasterization.depthBiasConstantFactor = pRasterizationNode->ToElement()->AttributeBool("depth_bias_constant_factor");
		return TRUE;
	}

	BOOL CResGraphics::LoadMultisample(TiXmlNode *pMultisampleNode)
	{
		m_param.multisample.samples = CVulkanHelper::StringToSampleCountFlagBits(pMultisampleNode->ToElement()->AttributeString("samples"));
		m_param.multisample.bAlphaToCoverageEnable = pMultisampleNode->ToElement()->AttributeBool("enable_alpha_to_Coverage");
		m_param.multisample.bAlphaToOneEnable = pMultisampleNode->ToElement()->AttributeBool("enable_alpha_to_one");
		return TRUE;
	}

	BOOL CResGraphics::LoadDepth(TiXmlNode *pDepthNode)
	{
		m_param.depth.bDepthTestEnable = pDepthNode->ToElement()->AttributeBool("enable_depth_test");
		m_param.depth.bDepthWriteEnable = pDepthNode->ToElement()->AttributeBool("enable_depth_write");
		m_param.depth.depthCompareOp = CVulkanHelper::StringToCompareOp(pDepthNode->ToElement()->AttributeString("depth_compare_op"));
		return TRUE;
	}

	BOOL CResGraphics::LoadStencil(TiXmlNode *pStencilNode)
	{
		m_param.stencil.bStencilTestEnable = pStencilNode->ToElement()->AttributeBool("enable_stencil_test");
		m_param.stencil.frontPassOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("front_pass_op"));
		m_param.stencil.frontFailOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("front_fail_op"));
		m_param.stencil.frontDepthFailOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("front_depth_fail_op"));
		m_param.stencil.frontCompareOp = CVulkanHelper::StringToCompareOp(pStencilNode->ToElement()->AttributeString("front_compare_op"));
		m_param.stencil.frontCompareMask = pStencilNode->ToElement()->AttributeInt1("front_compare_mask");
		m_param.stencil.frontWriteMask = pStencilNode->ToElement()->AttributeInt1("front_write_mask");
		m_param.stencil.frontReference = pStencilNode->ToElement()->AttributeInt1("front_reference");
		m_param.stencil.backPassOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("back_pass_op"));
		m_param.stencil.backFailOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("back_fail_op"));
		m_param.stencil.backDepthFailOp = CVulkanHelper::StringToStencilOp(pStencilNode->ToElement()->AttributeString("back_depth_fail_op"));
		m_param.stencil.backCompareOp = CVulkanHelper::StringToCompareOp(pStencilNode->ToElement()->AttributeString("back_compare_op"));
		m_param.stencil.backCompareMask = pStencilNode->ToElement()->AttributeInt1("back_compare_mask");
		m_param.stencil.backWriteMask = pStencilNode->ToElement()->AttributeInt1("back_write_mask");
		m_param.stencil.backReference = pStencilNode->ToElement()->AttributeInt1("back_reference");
		return TRUE;
	}

	BOOL CResGraphics::LoadBlend(TiXmlNode *pBlendNodes)
	{
		if (TiXmlNode *pBlendNode = pBlendNodes->FirstChild("Attachment")) {
			do {
				int indexAttachment = pBlendNode->ToElement()->AttributeInt1("attachment");
				m_param.blends[indexAttachment].bBlendEnable = pBlendNode->ToElement()->AttributeBool("enable");
				m_param.blends[indexAttachment].srcColorBlendFactor = CVulkanHelper::StringToBlendFactor(pBlendNode->ToElement()->AttributeString("src_color_blend_factor"));
				m_param.blends[indexAttachment].dstColorBlendFactor = CVulkanHelper::StringToBlendFactor(pBlendNode->ToElement()->AttributeString("dst_color_blend_factor"));
				m_param.blends[indexAttachment].colorBlendOp = CVulkanHelper::StringToBlendOp(pBlendNode->ToElement()->AttributeString("color_blend_op"));
				m_param.blends[indexAttachment].srcAlphaBlendFactor = CVulkanHelper::StringToBlendFactor(pBlendNode->ToElement()->AttributeString("src_alpha_blend_factor"));
				m_param.blends[indexAttachment].dstAlphaBlendFactor = CVulkanHelper::StringToBlendFactor(pBlendNode->ToElement()->AttributeString("dst_alpha_blend_factor"));
				m_param.blends[indexAttachment].alphaBlendOp = CVulkanHelper::StringToBlendOp(pBlendNode->ToElement()->AttributeString("alpha_blend_op"));
			} while (pBlendNode = pBlendNode->IterateChildren("Attachment", pBlendNode));
		}
		return TRUE;
	}

	BOOL CResGraphics::SetShaders(void)
	{
		const CResShaderPtr &ptrVertexShader = ShaderManager()->LoadResource(m_param.shader.dwVertexName);
		if (ptrVertexShader.IsNull() || ptrVertexShader->IsValid() == FALSE) return FALSE;

		const CResShaderPtr &ptrFragmentShader = ShaderManager()->LoadResource(m_param.shader.dwFragmentName);
		if (ptrFragmentShader.IsNull() || ptrFragmentShader->IsValid() == FALSE) return FALSE;

		m_ptrPipeline->SetVertexShader(ptrVertexShader->GetShader());
		m_ptrPipeline->SetFragmentShader(ptrFragmentShader->GetShader());

		return TRUE;
	}

	BOOL CResGraphics::SetAssemblyState(void)
	{
		m_ptrPipeline->SetPrimitiveTopology(m_param.assembly.topology, FALSE);
		return TRUE;
	}

	BOOL CResGraphics::SetTessellationState(void)
	{
		return TRUE;
	}

	BOOL CResGraphics::SetRasterizationState(void)
	{
		m_ptrPipeline->SetPolygonMode(m_param.rasterization.polygonMode);
		m_ptrPipeline->SetCullMode(m_param.rasterization.cullMode);
		m_ptrPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
		m_ptrPipeline->SetDepthClamp(FALSE);
		m_ptrPipeline->SetDepthBias(m_param.rasterization.bDepthBiasEnable, m_param.rasterization.depthBiasConstantFactor, 0.0f, m_param.rasterization.depthBiasSlopeFactor);
		m_ptrPipeline->SetRasterizerDiscard(FALSE);
		return TRUE;
	}

	BOOL CResGraphics::SetMultisampleState(void)
	{
		m_ptrPipeline->SetSampleCounts(m_param.multisample.samples);
		m_ptrPipeline->SetSampleShading(FALSE, 0.0f);
		m_ptrPipeline->SetSampleAlphaToCoverage(m_param.multisample.bAlphaToCoverageEnable);
		m_ptrPipeline->SetSampleAlphaToOne(m_param.multisample.bAlphaToOneEnable);
		return TRUE;
	}

	BOOL CResGraphics::SetDepthStencilState(void)
	{
		m_ptrPipeline->SetDepthBoundsTest(FALSE, 0.0f, 0.0f);
		m_ptrPipeline->SetDepthTest(m_param.depth.bDepthTestEnable, m_param.depth.bDepthWriteEnable, m_param.depth.depthCompareOp);
		m_ptrPipeline->SetStencilTest(m_param.stencil.bStencilTestEnable, m_param.stencil.frontFailOp, m_param.stencil.frontPassOp, m_param.stencil.frontDepthFailOp, m_param.stencil.frontCompareOp, m_param.stencil.frontCompareMask, m_param.stencil.frontWriteMask, m_param.stencil.frontReference, m_param.stencil.backFailOp, m_param.stencil.backPassOp, m_param.stencil.backDepthFailOp, m_param.stencil.backCompareOp, m_param.stencil.backCompareMask, m_param.stencil.backWriteMask, m_param.stencil.backReference);
		return TRUE;
	}

	BOOL CResGraphics::SetColorBlendState(void)
	{
		m_ptrPipeline->SetColorBlendLogic(FALSE, VK_LOGIC_OP_CLEAR);
		m_ptrPipeline->SetColorBlendConstants(0.0f, 0.0f, 0.0f, 0.0f);

		for (std::map<uint32_t, BlendParam>::const_iterator itColorBlendAttachment = m_param.blends.begin(); itColorBlendAttachment != m_param.blends.end(); ++itColorBlendAttachment) {
			m_ptrPipeline->SetColorBlendAttachment(itColorBlendAttachment->first, itColorBlendAttachment->second.bBlendEnable, itColorBlendAttachment->second.srcColorBlendFactor, itColorBlendAttachment->second.dstColorBlendFactor, itColorBlendAttachment->second.colorBlendOp, itColorBlendAttachment->second.srcAlphaBlendFactor, itColorBlendAttachment->second.dstAlphaBlendFactor, itColorBlendAttachment->second.alphaBlendOp, (VkColorComponentFlags)itColorBlendAttachment->second.colorWriteMask);
		}

		return TRUE;
	}

}
