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

	CRendererDescriptorSet::CRendererDescriptorSet(CRendererDevice *pDevice, VkDescriptorSet vkDescriptorSet, const uint32_t *typesUsedCount)
		: m_pDevice(pDevice)
		, m_vkDescriptorSet(vkDescriptorSet)
		, m_typesUsedCount{ 0 }
	{
		memcpy(m_typesUsedCount, typesUsedCount, sizeof(m_typesUsedCount));
	}

	CRendererDescriptorSet::~CRendererDescriptorSet(void)
	{

	}

	void CRendererDescriptorSet::ClearWriteDescriptorSets(void)
	{
		m_vkWriteDescriptorSets.clear();
	}

	void CRendererDescriptorSet::WriteDescriptorSet(uint32_t binding, VkDescriptorType type, VkDescriptorBufferInfo* pDescriptorBufferInfos, uint32_t descriptorCount)
	{
		m_vkWriteDescriptorSets[binding] = {};
		m_vkWriteDescriptorSets[binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		m_vkWriteDescriptorSets[binding].pNext = NULL;
		m_vkWriteDescriptorSets[binding].dstSet = m_vkDescriptorSet;
		m_vkWriteDescriptorSets[binding].dstBinding = binding;
		m_vkWriteDescriptorSets[binding].dstArrayElement = 0;
		m_vkWriteDescriptorSets[binding].descriptorCount = descriptorCount;
		m_vkWriteDescriptorSets[binding].descriptorType = type;
		m_vkWriteDescriptorSets[binding].pImageInfo = NULL;
		m_vkWriteDescriptorSets[binding].pBufferInfo = pDescriptorBufferInfos;
		m_vkWriteDescriptorSets[binding].pTexelBufferView = NULL;
	}

	void CRendererDescriptorSet::WriteDescriptorSet(uint32_t binding, VkDescriptorType type, VkDescriptorImageInfo* pDescriptorImageInfos, uint32_t descriptorCount)
	{
		m_vkWriteDescriptorSets[binding] = {};
		m_vkWriteDescriptorSets[binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		m_vkWriteDescriptorSets[binding].pNext = NULL;
		m_vkWriteDescriptorSets[binding].dstSet = m_vkDescriptorSet;
		m_vkWriteDescriptorSets[binding].dstBinding = binding;
		m_vkWriteDescriptorSets[binding].dstArrayElement = 0;
		m_vkWriteDescriptorSets[binding].descriptorCount = descriptorCount;
		m_vkWriteDescriptorSets[binding].descriptorType = type;
		m_vkWriteDescriptorSets[binding].pImageInfo = pDescriptorImageInfos;
		m_vkWriteDescriptorSets[binding].pBufferInfo = NULL;
		m_vkWriteDescriptorSets[binding].pTexelBufferView = NULL;
	}

	void CRendererDescriptorSet::UpdateDescriptorSets(void) const
	{
		std::vector<VkWriteDescriptorSet> writes;

		for (const auto &itWrite : m_vkWriteDescriptorSets) {
			writes.push_back(itWrite.second);
		}

		vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, NULL);
	}

	VkDescriptorSet CRendererDescriptorSet::GetDescriptorSet(void) const
	{
		return m_vkDescriptorSet;
	}

	const uint32_t* CRendererDescriptorSet::GetTypesUsedCount(void) const
	{
		return m_typesUsedCount;
	}

}
