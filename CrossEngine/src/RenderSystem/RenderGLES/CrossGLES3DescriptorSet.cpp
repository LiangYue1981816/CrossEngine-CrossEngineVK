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

	CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3Device *pDevice, CGLES3DescriptorSetManager *pDescriptorSetManager, CGLES3DescriptorSetLayout *pDescriptorSetLayout)
		: m_pDevice(pDevice)
		, m_pDescriptorSetManager(pDescriptorSetManager)
		, m_pDescriptorSetLayout(pDescriptorSetLayout)
	{

	}

	CGLES3DescriptorSet::~CGLES3DescriptorSet(void)
	{

	}

	void CGLES3DescriptorSet::Release(void)
	{
		m_pDescriptorSetManager->FreeDescriptorSet(this);
	}

	HANDLE CGLES3DescriptorSet::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	void CGLES3DescriptorSet::SetTexture(uint32_t binding, uint32_t unit, const CGfxTexturePtr &ptrTexture)
	{
		m_units[binding] = unit;
		m_ptrTextures[binding] = ptrTexture;
	}

	void CGLES3DescriptorSet::SetUniformBuffer(uint32_t binding, const CGfxUniformBufferPtr &ptrUniformBuffer)
	{
		m_ptrUniformBuffers[binding] = ptrUniformBuffer;
	}

	void CGLES3DescriptorSet::UpdateDescriptorSets(void) const
	{

	}

	const CGLES3DescriptorSetLayout* CGLES3DescriptorSet::GetDescriptorSetLayout(void) const
	{
		return m_pDescriptorSetLayout;
	}

	const uint32_t CGLES3DescriptorSet::GetTextureUnit(uint32_t binding) const
	{
		const auto &itUnit = m_units.find(binding);
		return itUnit != m_units.end() ? itUnit->second : 0;
	}

	const CGfxTexturePtr CGLES3DescriptorSet::GetTexture(uint32_t binding) const
	{
		const auto &itTexture = m_ptrTextures.find(binding);
		return itTexture != m_ptrTextures.end() ? itTexture->second : CGfxTexturePtr(NULL);
	}

	const CGfxUniformBufferPtr CGLES3DescriptorSet::GetUniformBuffer(uint32_t binding) const
	{
		const auto &itUniformBuffer = m_ptrUniformBuffers.find(binding);
		return itUniformBuffer != m_ptrUniformBuffers.end() ? itUniformBuffer->second : CGfxUniformBufferPtr(NULL);
	}

}
