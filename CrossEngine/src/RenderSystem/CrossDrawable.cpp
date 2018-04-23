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

	void CDrawable::SetUniformBuffer(uint32_t dwPassName, uint32_t dwName, uint32_t binding, const void *pBuffer, size_t offset, size_t size)
	{
		const CGfxMaterialPassPtr &ptrPass = m_ptrMaterial->GetMaterial()->GetPass(dwPassName);
		if (ptrPass.IsNull()) return;

		const CGfxPipelineGraphicsPtr &ptrPipeline = ptrPass->GetPipeline();
		if (ptrPipeline.IsNull()) return;

		CGfxUniformBufferPtr ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		ptrUniformBuffer->Create(size, pBuffer, TRUE);
		ptrUniformBuffer->SetDescriptorBufferInfo(DESCRIPTOR_SET_DRAW, binding, offset, size);
		m_ptrUniformBuffers[dwPassName][dwName] = ptrUniformBuffer;
		
		UpdateDescriptorSet(dwPassName, ptrPipeline);
	}

	void CDrawable::UpdateDescriptorSet(uint32_t dwPassName, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		uint32_t thread = thread_id();
		uint32_t frame = GfxSwapChain()->GetImageIndex();
		uint32_t pool = ((thread^frame) >> 1) | 0x80000000;

		m_ptrDescriptorSets[dwPassName] = GfxDevice()->AllocDescriptorSet(pool, DESCRIPTOR_SET_DRAW, ptrPipeline);
		{
			for (const auto &itUniformBuffer : m_ptrUniformBuffers[dwPassName]) {
				m_ptrDescriptorSets[dwPassName]->SetUniformBuffer(ptrPipeline->GetBinding(DESCRIPTOR_SET_DRAW, itUniformBuffer.first), itUniformBuffer.second);
			}
		}
		m_ptrDescriptorSets[dwPassName]->UpdateDescriptorSets();
	}

}
