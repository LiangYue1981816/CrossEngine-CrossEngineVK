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

	BOOL CGLES3DescriptorSetLayout::SetBinding(GLuint program, uint32_t binding, const char *szName)
	{
		GLint location = glGetUniformLocation(program, szName);

		if (location >= 0) {
			m_bindings[binding] = location;
		}

		return location >= 0 ? TRUE : FALSE;
	}

	uint32_t CGLES3DescriptorSetLayout::GetSet(void) const
	{
		return m_set;
	}

	const std::map<uint32_t, uint32_t>& CGLES3DescriptorSetLayout::GetBindings(void) const
	{
		return m_bindings;
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

			for (const auto &itUniform : shaderResources.uniform_buffers) {
				const uint32_t set = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
				const std::string name = pShaderCompiler->get_name(itUniform.id);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itUniform.type_id);

				if (m_pDescriptorSetLayouts[set] == NULL) {
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::Struct) {
					m_pDescriptorSetLayouts[set]->SetBinding((GLuint)itShader.second->GetHandle(), binding, name.c_str());
				}
			}

			for (const auto &itSampledImage : shaderResources.sampled_images) {
				const uint32_t set = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationBinding);
				const std::string name = pShaderCompiler->get_name(itSampledImage.id);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itSampledImage.type_id);

				if (m_pDescriptorSetLayouts[set] == NULL) {
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::SampledImage) {
					m_pDescriptorSetLayouts[set]->SetBinding((GLuint)itShader.second->GetHandle(), binding, name.c_str());
				}
			}
		}

		return TRUE;
	}

	void CGLES3Pipeline::DestroyDescriptorSetLayouts(void)
	{
		for (const auto &itDescriptorSetLayout : m_pDescriptorSetLayouts) {
			if (CGLES3DescriptorSetLayout* pDescriptorSetLayout = itDescriptorSetLayout.second) {
				SAFE_DELETE(pDescriptorSetLayout);
			}
		}

		m_pDescriptorSetLayouts.clear();
	}

	const CGLES3DescriptorSetLayout* CGLES3Pipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		const auto &itDescriptorSetLayout = m_pDescriptorSetLayouts.find(set);
		return itDescriptorSetLayout != m_pDescriptorSetLayouts.end() ? itDescriptorSetLayout->second : NULL;
	}

}
