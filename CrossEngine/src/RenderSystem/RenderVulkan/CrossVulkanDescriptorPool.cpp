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

		, m_numDescriptorSets(0)
		, m_maxDescriptorSets(0)
		, m_numAllocatedTypes{ 0 }
		, m_maxAllocatedTypes{ 0 }

		, pNext(NULL)
	{
		m_maxDescriptorSets = 16384;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = 4096;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = 4096;
		m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = 256;

		VkDescriptorPoolSize poolSizes[3] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] },
		};

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = m_maxDescriptorSets;
		createInfo.poolSizeCount = 3;
		createInfo.pPoolSizes = poolSizes;
		vkCreateDescriptorPool(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool);
	}

	CVulkanDescriptorPool::~CVulkanDescriptorPool(void)
	{
		ASSERT(m_pDescriptorSets.empty());

		for (auto &itDescriptorSet : m_pDescriptorSets) {
			SAFE_DELETE(itDescriptorSet.second);
		}

		vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
	}

	CVulkanDescriptorSet* CVulkanDescriptorPool::AllocDescriptorSet(const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout)
	{
		if (m_numDescriptorSets + 1 > m_maxDescriptorSets) {
			return NULL;
		}

		for (uint32_t index = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; index < VK_DESCRIPTOR_TYPE_END_RANGE; index++) {
			if (m_numAllocatedTypes[index] + ptrDescriptorSetLayout->GetTypesUsedCount()[index] > m_maxAllocatedTypes[index]) {
				return NULL;
			}
		}

		CVulkanDescriptorSet *pDescriptorSet = SAFE_NEW CVulkanDescriptorSet(m_pDevice, this, ptrDescriptorSetLayout);
		m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;

		m_numDescriptorSets++;
		for (uint32_t index = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; index < VK_DESCRIPTOR_TYPE_END_RANGE; index++) {
			m_numAllocatedTypes[index] += pDescriptorSet->GetTypesUsedCount()[index];
		}

		return pDescriptorSet;
	}

	void CVulkanDescriptorPool::FreeDescriptorSet(CVulkanDescriptorSet *pDescriptorSet)
	{
		if (pDescriptorSet) {
			m_numDescriptorSets--;
			for (uint32_t index = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; index < VK_DESCRIPTOR_TYPE_END_RANGE; index++) {
				m_numAllocatedTypes[index] -= pDescriptorSet->GetTypesUsedCount()[index];
			}

			m_pDescriptorSets.erase(pDescriptorSet);
			SAFE_DELETE(pDescriptorSet);
		}
	}

	void CVulkanDescriptorPool::ResetDescriptorSetPool(void)
	{
		vkResetDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, 0);
	}

	uint32_t CVulkanDescriptorPool::GetDescriptorSetCount(void) const
	{
		return m_numDescriptorSets;
	}

	VkDescriptorPool CVulkanDescriptorPool::GetDescriptorPool(void) const
	{
		return m_vkDescriptorPool;
	}

	void CVulkanDescriptorPool::DumpLog(void) const
	{
		LOGI("\t\tDescriptorSet = %d/%d\n", m_numDescriptorSets, m_maxDescriptorSets);
		for (uint32_t index = VK_DESCRIPTOR_TYPE_BEGIN_RANGE; index < VK_DESCRIPTOR_TYPE_END_RANGE; index++) {
			LOGI("\t\t\t%s = %d/%d\n", CVulkanHelper::DescriptorTypeToString((VkDescriptorType)index), m_numAllocatedTypes[index], m_maxAllocatedTypes[index]);
		}
	}

}
