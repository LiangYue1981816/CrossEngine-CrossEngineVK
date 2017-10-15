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

	CPipelineGraphics::CPipelineGraphics(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		m_ptrGfxPipeline = GfxDevice()->NewPipelineGraphics();
	}

	CPipelineGraphics::~CPipelineGraphics(void)
	{
		m_ptrGfxPipeline.Release();
	}

	RESOURCE_TYPE CPipelineGraphics::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_GRAPHICS;
	}

	const CGfxPipelineGraphicsPtr& CPipelineGraphics::GetGfxPipeline(void) const
	{
		return m_ptrGfxPipeline;
	}

	BOOL CPipelineGraphics::Load(void)
	{
		if (LoadShaders() == FALSE) return FALSE;
		if (LoadInputAssemblyState() == FALSE) return FALSE;
		if (LoadTessellationState() == FALSE) return FALSE;
		if (LoadRasterizationState() == FALSE) return FALSE;
		if (LoadMultisampleState() == FALSE) return FALSE;
		if (LoadDepthStencilState() == FALSE) return FALSE;
		if (LoadColorBlendState() == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CPipelineGraphics::PostLoad(BOOL bSync)
	{
		return TRUE;
	}

	BOOL CPipelineGraphics::LoadShaders(void)
	{
		DWORD dwVertexShader;
		DWORD dwFragmentShader;
		m_stream >> dwVertexShader;
		m_stream >> dwFragmentShader;

		const CShaderPtr &ptrVertexShader = ShaderManager()->LoadResource(dwVertexShader);
		const CShaderPtr &ptrFragmentShader = ShaderManager()->LoadResource(dwFragmentShader);
		if (ptrVertexShader.IsNull() || ptrFragmentShader.IsNull()) return FALSE;

		m_ptrGfxPipeline->SetVertexShader(ptrVertexShader->GetGfxShader());
		m_ptrGfxPipeline->SetFragmentShader(ptrFragmentShader->GetGfxShader());

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadInputAssemblyState(void)
	{
		VkPipelineInputAssemblyStateCreateInfo state = {};
		{
			m_stream >> state.topology;
			m_stream >> state.primitiveRestartEnable;
		}
		m_ptrGfxPipeline->SetPrimitiveTopology(state.topology, state.primitiveRestartEnable);

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadTessellationState(void)
	{
		VkPipelineTessellationStateCreateInfo state = {};
		{
			m_stream >> state.patchControlPoints;
		}
		m_ptrGfxPipeline->SetTessellationPatchControlPoints(state.patchControlPoints);

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadRasterizationState(void)
	{
		VkPipelineRasterizationStateCreateInfo state = {};
		{
			m_stream >> state.depthClampEnable;
			m_stream >> state.rasterizerDiscardEnable;
			m_stream >> state.polygonMode;
			m_stream >> state.cullMode;
			m_stream >> state.frontFace;
			m_stream >> state.depthBiasEnable;
			m_stream >> state.depthBiasConstantFactor;
			m_stream >> state.depthBiasClamp;
			m_stream >> state.depthBiasSlopeFactor;
		}
		m_ptrGfxPipeline->SetPolygonMode(state.polygonMode);
		m_ptrGfxPipeline->SetCullMode(state.cullMode);
		m_ptrGfxPipeline->SetFrontFace(state.frontFace);
		m_ptrGfxPipeline->SetDepthClamp(state.depthClampEnable);
		m_ptrGfxPipeline->SetDepthBias(state.depthBiasEnable, state.depthBiasConstantFactor, state.depthBiasClamp, state.depthBiasSlopeFactor);
		m_ptrGfxPipeline->SetRasterizerDiscard(state.rasterizerDiscardEnable);

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadMultisampleState(void)
	{
		VkPipelineMultisampleStateCreateInfo state = {};
		{
			m_stream >> state.rasterizationSamples;
			m_stream >> state.sampleShadingEnable;
			m_stream >> state.minSampleShading;
			m_stream >> state.alphaToCoverageEnable;
			m_stream >> state.alphaToOneEnable;
		}
		m_ptrGfxPipeline->SetSampleCounts(state.rasterizationSamples);
		m_ptrGfxPipeline->SetSampleShading(state.sampleShadingEnable, state.minSampleShading);
		m_ptrGfxPipeline->SetSampleAlphaToCoverage(state.alphaToCoverageEnable);
		m_ptrGfxPipeline->SetSampleAlphaToOne(state.alphaToOneEnable);

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadDepthStencilState(void)
	{
		VkPipelineDepthStencilStateCreateInfo state = {};
		{
			m_stream >> state.depthTestEnable;
			m_stream >> state.depthWriteEnable;
			m_stream >> state.depthCompareOp;
			m_stream >> state.depthBoundsTestEnable;
			m_stream >> state.minDepthBounds;
			m_stream >> state.maxDepthBounds;
			m_stream >> state.stencilTestEnable;
			{
				m_stream >> state.front.failOp;
				m_stream >> state.front.passOp;
				m_stream >> state.front.depthFailOp;
				m_stream >> state.front.compareOp;
				m_stream >> state.front.compareMask;
				m_stream >> state.front.writeMask;
				m_stream >> state.front.reference;
			}
			{
				m_stream >> state.back.failOp;
				m_stream >> state.back.passOp;
				m_stream >> state.back.depthFailOp;
				m_stream >> state.back.compareOp;
				m_stream >> state.back.compareMask;
				m_stream >> state.back.writeMask;
				m_stream >> state.back.reference;
			}
		}
		m_ptrGfxPipeline->SetDepthTest(state.depthTestEnable, state.depthWriteEnable, state.depthCompareOp);
		m_ptrGfxPipeline->SetDepthBoundsTest(state.depthBoundsTestEnable, state.minDepthBounds, state.maxDepthBounds);
		m_ptrGfxPipeline->SetStencilTest(state.stencilTestEnable, state.front, state.back);

		return TRUE;
	}

	BOOL CPipelineGraphics::LoadColorBlendState(void)
	{
		VkPipelineColorBlendStateCreateInfo state = {};
		{
			m_stream >> state.logicOpEnable;
			m_stream >> state.logicOp;
			m_stream >> state.blendConstants;
		}
		m_ptrGfxPipeline->SetColorBlendLogic(state.logicOpEnable, state.logicOp);
		m_ptrGfxPipeline->SetColorBlendConstants(state.blendConstants[0], state.blendConstants[1], state.blendConstants[2], state.blendConstants[3]);

		std::vector<VkPipelineColorBlendAttachmentState> attachmentColorBlends;
		{
			m_stream >> attachmentColorBlends;
		}
		for (uint32_t index = 0; index < attachmentColorBlends.size(); index++) {
			m_ptrGfxPipeline->SetColorBlendAttachment(
				index,
				attachmentColorBlends[index].blendEnable,
				attachmentColorBlends[index].srcColorBlendFactor,
				attachmentColorBlends[index].dstColorBlendFactor,
				attachmentColorBlends[index].colorBlendOp,
				attachmentColorBlends[index].srcAlphaBlendFactor,
				attachmentColorBlends[index].dstAlphaBlendFactor,
				attachmentColorBlends[index].alphaBlendOp,
				attachmentColorBlends[index].colorWriteMask);
		}

		return TRUE;
	}

}
