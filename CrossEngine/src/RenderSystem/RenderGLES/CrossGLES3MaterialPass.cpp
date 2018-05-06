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
		: m_indexSubPass(0)
	{

	}

	CGLES3MaterialPass::~CGLES3MaterialPass(void)
	{

	}

	const uint32_t CGLES3MaterialPass::GetIndexSubPass(void) const
	{
		return m_indexSubPass;
	}

	const CGfxRenderPassPtr& CGLES3MaterialPass::GetRenderPass(void) const
	{
		return m_ptrRenderPass;
	}

	const CGfxPipelineGraphicsPtr& CGLES3MaterialPass::GetPipeline(void) const
	{
		return m_ptrPipeline;
	}

	const CGfxDescriptorSetPtr& CGLES3MaterialPass::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
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
		m_ptrTextures[dwName] = ptrTexture;
	}

	void CGLES3MaterialPass::SetUniformBuffer(uint32_t dwName, const CGfxUniformBufferPtr &ptrUniformBuffer)
	{
		m_ptrUniformBuffers[dwName] = ptrUniformBuffer;
	}

	void CGLES3MaterialPass::UpdateDescriptorSet(void)
	{
		m_ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(thread_id(), DESCRIPTOR_SET_PASS, m_ptrPipeline);
		{
			for (const auto &itTexture : m_ptrTextures) {
				m_ptrDescriptorSet->SetTexture(m_ptrPipeline->GetBinding(DESCRIPTOR_SET_PASS, itTexture.first), itTexture.second);
			}

			for (const auto &itUniformBuffer : m_ptrUniformBuffers) {
				m_ptrDescriptorSet->SetUniformBuffer(m_ptrPipeline->GetBinding(DESCRIPTOR_SET_PASS, itUniformBuffer.first), itUniformBuffer.second);
			}
		}
		m_ptrDescriptorSet->UpdateDescriptorSets();
	}

}
