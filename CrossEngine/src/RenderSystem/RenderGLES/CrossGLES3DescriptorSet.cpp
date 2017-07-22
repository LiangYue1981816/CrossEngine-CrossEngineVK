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

	CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3Device *pDevice, CGLES3DescriptorSetLayout *pDescriptorSetLayout)
		: m_pDevice(pDevice)
		, m_pDescriptorSetLayout(pDescriptorSetLayout)
	{

	}

	CGLES3DescriptorSet::~CGLES3DescriptorSet(void)
	{

	}

	void CGLES3DescriptorSet::Release(void)
	{

	}

	HANDLE CGLES3DescriptorSet::GetHandle(void) const
	{
		ASSERT(FALSE);
		return INVALID_HANDLE_VALUE;
	}

	void CGLES3DescriptorSet::SetTexture(uint32_t binding, const CGfxTexturePtr &ptrTexture)
	{
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

}
