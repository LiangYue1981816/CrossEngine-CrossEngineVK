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

	CMaterialPass::CMaterialPass(void)
		: pNext(NULL)
	{
		m_ptrGfxPipeline = GfxDevice()->NewPipelineGraphics();
	}

	CMaterialPass::~CMaterialPass(void)
	{
		m_ptrGfxPipeline.Release();
	}

	const CGfxPipelineGraphicsPtr& CMaterialPass::GetGfxPipeline(void) const
	{
		return m_ptrGfxPipeline;
	}

	const std::map<DWORD, CMaterialPass::Texture>& CMaterialPass::GetTextures(void) const
	{
		return m_textures;
	}

	const std::map<DWORD, CMaterialPass::Uniform>& CMaterialPass::GetUniforms(void) const
	{
		return m_uniforms;
	}

	BOOL CMaterialPass::Load(CStream *pStream)
	{
		if (LoadShaders(pStream) == FALSE) return FALSE;
		if (LoadInputAssemblyState(pStream) == FALSE) return FALSE;
		if (LoadTessellationState(pStream) == FALSE) return FALSE;
		if (LoadRasterizationState(pStream) == FALSE) return FALSE;
		if (LoadMultisampleState(pStream) == FALSE) return FALSE;
		if (LoadDepthStencilState(pStream) == FALSE) return FALSE;
		if (LoadColorBlendState(pStream) == FALSE) return FALSE;
		if (LoadTextures(pStream) == FALSE) return FALSE;
		if (LoadUniforms(pStream) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CMaterialPass::PostLoad(void)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadShaders(CStream *pStream)
	{
		DWORD dwVertexShader;
		DWORD dwFragmentShader;
		*pStream >> dwVertexShader;
		*pStream >> dwFragmentShader;

		const CShaderPtr &ptrVertexShader = ShaderManager()->LoadResource(dwVertexShader);
		const CShaderPtr &ptrFragmentShader = ShaderManager()->LoadResource(dwFragmentShader);
		if (ptrVertexShader.IsNull() || ptrFragmentShader.IsNull()) return FALSE;

		m_ptrGfxPipeline->SetVertexShader(ptrVertexShader->GetGfxShader());
		m_ptrGfxPipeline->SetFragmentShader(ptrFragmentShader->GetGfxShader());

		return TRUE;
	}

	BOOL CMaterialPass::LoadInputAssemblyState(CStream *pStream)
	{
		VkPipelineInputAssemblyStateCreateInfo state = {};
		{
			*pStream >> state.topology;
			*pStream >> state.primitiveRestartEnable;
		}
		m_ptrGfxPipeline->SetPrimitiveTopology(state.topology, state.primitiveRestartEnable);

		return TRUE;
	}

	BOOL CMaterialPass::LoadTessellationState(CStream *pStream)
	{
		VkPipelineTessellationStateCreateInfo state = {};
		{
			*pStream >> state.patchControlPoints;
		}
		m_ptrGfxPipeline->SetTessellationPatchControlPoints(state.patchControlPoints);

		return TRUE;
	}

	BOOL CMaterialPass::LoadRasterizationState(CStream *pStream)
	{
		VkPipelineRasterizationStateCreateInfo state = {};
		{
			*pStream >> state.depthClampEnable;
			*pStream >> state.rasterizerDiscardEnable;
			*pStream >> state.polygonMode;
			*pStream >> state.cullMode;
			*pStream >> state.frontFace;
			*pStream >> state.depthBiasEnable;
			*pStream >> state.depthBiasConstantFactor;
			*pStream >> state.depthBiasClamp;
			*pStream >> state.depthBiasSlopeFactor;
		}
		m_ptrGfxPipeline->SetPolygonMode(state.polygonMode);
		m_ptrGfxPipeline->SetCullMode(state.cullMode);
		m_ptrGfxPipeline->SetFrontFace(state.frontFace);
		m_ptrGfxPipeline->SetDepthClamp(state.depthClampEnable);
		m_ptrGfxPipeline->SetDepthBias(state.depthBiasEnable, state.depthBiasConstantFactor, state.depthBiasClamp, state.depthBiasSlopeFactor);
		m_ptrGfxPipeline->SetRasterizerDiscard(state.rasterizerDiscardEnable);

		return TRUE;
	}

	BOOL CMaterialPass::LoadMultisampleState(CStream *pStream)
	{
		VkPipelineMultisampleStateCreateInfo state = {};
		{
			*pStream >> state.rasterizationSamples;
			*pStream >> state.sampleShadingEnable;
			*pStream >> state.minSampleShading;
			*pStream >> state.alphaToCoverageEnable;
			*pStream >> state.alphaToOneEnable;
		}
		m_ptrGfxPipeline->SetSampleCounts(state.rasterizationSamples);
		m_ptrGfxPipeline->SetSampleShading(state.sampleShadingEnable, state.minSampleShading);
		m_ptrGfxPipeline->SetSampleAlphaToCoverage(state.alphaToCoverageEnable);
		m_ptrGfxPipeline->SetSampleAlphaToOne(state.alphaToOneEnable);

		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthStencilState(CStream *pStream)
	{
		VkPipelineDepthStencilStateCreateInfo state = {};
		{
			*pStream >> state.depthTestEnable;
			*pStream >> state.depthWriteEnable;
			*pStream >> state.depthCompareOp;
			*pStream >> state.depthBoundsTestEnable;
			*pStream >> state.minDepthBounds;
			*pStream >> state.maxDepthBounds;
			*pStream >> state.stencilTestEnable;
			{
				*pStream >> state.front.failOp;
				*pStream >> state.front.passOp;
				*pStream >> state.front.depthFailOp;
				*pStream >> state.front.compareOp;
				*pStream >> state.front.compareMask;
				*pStream >> state.front.writeMask;
				*pStream >> state.front.reference;
			}
			{
				*pStream >> state.back.failOp;
				*pStream >> state.back.passOp;
				*pStream >> state.back.depthFailOp;
				*pStream >> state.back.compareOp;
				*pStream >> state.back.compareMask;
				*pStream >> state.back.writeMask;
				*pStream >> state.back.reference;
			}
		}
		m_ptrGfxPipeline->SetDepthTest(state.depthTestEnable, state.depthWriteEnable, state.depthCompareOp);
		m_ptrGfxPipeline->SetDepthBoundsTest(state.depthBoundsTestEnable, state.minDepthBounds, state.maxDepthBounds);
		m_ptrGfxPipeline->SetStencilTest(state.stencilTestEnable, state.front, state.back);

		return TRUE;
	}

	BOOL CMaterialPass::LoadColorBlendState(CStream *pStream)
	{
		VkPipelineColorBlendStateCreateInfo state = {};
		{
			*pStream >> state.logicOpEnable;
			*pStream >> state.logicOp;
			*pStream >> state.blendConstants;
		}
		m_ptrGfxPipeline->SetColorBlendLogic(state.logicOpEnable, state.logicOp);
		m_ptrGfxPipeline->SetColorBlendConstants(state.blendConstants[0], state.blendConstants[1], state.blendConstants[2], state.blendConstants[3]);
		
		std::vector<VkPipelineColorBlendAttachmentState> attachmentColorBlends;
		{
			*pStream >> attachmentColorBlends;
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

	BOOL CMaterialPass::LoadTextures(CStream *pStream)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadUniforms(CStream *pStream)
	{
		return TRUE;
	}

	BOOL CMaterialPass::SetTexture(const char *szTextureName, uint32_t unit)
	{
		return TRUE;
	}

	BOOL CMaterialPass::SetUniformFloat(const char *szUniformName, float value)
	{
		return TRUE;
	}

	BOOL CMaterialPass::SetUniformFloat4(const char *szUniformName, float *values)
	{
		return TRUE;
	}

	BOOL CMaterialPass::SetUniformMatrix4(const char *szUniformName, float *values)
	{
		return TRUE;
	}

}
