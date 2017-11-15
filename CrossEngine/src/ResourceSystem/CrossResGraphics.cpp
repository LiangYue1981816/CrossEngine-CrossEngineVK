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
		, m_indexSubPass(0)
	{
		m_ptrGfxPipeline = GfxDevice()->NewPipelineGraphics();
	}

	CResGraphics::~CResGraphics(void)
	{
		m_ptrGfxPipeline.Release();
		m_ptrGfxRenderPass.Release();
	}

	RESOURCE_TYPE CResGraphics::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_GRAPHICS;
	}

	const uint32_t CResGraphics::GetIndexSubPass(void) const
	{
		return m_indexSubPass;
	}

	const CGfxRenderPassPtr& CResGraphics::GetGfxRenderPass(void) const
	{
		return m_ptrGfxRenderPass;
	}

	const CGfxPipelineGraphicsPtr& CResGraphics::GetGfxPipeline(void) const
	{
		return m_ptrGfxPipeline;
	}

	BOOL CResGraphics::Load(BOOL bSync)
	{
		BOOL rcode = TRUE;

		if (rcode) rcode = LoadData();
		if (rcode) rcode = LoadShaders();
		if (rcode) rcode = LoadRenderPass();
		if (rcode) rcode = LoadInputAssemblyState();
		if (rcode) rcode = LoadTessellationState();
		if (rcode) rcode = LoadRasterizationState();
		if (rcode) rcode = LoadMultisampleState();
		if (rcode) rcode = LoadDepthStencilState();
		if (rcode) rcode = LoadColorBlendState();

		return rcode;
	}

	BOOL CResGraphics::PostLoad(void)
	{
		BOOL rcode = m_ptrGfxPipeline->Create(m_ptrGfxRenderPass->GetHandle(), m_indexSubPass);

		m_stream.Free();
		m_bIsLoaded = TRUE;

		return rcode;
	}

	BOOL CResGraphics::LoadData(void)
	{
		m_stream << m_data.mark;
		if (m_data.mark != HashValue("PipelineGraphicsData")) {
			return FALSE;
		}

		m_stream << m_data.size;
		if (m_data.size != sizeof(PipelineGraphicsData)) {
			return FALSE;
		}

		m_stream << m_data.shader;
		m_stream << m_data.renderPass;
		m_stream << m_data.inputAssembly;
		m_stream << m_data.rasterization;
		m_stream << m_data.multisample;
		m_stream << m_data.depth;
		m_stream << m_data.stencil;
		m_stream << m_data.colorBlendAttachments;

		return TRUE;
	}

	BOOL CResGraphics::LoadShaders(void)
	{
		const CResShaderPtr &ptrResVertexShader = ShaderManager()->LoadResource(m_data.shader.vertex);
		const CResShaderPtr &ptrResFragmentShader = ShaderManager()->LoadResource(m_data.shader.fragment);
		if (ptrResVertexShader.IsNull() || ptrResFragmentShader.IsNull()) return FALSE;

		m_ptrGfxPipeline->SetVertexShader(ptrResVertexShader->GetGfxShader());
		m_ptrGfxPipeline->SetFragmentShader(ptrResFragmentShader->GetGfxShader());

		return TRUE;
	}

	BOOL CResGraphics::LoadRenderPass(void)
	{
		m_indexSubPass = m_data.renderPass.indexSubPass;
		m_ptrGfxRenderPass = RenderPassManager()->GetRenderPass(m_data.renderPass.dwName);
		if (m_ptrGfxRenderPass.IsNull()) return FALSE;

		return TRUE;
	}

	BOOL CResGraphics::LoadInputAssemblyState(void)
	{
		m_ptrGfxPipeline->SetPrimitiveTopology((VkPrimitiveTopology)m_data.inputAssembly.topology, FALSE);
		return TRUE;
	}

	BOOL CResGraphics::LoadTessellationState(void)
	{
		return TRUE;
	}

	BOOL CResGraphics::LoadRasterizationState(void)
	{
		m_ptrGfxPipeline->SetPolygonMode((VkPolygonMode)m_data.rasterization.polygonMode);
		m_ptrGfxPipeline->SetCullMode((VkCullModeFlags)m_data.rasterization.cullMode);
		m_ptrGfxPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
		m_ptrGfxPipeline->SetDepthClamp(FALSE);
		m_ptrGfxPipeline->SetDepthBias(m_data.rasterization.depthBiasEnable, m_data.rasterization.depthBiasConstantFactor, 0.0f, m_data.rasterization.depthBiasSlopeFactor);
		m_ptrGfxPipeline->SetRasterizerDiscard(FALSE);

		return TRUE;
	}

	BOOL CResGraphics::LoadMultisampleState(void)
	{
		m_ptrGfxPipeline->SetSampleCounts((VkSampleCountFlagBits)m_data.multisample.rasterizationSamples);
		m_ptrGfxPipeline->SetSampleShading(FALSE, 0.0f);
		m_ptrGfxPipeline->SetSampleAlphaToCoverage(m_data.multisample.alphaToCoverageEnable);
		m_ptrGfxPipeline->SetSampleAlphaToOne(m_data.multisample.alphaToOneEnable);

		return TRUE;
	}

	BOOL CResGraphics::LoadDepthStencilState(void)
	{
		m_ptrGfxPipeline->SetDepthBoundsTest(FALSE, 0.0f, 0.0f);
		m_ptrGfxPipeline->SetDepthTest(m_data.depth.depthTestEnable, m_data.depth.depthWriteEnable, (VkCompareOp)m_data.depth.depthCompareOp);
		m_ptrGfxPipeline->SetStencilTest(m_data.stencil.stencilTestEnable, (VkStencilOp)m_data.stencil.frontFailOp, (VkStencilOp)m_data.stencil.frontPassOp, (VkStencilOp)m_data.stencil.frontDepthFailOp, (VkCompareOp)m_data.stencil.frontCompareOp, m_data.stencil.frontCompareMask, m_data.stencil.frontWriteMask, m_data.stencil.frontReference, (VkStencilOp)m_data.stencil.backFailOp, (VkStencilOp)m_data.stencil.backPassOp, (VkStencilOp)m_data.stencil.backDepthFailOp, (VkCompareOp)m_data.stencil.backCompareOp, m_data.stencil.backCompareMask, m_data.stencil.backWriteMask, m_data.stencil.backReference);

		return TRUE;
	}

	BOOL CResGraphics::LoadColorBlendState(void)
	{
		m_ptrGfxPipeline->SetColorBlendLogic(FALSE, VK_LOGIC_OP_CLEAR);
		m_ptrGfxPipeline->SetColorBlendConstants(0.0f, 0.0f, 0.0f, 0.0f);

		for (std::map<uint32_t, PipelineGraphicsData::ColorBlendAttachment>::const_iterator itColorBlendAttachment = m_data.colorBlendAttachments.begin(); itColorBlendAttachment != m_data.colorBlendAttachments.end(); ++itColorBlendAttachment) {
			m_ptrGfxPipeline->SetColorBlendAttachment(itColorBlendAttachment->first, itColorBlendAttachment->second.blendEnable, (VkBlendFactor)itColorBlendAttachment->second.srcColorBlendFactor, (VkBlendFactor)itColorBlendAttachment->second.dstColorBlendFactor, (VkBlendOp)itColorBlendAttachment->second.colorBlendOp, (VkBlendFactor)itColorBlendAttachment->second.srcAlphaBlendFactor, (VkBlendFactor)itColorBlendAttachment->second.dstAlphaBlendFactor, (VkBlendOp)itColorBlendAttachment->second.alphaBlendOp, (VkColorComponentFlags)itColorBlendAttachment->second.colorWriteMask);
		}

		return TRUE;
	}

	BOOL CResGraphics::IsLoaded(void) const
	{
		return m_bIsLoaded;
	}

}
