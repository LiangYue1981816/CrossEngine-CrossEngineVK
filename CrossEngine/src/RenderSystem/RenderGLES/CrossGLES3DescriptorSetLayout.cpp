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

	CGLES3DescriptorSetLayout::CGLES3DescriptorSetLayout(CGLES3Device *pDevice, uint32_t set)
		: m_pDevice(pDevice)

		, m_set(set)
		, m_numTypesUsedCount{ 0 }
	{

	}

	CGLES3DescriptorSetLayout::~CGLES3DescriptorSetLayout(void)
	{

	}

	void CGLES3DescriptorSetLayout::Release(void)
	{
		CGLES3DescriptorSetLayout *pVulkanDescriptorSetLayout = this;
		Destroy();
		SAFE_DELETE(pVulkanDescriptorSetLayout);
	}

	CGLES3Device* CGLES3DescriptorSetLayout::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3DescriptorSetLayout::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	BOOL CGLES3DescriptorSetLayout::Create(void)
	{
		return TRUE;
	}

	void CGLES3DescriptorSetLayout::Destroy(void)
	{

	}

	BOOL CGLES3DescriptorSetLayout::SetUniformBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_uniformBlockNameBindings[szName] = binding;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	BOOL CGLES3DescriptorSetLayout::SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_sampledImageNameBindings[szName] = binding;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	BOOL CGLES3DescriptorSetLayout::SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
	{
		uint32_t dwName = HashValue(szName);
		VkDescriptorType type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		if (m_nameBindings.find(dwName) == m_nameBindings.end()) {
			m_nameBindings[dwName] = binding;
			m_inputAttachmentNameBindings[szName] = binding;
			m_numTypesUsedCount[type]++;
		}

		return TRUE;
	}

	uint32_t CGLES3DescriptorSetLayout::GetSet(void) const
	{
		return m_set;
	}

	uint32_t CGLES3DescriptorSetLayout::GetBinding(uint32_t dwName) const
	{
		const auto &itName = m_nameBindings.find(dwName);
		return itName != m_nameBindings.end() ? itName->second : -1;
	}

	const uint32_t* CGLES3DescriptorSetLayout::GetTypesUsedCount(void) const
	{
		return m_numTypesUsedCount;
	}

	const std::map<uint32_t, uint32_t>& CGLES3DescriptorSetLayout::GetUniformBlockBindings(GLuint program)
	{
		if (m_uniformBlockBindings.find(program) == m_uniformBlockBindings.end()) {
			for (const auto &itNameBinding : m_uniformBlockNameBindings) {
				GLuint binding = itNameBinding.second;
				GLuint location = glGetUniformBlockIndex(program, itNameBinding.first.c_str());

				if (location != GL_INVALID_INDEX) {
					m_uniformBlockBindings[program][binding] = location;
				}
			}
		}

		return m_uniformBlockBindings[program];
	}

	const std::map<uint32_t, uint32_t>& CGLES3DescriptorSetLayout::GetSampledImageBindings(GLuint program)
	{
		if (m_sampledImageBindings.find(program) == m_sampledImageBindings.end()) {
			for (const auto &itNameBinding : m_sampledImageNameBindings) {
				GLuint binding = itNameBinding.second;
				GLuint location = glGetUniformLocation(program, itNameBinding.first.c_str());

				if (location != GL_INVALID_INDEX) {
					m_sampledImageBindings[program][binding] = location;
				}
			}
		}

		return m_sampledImageBindings[program];
	}

	const std::map<uint32_t, uint32_t>& CGLES3DescriptorSetLayout::GetInputAttachmentBindings(GLuint program)
	{
		if (m_inputAttachmentBindings.find(program) == m_inputAttachmentBindings.end()) {
			for (const auto &itNameBinding : m_inputAttachmentNameBindings) {
				GLuint binding = itNameBinding.second;
				GLuint location = glGetUniformLocation(program, itNameBinding.first.c_str());

				if (location != GL_INVALID_INDEX) {
					m_inputAttachmentBindings[program][binding] = location;
				}
			}
		}

		return m_inputAttachmentBindings[program];
	}

}
