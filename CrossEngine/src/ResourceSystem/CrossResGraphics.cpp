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

	const uint32_t CResGraphics::GetSubpassIndex(void) const
	{
		return m_param.renderpass.indexSubPass;
	}

	const CGfxRenderPassPtr& CResGraphics::GetRenderPass(void) const
	{
		return m_ptrRenderPass->GetRenderPass();
	}

	const CGfxPipelineGraphicsPtr& CResGraphics::GetPipeline(void) const
	{
		return m_ptrPipelineGraphics;
	}

	BOOL CResGraphics::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrPipelineGraphics.IsNull() || m_ptrPipelineGraphics->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResGraphics::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;

		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pGraphicsNode = xmlDoc.FirstChild("Graphics")) {
				return LoadGraphics(pGraphicsNode);
			}
		}

		return FALSE;
	}

	BOOL CResGraphics::InternalPostLoad(void)
	{
		m_ptrRenderPass = RenderPassManager()->LoadResource(m_param.renderpass.dwName);
		if (m_ptrRenderPass.IsNull() || m_ptrRenderPass->IsValid() == FALSE) return FALSE;

		m_ptrPipelineGraphics = GfxDevice()->NewPipelineGraphics(m_ptrRenderPass->GetRenderPass()->GetSubpassOutputAttachmentCount(m_param.renderpass.indexSubPass));
		{
			if (SetShaders() == FALSE) return FALSE;
			if (SetAssemblyState() == FALSE) return FALSE;
			if (SetTessellationState() == FALSE) return FALSE;
			if (SetRasterizationState() == FALSE) return FALSE;
			if (SetMultisampleState() == FALSE) return FALSE;
			if (SetDepthStencilState() == FALSE) return FALSE;
			if (SetColorBlendState() == FALSE) return FALSE;
		}
		return m_ptrPipelineGraphics->Create(m_ptrRenderPass->GetRenderPass()->GetHandle(), m_param.renderpass.indexSubPass);
	}

	void CResGraphics::InternalLoadFail(void)
	{
		m_ptrPipelineGraphics.Release();
		m_ptrRenderPass.Release();
		CResource::InternalLoadFail();
	}

	void CResGraphics::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResGraphics::LoadGraphics(TiXmlNode *pGraphicsNode)
	{
		if (TiXmlNode *pRenderPassNode = pGraphicsNode->FirstChild("RenderPass")) {
			if (LoadRenderPass(pRenderPassNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pShaderNode = pGraphicsNode->FirstChild("Shader")) {
			if (LoadShader(pShaderNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pAssemblyNode = pGraphicsNode->FirstChild("Assembly")) {
			if (LoadAssembly(pAssemblyNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pRasterizationNode = pGraphicsNode->FirstChild("Rasterization")) {
			if (LoadRasterization(pRasterizationNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pMultisampleNode = pGraphicsNode->FirstChild("Multisample")) {
			if (LoadMultisample(pMultisampleNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pDepthNode = pGraphicsNode->FirstChild("Depth")) {
			if (LoadDepth(pDepthNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pStencilNode = pGraphicsNode->FirstChild("Stencil")) {
			if (LoadStencil(pStencilNode) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		if (TiXmlNode *pBlendNodes = pGraphicsNode->FirstChild("Blend")) {
			if (LoadBlend(pBlendNodes) == FALSE) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResGraphics::LoadRenderPass(TiXmlNode *pRenderPassNode)
	{
		m_param.renderpass.dwName = HashValue(pRenderPassNode->ToElement()->AttributeString("name"));
		m_param.renderpass.indexSubPass = pRenderPassNode->ToElement()->AttributeInt1("subpass");
		return TRUE;
	}

	BOOL CResGraphics::LoadShader(TiXmlNode *pShaderNode)
	{
		m_param.shader.dwVertexName = HashValue(pShaderNode->ToElement()->AttributeString("vertex"));
		m_param.shader.dwFragmentName = HashValue(pShaderNode->ToElement()->AttributeString("fragment"));
		return TRUE;
	}

	BOOL CResGraphics::LoadAssembly(TiXmlNode *pAssemblyNode)
	{
		m_param.assembly.topology = CVulkanHelper::StringToPrimitiveTopology(pAssemblyNode->ToElement()->AttributeString("topology"));
		m_param.assembly.bPrimitiveRestartEnable = pAssemblyNode->ToElement()->AttributeBool("enable_primitive_restart");
		return TRUE;
	}

	BOOL CResGraphics::LoadRasterization(TiXmlNode *pRasterizationNode)
	{
		m_param.rasterization.polygonMode = CVulkanHelper::StringToPolygonMode(pRasterizationNode->ToElement()->AttributeString("polygon_mode"));
		m_param.rasterization.cullMode = CVulkanHelper::StringToCullModeFlags(pRasterizationNode->ToElement()->AttributeString("cull_mode"));
		m_param.rasterization.frontFace = CVulkanHelper::StringToFrontFace(pRasterizationNode->ToElement()->AttributeString("front_face"));
		m_param.rasterization.bDepthClampEnable = pRasterizationNode->ToElement()->AttributeBool("enable_depth_clamp");
		m_param.rasterization.bDepthBiasEnable = pRasterizationNode->ToElement()->AttributeBool("enable_depth_bias");
		m_param.rasterization.depthBiasClamp = pRasterizationNode->ToElement()->AttributeBool("depth_bias_clamp");
		m_param.rasterization.depthBiasSlopeFactor = pRasterizationNode->ToElement()->AttributeFloat1("depth_bias_slope_factor");
		m_param.rasterization.depthBiasConstantFactor = pRasterizationNode->ToElement()->AttributeFloat1("depth_bias_constant_factor");
		m_param.rasterization.bRasterizerDiscardEnable = pRasterizationNode->ToElement()->AttributeFloat1("enable_rasterizer_discard");
		return TRUE;
	}

	BOOL CResGraphics::LoadMultisample(TiXmlNode *pMultisampleNode)
	{
		m_param.multisample.samples = CVulkanHelper::StringToSampleCountFlagBits(pMultisampleNode->ToElement()->AttributeString("samples"));
		m_param.multisample.bSampleShadingEnable = pMultisampleNode->ToElement()->AttributeBool("enable_sample_shading");
		m_param.multisample.minSampleShading = pMultisampleNode->ToElement()->AttributeFloat1("min_sample_shading");
		m_param.multisample.bAlphaToCoverageEnable = pMultisampleNode->ToElement()->AttributeBool("enable_alpha_to_coverage");
		m_param.multisample.bAlphaToOneEnable = pMultisampleNode->ToElement()->AttributeBool("enable_alpha_to_one");
		return TRUE;
	}

	BOOL CResGraphics::LoadDepth(TiXmlNode *pDepthNode)
	{
		m_param.depth.bDepthBoundsTestEnable = pDepthNode->ToElement()->AttributeBool("enable_depth_bounds_test");
		m_param.depth.minDepthBounds = pDepthNode->ToElement()->AttributeFloat1 ("min_depth_bound");
		m_param.depth.maxDepthBounds = pDepthNode->ToElement()->AttributeFloat1("max_depth_bound");
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
			} while (pBlendNode = pBlendNodes->IterateChildren("Attachment", pBlendNode));
		}
		return TRUE;
	}

	BOOL CResGraphics::SetShaders(void)
	{
		const CResShaderPtr &ptrVertexShader = ShaderManager()->LoadResource(m_param.shader.dwVertexName);
		if (ptrVertexShader.IsNull() || ptrVertexShader->IsValid() == FALSE) return FALSE;

		const CResShaderPtr &ptrFragmentShader = ShaderManager()->LoadResource(m_param.shader.dwFragmentName);
		if (ptrFragmentShader.IsNull() || ptrFragmentShader->IsValid() == FALSE) return FALSE;

		m_ptrPipelineGraphics->SetVertexShader(ptrVertexShader->GetShader());
		m_ptrPipelineGraphics->SetFragmentShader(ptrFragmentShader->GetShader());

		return TRUE;
	}

	BOOL CResGraphics::SetAssemblyState(void)
	{
		m_ptrPipelineGraphics->SetPrimitiveTopology(m_param.assembly.topology, m_param.assembly.bPrimitiveRestartEnable);
		return TRUE;
	}

	BOOL CResGraphics::SetTessellationState(void)
	{
		return TRUE;
	}

	BOOL CResGraphics::SetRasterizationState(void)
	{
		m_ptrPipelineGraphics->SetPolygonMode(m_param.rasterization.polygonMode);
		m_ptrPipelineGraphics->SetCullMode(m_param.rasterization.cullMode);
		m_ptrPipelineGraphics->SetFrontFace(m_param.rasterization.frontFace);
		m_ptrPipelineGraphics->SetDepthClamp(m_param.rasterization.bDepthClampEnable);
		m_ptrPipelineGraphics->SetDepthBias(m_param.rasterization.bDepthBiasEnable, m_param.rasterization.depthBiasConstantFactor, m_param.rasterization.depthBiasClamp, m_param.rasterization.depthBiasSlopeFactor);
		m_ptrPipelineGraphics->SetRasterizerDiscard(m_param.rasterization.bRasterizerDiscardEnable);
		return TRUE;
	}

	BOOL CResGraphics::SetMultisampleState(void)
	{
		m_ptrPipelineGraphics->SetSampleCounts(m_param.multisample.samples);
		m_ptrPipelineGraphics->SetSampleShading(m_param.multisample.bSampleShadingEnable, m_param.multisample.minSampleShading);
		m_ptrPipelineGraphics->SetSampleAlphaToCoverage(m_param.multisample.bAlphaToCoverageEnable);
		m_ptrPipelineGraphics->SetSampleAlphaToOne(m_param.multisample.bAlphaToOneEnable);
		return TRUE;
	}

	BOOL CResGraphics::SetDepthStencilState(void)
	{
		m_ptrPipelineGraphics->SetDepthBoundsTest(m_param.depth.bDepthBoundsTestEnable, m_param.depth.minDepthBounds, m_param.depth.maxDepthBounds);
		m_ptrPipelineGraphics->SetDepthTest(m_param.depth.bDepthTestEnable, m_param.depth.bDepthWriteEnable, m_param.depth.depthCompareOp);
		m_ptrPipelineGraphics->SetStencilTest(m_param.stencil.bStencilTestEnable, m_param.stencil.frontFailOp, m_param.stencil.frontPassOp, m_param.stencil.frontDepthFailOp, m_param.stencil.frontCompareOp, m_param.stencil.frontCompareMask, m_param.stencil.frontWriteMask, m_param.stencil.frontReference, m_param.stencil.backFailOp, m_param.stencil.backPassOp, m_param.stencil.backDepthFailOp, m_param.stencil.backCompareOp, m_param.stencil.backCompareMask, m_param.stencil.backWriteMask, m_param.stencil.backReference);
		return TRUE;
	}

	BOOL CResGraphics::SetColorBlendState(void)
	{
		m_ptrPipelineGraphics->SetColorBlendLogic(FALSE, VK_LOGIC_OP_COPY);
		m_ptrPipelineGraphics->SetColorBlendConstants(0.0f, 0.0f, 0.0f, 0.0f);

		for (const auto &itColorBlendAttachment : m_param.blends) {
			m_ptrPipelineGraphics->SetColorBlendAttachment(itColorBlendAttachment.first, itColorBlendAttachment.second.bBlendEnable, itColorBlendAttachment.second.srcColorBlendFactor, itColorBlendAttachment.second.dstColorBlendFactor, itColorBlendAttachment.second.colorBlendOp, itColorBlendAttachment.second.srcAlphaBlendFactor, itColorBlendAttachment.second.dstAlphaBlendFactor, itColorBlendAttachment.second.alphaBlendOp, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
		}

		return TRUE;
	}

}
