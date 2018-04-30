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
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(m_pDevice, set);
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
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(m_pDevice, set);
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
					m_pDescriptorSetLayouts[set] = SAFE_NEW CGLES3DescriptorSetLayout(m_pDevice, set);
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

	const CGfxShaderPtr& CGLES3Pipeline::GetShader(VkShaderStageFlags flags) const
	{
		static const CGfxShaderPtr ptrShaderNull;
		const auto &itShader = m_ptrShaders.find(flags);
		return itShader != m_ptrShaders.end() ? itShader->second : ptrShaderNull;
	}

	const CGfxDescriptorSetLayout* CGLES3Pipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		set = 0; // GLES3 not support DescriptorSet!!!
		const auto &itDescriptorSetLayout = m_pDescriptorSetLayouts.find(set);
		return itDescriptorSetLayout != m_pDescriptorSetLayouts.end() ? itDescriptorSetLayout->second : NULL;
	}

}
