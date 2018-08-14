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

	CVulkanPipeline::CVulkanPipeline(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkPipeline(VK_NULL_HANDLE)
		, m_vkPipelineLayout(VK_NULL_HANDLE)
	{
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].stage = VK_SHADER_STAGE_VERTEX_BIT;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pName = "main";

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pName = "main";

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pName = "main";

		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pName = "main";

		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pName = "main";

		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].stage = VK_SHADER_STAGE_COMPUTE_BIT;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].pName = "main";
	}

	CVulkanPipeline::~CVulkanPipeline(void)
	{

	}

	BOOL CVulkanPipeline::CreateDescriptorSetLayouts(eastl::vector<VkDescriptorSetLayout> &layouts)
	{
		layouts.clear();

		m_ptrDescriptorSetLayouts[DESCRIPTOR_SET_DRAW] = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_DRAW);
		m_ptrDescriptorSetLayouts[DESCRIPTOR_SET_PASS] = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_PASS);
		m_ptrDescriptorSetLayouts[DESCRIPTOR_SET_FRAME] = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_FRAME);

		for (const auto &itShader : m_ptrShaders) {
			const spirv_cross::Compiler *pShaderCompiler = itShader.second->GetShaderCompiler();
			const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

			for (const auto &itUniform : shaderResources.uniform_buffers) {
				const uint32_t set = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itUniform.type_id);

				if (m_ptrDescriptorSetLayouts[set].IsNull()) {
					m_ptrDescriptorSetLayouts[set] = GfxDevice()->AllocDescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::Struct) {
					m_ptrDescriptorSetLayouts[set]->SetUniformBinding(itUniform.name.c_str(), binding, VK_SHADER_STAGE_ALL);
				}
			}

			for (const auto &itSampledImage : shaderResources.sampled_images) {
				const uint32_t set = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itSampledImage.type_id);

				if (m_ptrDescriptorSetLayouts[set].IsNull()) {
					m_ptrDescriptorSetLayouts[set] = GfxDevice()->AllocDescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::SampledImage) {
					m_ptrDescriptorSetLayouts[set]->SetSampledImageBinding(itSampledImage.name.c_str(), binding, VK_SHADER_STAGE_ALL);
				}
			}

			for (const auto &itSubpassInput : shaderResources.subpass_inputs) {
				const uint32_t set = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationDescriptorSet);
				const uint32_t binding = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationBinding);
				const spirv_cross::SPIRType type = pShaderCompiler->get_type(itSubpassInput.type_id);

				if (m_ptrDescriptorSetLayouts[set].IsNull()) {
					m_ptrDescriptorSetLayouts[set] = GfxDevice()->AllocDescriptorSetLayout(set);
				}

				if (type.basetype == spirv_cross::SPIRType::Image) {
					m_ptrDescriptorSetLayouts[set]->SetInputAttachmentBinding(itSubpassInput.name.c_str(), binding, VK_SHADER_STAGE_ALL);
				}
			}
		}

		for (auto &itDescriptorSetLayout : m_ptrDescriptorSetLayouts) {
			itDescriptorSetLayout.second->Create();
			layouts.emplace_back((VkDescriptorSetLayout)itDescriptorSetLayout.second->GetHandle());
		}

		return TRUE;
	}

	BOOL CVulkanPipeline::CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaderStages)
	{
		shaderStages.clear();

		for (const auto &itShaderStage : m_shaderStages) {
			if (itShaderStage.second.module) {
				shaderStages.emplace_back(itShaderStage.second);
			}
		}

		return TRUE;
	}

	void CVulkanPipeline::Destroy(void)
	{
		DestroyDescriptorSetLayouts();
		DestroyShaderStages();

		if (m_vkPipeline) {
			vkDestroyPipeline(m_pDevice->GetDevice(), m_vkPipeline, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_vkPipelineLayout) {
			vkDestroyPipelineLayout(m_pDevice->GetDevice(), m_vkPipelineLayout, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkPipeline = VK_NULL_HANDLE;
		m_vkPipelineLayout = VK_NULL_HANDLE;
	}

	void CVulkanPipeline::DestroyDescriptorSetLayouts(void)
	{
		m_ptrDescriptorSetLayouts.clear();
	}

	void CVulkanPipeline::DestroyShaderStages(void)
	{
		m_ptrShaders.clear();
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].module = VK_NULL_HANDLE;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].module = VK_NULL_HANDLE;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].module = VK_NULL_HANDLE;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].module = VK_NULL_HANDLE;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].module = VK_NULL_HANDLE;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].module = VK_NULL_HANDLE;
	}

	VkPipelineLayout CVulkanPipeline::GetPipelineLayout(void) const
	{
		return m_vkPipelineLayout;
	}

	const CGfxDescriptorSetLayoutPtr& CVulkanPipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		static const CGfxDescriptorSetLayoutPtr ptrDescriptorSetLayoutNull;
		const auto &itDescriptorSetLayout = m_ptrDescriptorSetLayouts.find(set);
		return itDescriptorSetLayout != m_ptrDescriptorSetLayouts.end() ? itDescriptorSetLayout->second : ptrDescriptorSetLayoutNull;
	}

}
