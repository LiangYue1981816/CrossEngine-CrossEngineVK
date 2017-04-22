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

	CVulkanDescriptorSetLayout::CVulkanDescriptorSetLayout(CVulkanDevice *pDevice, CVulkanResourceManager *pManager)
		: CVulkanResource(pDevice, pManager)
		, m_vkDescriptorSetLayout(VK_NULL_HANDLE)
		, m_numTypesUsedCount{ 0 }
	{

	}

	CVulkanDescriptorSetLayout::~CVulkanDescriptorSetLayout(void)
	{
		ASSERT(m_vkDescriptorSetLayout == VK_NULL_HANDLE);
	}

	BOOL CVulkanDescriptorSetLayout::Create(void)
	{
		try {
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			CALL_BOOL_FUNCTION_THROW(CreateBindings(bindings));

			VkDescriptorSetLayoutCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.bindingCount = bindings.size();
			createInfo.pBindings = bindings.data();
			CALL_VK_FUNCTION_THROW(vkCreateDescriptorSetLayout(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorSetLayout));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanDescriptorSetLayout::CreateBindings(std::vector<VkDescriptorSetLayoutBinding> &bindings)
	{
		bindings.clear();

		for (const auto &itBinding : m_bindings) {
			bindings.push_back(itBinding.second);
		}

		return TRUE;
	}

	void CVulkanDescriptorSetLayout::Destroy(void)
	{
		if (m_vkDescriptorSetLayout) {
			vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_vkDescriptorSetLayout, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkDescriptorSetLayout = VK_NULL_HANDLE;
		m_bindings.clear();
	}

	BOOL CVulkanDescriptorSetLayout::SetBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags /*= VK_SHADER_STAGE_ALL*/)
	{
		m_numTypesUsedCount[type]++;

		m_bindings[binding].binding = binding;
		m_bindings[binding].descriptorType = type;
		m_bindings[binding].descriptorCount = 1;
		m_bindings[binding].stageFlags = flags;
		m_bindings[binding].pImmutableSamplers = NULL;

		return TRUE;
	}

	VkDescriptorSetLayout CVulkanDescriptorSetLayout::GetLayout(void) const
	{
		return m_vkDescriptorSetLayout;
	}

	const uint32_t* CVulkanDescriptorSetLayout::GetTypesUsedCount(void) const
	{
		return m_numTypesUsedCount;
	}

	uint32_t CVulkanDescriptorSetLayout::GetTypeUsedCount(VkDescriptorType type) const
	{
		return m_numTypesUsedCount[type];
	}

	void CVulkanDescriptorSetLayout::DumpLog(void) const
	{

	}

}
