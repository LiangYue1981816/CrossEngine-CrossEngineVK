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

	CVulkanDescriptorSet::CVulkanDescriptorSet(CVulkanDescriptorPool *pDescriptorPool, CVulkanDevice *pDevice, VkDescriptorSet vkDescriptorSet, const uint32_t *typesUsedCount)
		: m_pDevice(pDevice)
		, m_pDescriptorPool(pDescriptorPool)
		, m_vkDescriptorSet(vkDescriptorSet)
		, m_typesUsedCount{ 0 }
	{
		memcpy(m_typesUsedCount, typesUsedCount, sizeof(m_typesUsedCount));
	}

	CVulkanDescriptorSet::~CVulkanDescriptorSet(void)
	{
		for (auto &itTexture : m_ptrTextures) {
			CVulkanTexturePtr &ptrTexture = itTexture.second;
			ptrTexture.SetNull();
		}

		for (auto &itUniformBuffer : m_ptrUniformBuffers) {
			CVulkanUniformBufferPtr &ptrUniformBuffer = itUniformBuffer.second;
			ptrUniformBuffer.SetNull();
		}

		m_ptrTextures.clear();
		m_ptrUniformBuffers.clear();
	}

	void CVulkanDescriptorSet::SetTexture(uint32_t binding, const CVulkanTexturePtr &ptrTexture)
	{
		m_ptrTextures[binding] = ptrTexture;
	}

	void CVulkanDescriptorSet::SetUniformBuffer(uint32_t binding, const CVulkanUniformBufferPtr &ptrUniformBuffer)
	{
		m_ptrUniformBuffers[binding] = ptrUniformBuffer;
	}

	void CVulkanDescriptorSet::UpdateDescriptorSets(void) const
	{
		std::vector<VkWriteDescriptorSet> writes;

		for (const auto &itTexture : m_ptrTextures) {
			const uint32_t binding = itTexture.first;
			const CVulkanTexturePtr &ptrTexture = itTexture.second;

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = NULL;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = binding;
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &ptrTexture->GetDescriptorImageInfo();
			write.pBufferInfo = NULL;
			write.pTexelBufferView = NULL;

			writes.push_back(write);
		}

		for (const auto &itUniformBuffer : m_ptrUniformBuffers) {
			const uint32_t binding = itUniformBuffer.first;
			const CVulkanUniformBufferPtr &ptrUniformBuffer = itUniformBuffer.second;

			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = NULL;
			write.dstSet = m_vkDescriptorSet;
			write.dstBinding = binding;
			write.dstArrayElement = 0;
			write.descriptorCount = 1;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.pImageInfo = NULL;
			write.pBufferInfo = &ptrUniformBuffer->GetDescriptorBufferInfo(binding);
			write.pTexelBufferView = NULL;

			writes.push_back(write);
		}

		vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, NULL);
	}

	CVulkanDescriptorPool* CVulkanDescriptorSet::GetDescriptorPool(void) const
	{
		return m_pDescriptorPool;
	}

	VkDescriptorSet CVulkanDescriptorSet::GetDescriptorSet(void) const
	{
		return m_vkDescriptorSet;
	}

	const uint32_t* CVulkanDescriptorSet::GetTypesUsedCount(void) const
	{
		return m_typesUsedCount;
	}

}
