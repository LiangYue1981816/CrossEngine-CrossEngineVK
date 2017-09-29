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
