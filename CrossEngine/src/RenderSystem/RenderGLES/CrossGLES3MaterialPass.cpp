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

	CGLES3MaterialPass::CGLES3MaterialPass(void)
	{

	}

	CGLES3MaterialPass::~CGLES3MaterialPass(void)
	{

	}

	void CGLES3MaterialPass::SetRenderPass(const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass)
	{
		m_indexSubPass = indexSubPass;
		m_ptrRenderPass = ptrRenderPass;
	}

	void CGLES3MaterialPass::SetPipeline(const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		m_ptrPipeline = ptrPipeline;
	}

	void CGLES3MaterialPass::SetTexture(uint32_t dwName, const CGfxTexturePtr &ptrTexture)
	{
		m_textures[dwName] = ptrTexture;
	}

	void CGLES3MaterialPass::SetUniform(uint32_t dwName, const CGfxUniformBufferPtr &ptrUniform)
	{
		m_uniforms[dwName] = ptrUniform;
	}

	void CGLES3MaterialPass::UpdateDescriptorSet(uint32_t pool)
	{
		m_ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(pool, DESCRIPTOR_SET_PASS, m_ptrPipeline);
		{
			for (const auto &itTexture : m_textures) {
				m_ptrDescriptorSet->SetTexture(m_ptrPipeline->GetBinding(DESCRIPTOR_SET_PASS, itTexture.first), itTexture.second);
			}

			for (const auto &itUniform : m_uniforms) {
				m_ptrDescriptorSet->SetUniformBuffer(m_ptrPipeline->GetBinding(DESCRIPTOR_SET_PASS, itUniform.first), itUniform.second);
			}
		}
		m_ptrDescriptorSet->UpdateDescriptorSets();
	}

}
