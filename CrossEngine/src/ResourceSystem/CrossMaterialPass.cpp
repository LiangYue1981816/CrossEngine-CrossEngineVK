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
		if (LoadSampleMask(pPassNode) == FALSE) return FALSE;
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
		return TRUE;
	}

	BOOL CMaterialPass::LoadTessellationPatchControlPoints(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadPolygonMode(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadCullMode(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadFrontFace(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthClamp(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthBias(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadRasterizerDiscard(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleCounts(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleShading(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleMask(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleAlphaToCoverage(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadSampleAlphaToOne(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthTest(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadDepthBoundsTest(TiXmlNode *pPassNode)
	{
		return TRUE;
	}

	BOOL CMaterialPass::LoadStencilTest(TiXmlNode *pPassNode)
	{
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
