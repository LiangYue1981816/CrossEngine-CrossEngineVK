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

	CDrawable::CDrawable(void)
	{

	}

	CDrawable::~CDrawable(void)
	{

	}

	void CDrawable::SetMaterial(const CResMaterialPtr &ptrMaterial)
	{
		m_ptrMaterial = ptrMaterial;
	}

	const CGfxMaterialPtr& CDrawable::GetMaterial(void) const
	{
		return m_ptrMaterial->GetMaterial();
	}

	const CGfxDescriptorSetPtr& CDrawable::GetDescriptorSet(uint32_t dwPassName) const
	{
		static const CGfxDescriptorSetPtr ptrDescriptorSetNull;
		const auto &itDescriptorSet = m_ptrDescriptorSets.find(dwPassName);
		return itDescriptorSet != m_ptrDescriptorSets.end() ? itDescriptorSet->second : ptrDescriptorSetNull;
	}

	void CDrawable::SetTransform(const glm::mat4 &mtxModelToWorld)
	{
		m_mtxModelToWorld = mtxModelToWorld;
	}

	const glm::mat4& CDrawable::GetTransform(void) const
	{
		return m_mtxModelToWorld;
	}

	void CDrawable::SetUniform(uint32_t dwPassName, uint32_t dwName, uint32_t binding, float value)
	{
		const CGfxMaterialPassPtr &ptrMaterialPass = m_ptrMaterial->GetMaterial()->GetPass(dwPassName);
		if (ptrMaterialPass.IsNull()) return;

		const CGfxPipelineGraphicsPtr &ptrPipeline = ptrMaterialPass->GetPipeline();
		if (ptrPipeline.IsNull()) return;

		{
			CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
			ptrUniform->Create(sizeof(value), &value, FALSE);
			ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_DRAW, binding, 0, sizeof(value));
			m_ptrUniformBuffers[dwPassName][dwName] = ptrUniform;
		}
		
		UpdateDescriptorSet(dwPassName, ptrPipeline);
	}

	void CDrawable::SetUniform(uint32_t dwPassName, uint32_t dwName, uint32_t binding, glm::vec4 &value)
	{
		const CGfxMaterialPassPtr &ptrMaterialPass = m_ptrMaterial->GetMaterial()->GetPass(dwPassName);
		if (ptrMaterialPass.IsNull()) return;

		const CGfxPipelineGraphicsPtr &ptrPipeline = ptrMaterialPass->GetPipeline();
		if (ptrPipeline.IsNull()) return;

		{
			CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
			ptrUniform->Create(sizeof(value), &value, FALSE);
			ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_DRAW, binding, 0, sizeof(value));
			m_ptrUniformBuffers[dwPassName][dwName] = ptrUniform;
		}

		UpdateDescriptorSet(dwPassName, ptrPipeline);
	}

	void CDrawable::UpdateDescriptorSet(uint32_t dwPassName, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		m_ptrDescriptorSets[dwPassName] = GfxDevice()->AllocDescriptorSet(thread_id(), DESCRIPTOR_SET_DRAW, ptrPipeline);
		{
			for (const auto &itUniform : m_ptrUniformBuffers[dwPassName]) {
				m_ptrDescriptorSets[dwPassName]->SetUniformBuffer(ptrPipeline->GetBinding(DESCRIPTOR_SET_DRAW, itUniform.first), itUniform.second);
			}
		}
		m_ptrDescriptorSets[dwPassName]->UpdateDescriptorSets();
	}

}
