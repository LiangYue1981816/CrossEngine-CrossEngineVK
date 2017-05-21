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

	CVulkanDescriptorPool::CVulkanDescriptorPool(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkDescriptorPool(VK_NULL_HANDLE)

		, m_maxDescriptorSets(0)
		, m_numDescriptorSets(0)
		, m_maxAllocatedTypes{ 0 }
		, m_numAllocatedTypes{ 0 }

		, pNext(NULL)
	{
		m_maxDescriptorSets = 16384;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] = 2048;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = 2048;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_SAMPLER] = 1024;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = 4096;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] = 512;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] = 512;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] = 512;

		VkDescriptorPoolSize poolSizes[7] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] },
			{ VK_DESCRIPTOR_TYPE_SAMPLER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_SAMPLER] },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] },
		};

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = m_maxDescriptorSets;
		createInfo.poolSizeCount = 7;
		createInfo.pPoolSizes = poolSizes;
		vkCreateDescriptorPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool);
	}

	CVulkanDescriptorPool::~CVulkanDescriptorPool(void)
	{
		for (const auto &itDescriptorSet : m_pDescriptorSets) {
			if (CVulkanDescriptorSet *pDescriptorSet = itDescriptorSet.second) {
				SAFE_DELETE(pDescriptorSet);
			}
		}

		vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
	}

	CVulkanDescriptorSet* CVulkanDescriptorPool::AllocDescriptorSet(VkDescriptorSetLayout vkSetLayout, const uint32_t *typesUsedCount)
	{
		if (m_numDescriptorSets + 1 > m_maxDescriptorSets) {
			return NULL;
		}

		for (uint32_t index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
			if (m_numAllocatedTypes[index] + typesUsedCount[index] > m_maxAllocatedTypes[index]) {
				return NULL;
			}
		}

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = NULL;
		allocInfo.descriptorPool = m_vkDescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &vkSetLayout;

		VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(m_pDevice->GetDevice(), &allocInfo, &vkDescriptorSet);
		if (result != VK_SUCCESS) return NULL;

		CVulkanDescriptorSet *pDescriptorSet = SAFE_NEW CVulkanDescriptorSet(this, m_pDevice, vkDescriptorSet, typesUsedCount);
		m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;

		m_numDescriptorSets++;
		for (uint32_t index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
			m_numAllocatedTypes[index] += pDescriptorSet->GetTypesUsedCount()[index];
		}

		return pDescriptorSet;
	}

	void CVulkanDescriptorPool::FreeDescriptorSet(CVulkanDescriptorSet *pDescriptorSet)
	{
		ASSERT(pDescriptorSet->GetDescriptorPool() == this);

		const auto &itDescriptor = m_pDescriptorSets.find(pDescriptorSet);
		if (itDescriptor != m_pDescriptorSets.end()) m_pDescriptorSets.erase(itDescriptor);

		VkDescriptorSet vkDescriptorSet = pDescriptorSet->GetDescriptorSet();
		vkFreeDescriptorSets(m_pDevice->GetDevice(), m_vkDescriptorPool, 1, &vkDescriptorSet);

		m_numDescriptorSets--;
		for (uint32_t index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
			m_numAllocatedTypes[index] -= pDescriptorSet->GetTypesUsedCount()[index];
		}

		SAFE_DELETE(pDescriptorSet);
	}

	uint32_t CVulkanDescriptorPool::GetDescriptorSetCount(void) const
	{
		return m_numDescriptorSets;
	}

	void CVulkanDescriptorPool::DumpLog(void) const
	{
		LOGI("\t\tDescriptorSet = %d/%d\n", m_numDescriptorSets, m_maxDescriptorSets);
		for (uint32_t index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
			LOGI("\t\t\t%s = %d/%d\n", CVulkanHelper::vkDescriptorTypeToString((VkDescriptorType)index), m_numAllocatedTypes[index], m_maxAllocatedTypes[index]);
		}
	}

}
