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

	CVulkanDescriptorSetLayout::CVulkanDescriptorSetLayout(CVulkanDevice *pDevice, uint32_t set)
		: m_pDevice(pDevice)
		, m_vkDescriptorSetLayout(VK_NULL_HANDLE)

		, m_set(set)
		, m_numTypesUsedCount{ 0 }
	{

	}

	CVulkanDescriptorSetLayout::~CVulkanDescriptorSetLayout(void)
	{

	}

	void CVulkanDescriptorSetLayout::Release(void)
	{
		CVulkanDescriptorSetLayout *pVulkanDescriptorSetLayout = this;
		SAFE_DELETE(pVulkanDescriptorSetLayout);
	}

	CVulkanDevice* CVulkanDescriptorSetLayout::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanDescriptorSetLayout::GetHandle(void) const
	{
		return m_vkDescriptorSetLayout;
	}

	BOOL CVulkanDescriptorSetLayout::Create(void)
	{
		try {
			std::vector<VkDescriptorSetLayoutBinding> bindings;
			for (const auto &itBinding : m_layoutBindings) {
				bindings.push_back(itBinding.second);
			}

			VkDescriptorSetLayoutCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.bindingCount = bindings.size();
			createInfo.pBindings = bindings.data();
			CALL_VK_FUNCTION_THROW(vkCreateDescriptorSetLayout(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorSetLayout));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanDescriptorSetLayout::Destroy(void)
	{
		if (m_vkDescriptorSetLayout) {
			vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_vkDescriptorSetLayout, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_nameBindings.clear();
		m_layoutBindings.clear();
		m_vkDescriptorSetLayout = VK_NULL_HANDLE;

		memset(m_numTypesUsedCount, 0, sizeof(m_numTypesUsedCount));
	}

	BOOL CVulkanDescriptorSetLayout::SetUniformBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_layoutBindings[binding].binding = binding;
			m_layoutBindings[binding].descriptorType = type;
			m_layoutBindings[binding].descriptorCount = 1;
			m_layoutBindings[binding].stageFlags = flags;
			m_layoutBindings[binding].pImmutableSamplers = NULL;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	BOOL CVulkanDescriptorSetLayout::SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_layoutBindings[binding].binding = binding;
			m_layoutBindings[binding].descriptorType = type;
			m_layoutBindings[binding].descriptorCount = 1;
			m_layoutBindings[binding].stageFlags = flags;
			m_layoutBindings[binding].pImmutableSamplers = NULL;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	BOOL CVulkanDescriptorSetLayout::SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_layoutBindings[binding].binding = binding;
			m_layoutBindings[binding].descriptorType = type;
			m_layoutBindings[binding].descriptorCount = 1;
			m_layoutBindings[binding].stageFlags = flags;
			m_layoutBindings[binding].pImmutableSamplers = NULL;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	uint32_t CVulkanDescriptorSetLayout::GetSet(void) const
	{
		return m_set;
	}

	uint32_t CVulkanDescriptorSetLayout::GetBinding(uint32_t dwName) const
	{
		const auto &itName = m_nameBindings.find(dwName);
		return itName != m_nameBindings.end() ? itName->second : -1;
	}

	const uint32_t* CVulkanDescriptorSetLayout::GetTypesUsedCount(void) const
	{
		return m_numTypesUsedCount;
	}

}
