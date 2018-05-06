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

	CVulkanDescriptorSet::CVulkanDescriptorSet(CVulkanDevice *pDevice, CVulkanDescriptorPool *pDescriptorPool, const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout)
		: m_pDevice(pDevice)
		, m_pDescriptorPool(pDescriptorPool)
		, m_ptrDescriptorSetLayout(ptrDescriptorSetLayout)
		, m_vkDescriptorSet(VK_NULL_HANDLE)

		, m_bNeedUpdate(FALSE)
	{
		VkDescriptorSetLayout vkDescriptorSetLayout = (VkDescriptorSetLayout)m_ptrDescriptorSetLayout->GetHandle();
		VkDescriptorPool vkDescriptorPool = m_pDescriptorPool->GetDescriptorPool();
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = NULL;
		allocInfo.descriptorPool = vkDescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &vkDescriptorSetLayout;
		vkAllocateDescriptorSets(m_pDevice->GetDevice(), &allocInfo, &m_vkDescriptorSet);
	}

	CVulkanDescriptorSet::~CVulkanDescriptorSet(void)
	{
		vkFreeDescriptorSets(m_pDevice->GetDevice(), m_pDescriptorPool->GetDescriptorPool(), 1, &m_vkDescriptorSet);
	}

	void CVulkanDescriptorSet::Release(void)
	{
		m_pDescriptorPool->FreeDescriptorSet(this);
	}

	CVulkanDevice* CVulkanDescriptorSet::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanDescriptorSet::GetHandle(void) const
	{
		return m_vkDescriptorSet;
	}

	void CVulkanDescriptorSet::SetTexture(uint32_t binding, const CGfxTexturePtr &ptrTexture)
	{
		m_bNeedUpdate = TRUE;
		m_ptrTextures[binding] = ptrTexture;
	}

	void CVulkanDescriptorSet::SetRenderTexture(uint32_t binding, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		m_bNeedUpdate = TRUE;
		m_ptrRenderTextures[binding] = ptrRenderTexture;
	}

	void CVulkanDescriptorSet::SetUniformBuffer(uint32_t binding, const CGfxUniformBufferPtr &ptrUniformBuffer)
	{
		m_bNeedUpdate = TRUE;
		m_ptrUniformBuffers[binding] = ptrUniformBuffer;
		m_uniformBufferOffsets[binding] = 0;
	}

	void CVulkanDescriptorSet::SetUniformBufferData(uint32_t binding, size_t offset, size_t size, const void *pBuffer)
	{
		if (m_ptrUniformBuffers.find(binding) != m_ptrUniformBuffers.end()) {
			uint32_t base = m_ptrUniformBuffers[binding]->IsDynamic() ? GfxSwapChain()->GetImageIndex() * ALIGN_BYTE(m_ptrUniformBuffers[binding]->GetBufferSize(), UNIFORM_BUFFER_ALIGNMENT) : 0;
			m_ptrUniformBuffers[binding]->SetData(base + offset, size, pBuffer);
			m_uniformBufferOffsets[binding] = base;
		}
	}

	void CVulkanDescriptorSet::UpdateDescriptorSets(void)
	{
		if (m_bNeedUpdate) {
			m_bNeedUpdate = FALSE;

			std::vector<VkWriteDescriptorSet> writes;

			for (const auto &itTexture : m_ptrTextures) {
				const uint32_t binding = itTexture.first;
				const CGfxTexturePtr &ptrTexture = itTexture.second;

				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_vkDescriptorSet;
				write.dstBinding = binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				write.pImageInfo = &((CVulkanTexture *)((CGfxTexture *)ptrTexture))->GetDescriptorImageInfo();
				write.pBufferInfo = NULL;
				write.pTexelBufferView = NULL;

				writes.push_back(write);
			}

			for (const auto &itRenderTexture : m_ptrRenderTextures) {
				const uint32_t binding = itRenderTexture.first;
				const CGfxRenderTexturePtr &ptrRenderTexture = itRenderTexture.second;

				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_vkDescriptorSet;
				write.dstBinding = binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
				write.pImageInfo = &((CVulkanRenderTexture *)((CGfxRenderTexture *)ptrRenderTexture))->GetDescriptorImageInfo();
				write.pBufferInfo = NULL;
				write.pTexelBufferView = NULL;

				writes.push_back(write);
			}

			for (const auto &itUniformBuffer : m_ptrUniformBuffers) {
				const uint32_t binding = itUniformBuffer.first;
				const CGfxUniformBufferPtr &ptrUniformBuffer = itUniformBuffer.second;

				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_vkDescriptorSet;
				write.dstBinding = binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				write.pImageInfo = NULL;
				write.pBufferInfo = &((CVulkanUniformBuffer *)((CGfxUniformBuffer *)ptrUniformBuffer))->GetDescriptorBufferInfo();
				write.pTexelBufferView = NULL;

				writes.push_back(write);
			}

			vkUpdateDescriptorSets(m_pDevice->GetDevice(), writes.size(), writes.data(), 0, NULL);
		}
	}

	const uint32_t CVulkanDescriptorSet::GetSet(void) const
	{
		return m_ptrDescriptorSetLayout->GetSet();
	}

	const uint32_t* CVulkanDescriptorSet::GetTypesUsedCount(void) const
	{
		return m_ptrDescriptorSetLayout->GetTypesUsedCount();
	}

	const std::vector<uint32_t> CVulkanDescriptorSet::GetUniformBufferOffsets(void) const
	{
		std::vector<uint32_t> offsets;

		for (const auto &itOffset : m_uniformBufferOffsets) {
			offsets.push_back(itOffset.second);
		}

		return offsets;
	}

}
