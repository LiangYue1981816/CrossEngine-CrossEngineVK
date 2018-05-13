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

	CVulkanMaterialPass::CVulkanMaterialPass(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_indexSubPass(0)
	{

	}

	CVulkanMaterialPass::~CVulkanMaterialPass(void)
	{

	}

	CVulkanDevice * CVulkanMaterialPass::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanMaterialPass::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	void CVulkanMaterialPass::Destroy(void)
	{

	}

	void CVulkanMaterialPass::DumpLog(void) const
	{

	}

	const uint32_t CVulkanMaterialPass::GetIndexSubPass(void) const
	{
		return m_indexSubPass;
	}

	const CGfxRenderPassPtr& CVulkanMaterialPass::GetRenderPass(void) const
	{
		return m_ptrRenderPass;
	}

	const CGfxPipelineGraphicsPtr& CVulkanMaterialPass::GetPipeline(void) const
	{
		return m_ptrPipeline;
	}

	const CGfxDescriptorSetPtr& CVulkanMaterialPass::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
	}

	void CVulkanMaterialPass::SetRenderPass(const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass)
	{
		m_indexSubPass = indexSubPass;
		m_ptrRenderPass = ptrRenderPass;
	}

	void CVulkanMaterialPass::SetPipeline(const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		m_ptrPipeline = ptrPipeline;
	}

	void CVulkanMaterialPass::SetTexture(uint32_t dwName, const CGfxTexturePtr &ptrTexture)
	{
		m_ptrTextures[dwName] = ptrTexture;
	}

	void CVulkanMaterialPass::SetUniformBuffer(uint32_t dwName, const CGfxUniformBufferPtr &ptrUniformBuffer)
	{
		m_ptrUniformBuffers[dwName] = ptrUniformBuffer;
	}

	void CVulkanMaterialPass::UpdateDescriptorSet(void)
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
