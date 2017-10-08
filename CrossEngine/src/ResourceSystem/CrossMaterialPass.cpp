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

	const std::map<uint32_t, CMaterialPass::Texture>& CMaterialPass::GetTextures(void) const
	{
		return m_textures;
	}

	const std::map<uint32_t, CMaterialPass::Uniform>& CMaterialPass::GetUniforms(void) const
	{
		return m_uniforms;
	}

	BOOL CMaterialPass::Load(TiXmlNode *pPassNode)
	{
		if (LoadShaderVertex(pPassNode) == FALSE) return FALSE;
		if (LoadShaderFragment(pPassNode) == FALSE) return FALSE;
		if (LoadPrimitiveTopology(pPassNode) == FALSE) return FALSE;
		if (LoadTessellationPatchControlPoints(pPassNode) == FALSE) return FALSE;
		if (LoadPolygonMode(pPassNode) == FALSE) return FALSE;
		if (LoadCullMode(pPassNode) == FALSE) return FALSE;
		if (LoadFrontFace(pPassNode) == FALSE) return FALSE;
		if (LoadDepthClamp(pPassNode) == FALSE) return FALSE;
		if (LoadDepthBias(pPassNode) == FALSE) return FALSE;
		if (LoadRasterizerDiscard(pPassNode) == FALSE) return FALSE;
		if (LoadSampleCounts(pPassNode) == FALSE) return FALSE;
		if (LoadSampleShading(pPassNode) == FALSE) return FALSE;
		if (LoadSampleAlphaToCoverage(pPassNode) == FALSE) return FALSE;
		if (LoadSampleAlphaToOne(pPassNode) == FALSE) return FALSE;
		if (LoadDepthTest(pPassNode) == FALSE) return FALSE;
		if (LoadDepthBoundsTest(pPassNode) == FALSE) return FALSE;
		if (LoadStencilTest(pPassNode) == FALSE) return FALSE;
		if (LoadColorBlendLogic(pPassNode) == FALSE) return FALSE;
		if (LoadColorBlendConstants(pPassNode) == FALSE) return FALSE;
		if (LoadColorBlendAttachment(pPassNode) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CMaterialPass::LoadShaderVertex(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pShaderNode = pPassNode->FirstChild("ShaderVertex")) {
			const char *szName = pShaderNode->ToElement()->AttributeString("name");
			if (szName == NULL) return FALSE;

			const CShaderPtr &ptrShader = ShaderManager()->LoadResource(szName);
			if (ptrShader.IsNull()) return FALSE;

			return m_ptrGfxPipeline->SetVertexShader(ptrShader->GetGfxShader());
		}

		return FALSE;
	}

	BOOL CMaterialPass::LoadShaderFragment(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pShaderNode = pPassNode->FirstChild("ShaderFragment")) {
			const char *szName = pShaderNode->ToElement()->AttributeString("name");
			if (szName == NULL) return FALSE;

			const CShaderPtr &ptrShader = ShaderManager()->LoadResource(szName);
			if (ptrShader.IsNull()) return FALSE;

			return m_ptrGfxPipeline->SetFragmentShader(ptrShader->GetGfxShader());
		}

		return FALSE;
	}

	BOOL CMaterialPass::LoadPrimitiveTopology(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pPrimitiveTopologyNode = pPassNode->FirstChild("PrimitiveTopology")) {
			const char *szTopology = pPrimitiveTopologyNode->ToElement()->AttributeString("topology");
			const BOOL bRestart = pPrimitiveTopologyNode->ToElement()->AttributeInt("restart");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadTessellationPatchControlPoints(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadPolygonMode(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pPolygonMode = pPassNode->FirstChild("PolygonMode")) {
			const char *szPolygonMode = pPolygonMode->ToElement()->AttributeString("mode");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadCullMode(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pCullMode = pPassNode->FirstChild("CullMode")) {
			const char *szCullMode = pCullMode->ToElement()->AttributeString("mode");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadFrontFace(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pFrontFaceNode = pPassNode->FirstChild("FrontFace")) {
			const char *szFrontFace = pFrontFaceNode->ToElement()->AttributeString("front_face");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthClamp(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pDepthClampNode = pPassNode->FirstChild("DepthClamp")) {
			const BOOL bEnable = pDepthClampNode->ToElement()->AttributeInt("enable");
			return m_ptrGfxPipeline->SetDepthClamp(bEnable);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthBias(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pDepthBiasNode = pPassNode->FirstChild("DepthBias")) {
			const BOOL bEnable = pDepthBiasNode->ToElement()->AttributeInt("enable");
			const float factor = pDepthBiasNode->ToElement()->AttributeFloat("factor");
			const float clamp = pDepthBiasNode->ToElement()->AttributeFloat("clamp");
			const float slope = pDepthBiasNode->ToElement()->AttributeFloat("slope");
			return m_ptrGfxPipeline->SetDepthBias(bEnable, factor, clamp, slope);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadRasterizerDiscard(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pRasterizerDiscardNode = pPassNode->FirstChild("RasterizerDiscard")) {
			const BOOL bEnable = pRasterizerDiscardNode->ToElement()->AttributeInt("enable");
			return m_ptrGfxPipeline->SetRasterizerDiscard(bEnable);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleCounts(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pSampleCountsNode = pPassNode->FirstChild("SampleCounts")) {
			const char *szSampleCounts = pPassNode->ToElement()->AttributeString("sample_counts");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleShading(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pSampleShadingNode = pPassNode->FirstChild("SampleShading")) {
			const BOOL bEnable = pSampleShadingNode->ToElement()->AttributeInt("enable");
			const float minSampleShading = pSampleShadingNode->ToElement()->AttributeFloat("min_sample_Shading");
			return m_ptrGfxPipeline->SetSampleShading(bEnable, minSampleShading);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleAlphaToCoverage(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pSampleAlphaToCoverage = pPassNode->FirstChild("SampleAlphaToCoverage")) {
			const BOOL bEnable = pSampleAlphaToCoverage->ToElement()->AttributeInt("enable");
			return m_ptrGfxPipeline->SetSampleAlphaToCoverage(bEnable);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleAlphaToOne(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pSampleAlphaToOne = pPassNode->FirstChild("SampleAlphaToOne")) {
			const BOOL bEnable = pSampleAlphaToOne->ToElement()->AttributeInt("enable");
			return m_ptrGfxPipeline->SetSampleAlphaToOne(bEnable);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthTest(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pDepthTestNode = pPassNode->FirstChild("DepthTest")) {
			const BOOL bEnableTest = pDepthTestNode->ToElement()->AttributeInt("enable_test");
			const BOOL bEnableWrite = pDepthTestNode->ToElement()->AttributeInt("enable_write");
			const char *szCompareOp = pDepthTestNode->ToElement()->AttributeString("compare_op");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthBoundsTest(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pDepthBoundsTestNode = pPassNode->FirstChild("DepthBoundsTest")) {
			const BOOL bEnable = pDepthBoundsTestNode->ToElement()->AttributeInt("enable");
			const float minDepthBounds = pDepthBoundsTestNode->ToElement()->AttributeFloat("min_depth_bounds");
			const float maxDepthBounds = pDepthBoundsTestNode->ToElement()->AttributeFloat("max_depth_bounds");
			return m_ptrGfxPipeline->SetDepthBoundsTest(bEnable, minDepthBounds, maxDepthBounds);
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadStencilTest(TiXmlNode *pPassNode)
	{
		if (TiXmlNode *pStencilTestNode = pPassNode->FirstChild("StencilTest")) {
			const BOOL bEnable = pStencilTestNode->ToElement()->AttributeInt("enable");
			const char *szFront = pStencilTestNode->ToElement()->AttributeString("front");
			const char *szBack = pStencilTestNode->ToElement()->AttributeString("back");
		}

		return TRUE;
	}

	BOOL CMaterialPass::LoadColorBlendLogic(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadColorBlendConstants(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadColorBlendAttachment(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::PostLoad(void)
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
