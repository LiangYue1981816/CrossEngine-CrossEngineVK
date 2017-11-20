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

	CGLES3DescriptorSetLayout::CGLES3DescriptorSetLayout(uint32_t set)
		: m_set(set)
	{

	}

	CGLES3DescriptorSetLayout::~CGLES3DescriptorSetLayout(void)
	{

	}

	BOOL CGLES3DescriptorSetLayout::SetUniformBinding(const char *szName, uint32_t binding, GLuint program)
	{
		uint32_t dwName = HashValue(szName);
		GLuint location = glGetUniformBlockIndex(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[dwName] = binding;
			m_uniformBlockBindings[program][binding] = location;
			return TRUE;
		}

		return FALSE;
	}

	BOOL CGLES3DescriptorSetLayout::SetSampledImageBinding(const char *szName, uint32_t binding, GLuint program)
	{
		uint32_t dwName = HashValue(szName);
		GLuint location = glGetUniformLocation(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[dwName] = binding;
			m_sampledImageBindings[program][binding] = location;
			return TRUE;
		}

		return FALSE;
	}

	BOOL CGLES3DescriptorSetLayout::SetInputAttachmentBinding(const char *szName, uint32_t binding, GLuint program)
	{
		uint32_t dwName = HashValue(szName);
		GLuint location = glGetUniformLocation(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[dwName] = binding;
			m_inputAttachmentBindings[program][binding] = location;
			return TRUE;
		}

		return FALSE;
	}

	uint32_t CGLES3DescriptorSetLayout::GetSet(void) const
	{
		return m_set;
	}

	uint32_t CGLES3DescriptorSetLayout::GetBinding(uint32_t dwName) const
	{
		const auto &itName = m_names.find(dwName);
		return itName != m_names.end() ? itName->second : -1;
	}

	const std::map<uint32_t, std::map<uint32_t, uint32_t>>& CGLES3DescriptorSetLayout::GetUniformBlockBindings(void) const
	{
		return m_uniformBlockBindings;
	}

	const std::map<uint32_t, std::map<uint32_t, uint32_t>>& CGLES3DescriptorSetLayout::GetSampledImageBindings(void) const
	{
		return m_sampledImageBindings;
	}

	const std::map<uint32_t, std::map<uint32_t, uint32_t>>& CGLES3DescriptorSetLayout::GetInputAttachmentBindings(void) const
	{
		return m_inputAttachmentBindings;
	}


	CGLES3Pipeline::CGLES3Pipeline(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_pipeline(0)
	{

	}

	CGLES3Pipeline::~CGLES3Pipeline(void)
	{

	}

	BOOL CGLES3Pipeline::CreateDescriptorSetLayouts(void)
	{
		for (const auto &itShader : m_ptrShaders) {
			const VkShaderStageFlags shaderStageFlags = itShader.first;
			const spirv_cross::Compiler *pShaderCompiler = itShader.second->GetShaderCompiler();
			const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();
			const GLuint program = (GLuint)itShader.second->GetHandle();

			for (const auto &itUniform : shaderResources.uniform_buffers) {
				const uint32_t set = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itUniform.type_id);

				if (m_pDescriptorSetLayouts[set] == NULL) {
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::Struct) {
					m_pDescriptorSetLayouts[set]->SetUniformBinding(itUniform.name.c_str(), binding, program);
				}
			}

			for (const auto &itSampledImage : shaderResources.sampled_images) {
				const uint32_t set = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itSampledImage.type_id);

				if (m_pDescriptorSetLayouts[set] == NULL) {
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::SampledImage) {
					m_pDescriptorSetLayouts[set]->SetSampledImageBinding(itSampledImage.name.c_str(), binding, program);
				}
			}

			for (const auto &itSubpassInput : shaderResources.subpass_inputs) {
				const uint32_t set = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itSubpassInput.type_id);

				if (m_pDescriptorSetLayouts[set] == NULL) {
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::Image) {
					m_pDescriptorSetLayouts[set]->SetInputAttachmentBinding(itSubpassInput.name.c_str(), binding, program);
				}
			}
		}

		return TRUE;
	}

	void CGLES3Pipeline::Destroy(void)
	{
		DestroyDescriptorSetLayouts();

		if (m_pipeline) {
			glDeleteProgramPipelines(1, &m_pipeline);
		}

		m_pipeline = 0;
	}

	void CGLES3Pipeline::DestroyDescriptorSetLayouts(void)
	{
		for (auto &itDescriptorSetLayout : m_pDescriptorSetLayouts) {
			SAFE_DELETE(itDescriptorSetLayout.second);
		}

		m_pDescriptorSetLayouts.clear();
	}

	const CGLES3DescriptorSetLayout* CGLES3Pipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		const auto &itDescriptorSetLayout = m_pDescriptorSetLayouts.find(set);
		return itDescriptorSetLayout != m_pDescriptorSetLayouts.end() ? itDescriptorSetLayout->second : NULL;
	}

}
