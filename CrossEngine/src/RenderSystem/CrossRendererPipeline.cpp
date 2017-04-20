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

	CRendererPipeline::CRendererPipeline(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkPipeline(VK_NULL_HANDLE)
		, m_vkPipelineLayout(VK_NULL_HANDLE)
	{
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].stage = VK_SHADER_STAGE_VERTEX_BIT;

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].stage = VK_SHADER_STAGE_GEOMETRY_BIT;

		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT] = {};
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_COMPUTE_BIT].stage = VK_SHADER_STAGE_COMPUTE_BIT;
	}

	CRendererPipeline::~CRendererPipeline(void)
	{
		ASSERT(m_vkPipeline == VK_NULL_HANDLE);
		ASSERT(m_vkPipelineLayout == VK_NULL_HANDLE);
	}

	void CRendererPipeline::Destroy(void)
	{
		if (m_vkPipeline) {
			vkDestroyPipeline(m_pDevice->GetDevice(), m_vkPipeline, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_vkPipelineLayout) {
			vkDestroyPipelineLayout(m_pDevice->GetDevice(), m_vkPipelineLayout, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkPipeline = VK_NULL_HANDLE;
		m_vkPipelineLayout = VK_NULL_HANDLE;
	}

	void CRendererPipeline::DumpLog(void) const
	{
		if (m_vkPipeline) {
			LOGI("\t\tPipeline 0x%x\n", m_vkPipeline);
		}
	}

	VkPipeline CRendererPipeline::GetPipeline(void) const
	{
		return m_vkPipeline;
	}

	VkPipelineLayout CRendererPipeline::GetPipelineLayout(void) const
	{
		return m_vkPipelineLayout;
	}

	const VkBinding* CRendererPipeline::GetDescriptorSetLayout(VkDescriptorType type, const char *szName) const
	{
		const auto &itTypeBindings = m_bindings.find(type);
		if (itTypeBindings == m_bindings.end()) return NULL;

		const auto &itNameBinding = itTypeBindings->second.find(szName);
		return itNameBinding != itTypeBindings->second.end() ? &itNameBinding->second : NULL;
	}

	BOOL CRendererPipeline::CreateDescriptorSetLayouts(std::vector<VkDescriptorSetLayout> &layouts)
	{
		layouts.clear();
		m_bindings.clear();
		m_pDescriptorSetLayouts.clear();

		std::map<uint32_t, std::map<uint32_t, std::string>> names;
		std::map<uint32_t, std::map<uint32_t, VkDescriptorType>> types;
		std::map<uint32_t, CRendererDescriptorSetLayout*> pDescriptorSetLayouts;

		for (const auto &itMoudle : m_shaderModules) {
			for (const auto &variable : itMoudle.second.variables) {
				if (variable.second.storage_class != SpvStorageClassUniform &&
					variable.second.storage_class != SpvStorageClassUniformConstant) {
					continue;
				}

				uint32_t set = variable.second.descriptor_set;
				uint32_t binding = variable.second.binding;
				VkShaderStageFlags shaderStageFlags = itMoudle.first;

				if (pDescriptorSetLayouts[set] == NULL) {
					pDescriptorSetLayouts[set] = m_pDevice->GetDescriptorSetLayoutManager()->AllocDescriptorSetLayout();
				}

				if (variable.second.storage_class == SpvStorageClassUniform) {
					types[set][binding] = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					names[set][binding] = itMoudle.second.struct_types.at(variable.second.type_id).name;
					pDescriptorSetLayouts[set]->SetBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStageFlags);
				}

				if (variable.second.storage_class == SpvStorageClassUniformConstant) {
					types[set][binding] = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					names[set][binding] = variable.second.name;
					pDescriptorSetLayouts[set]->SetBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, shaderStageFlags);
				}
			}
		}

		for (const auto &itDescriptorSetLayout : pDescriptorSetLayouts) {
			if (CRendererDescriptorSetLayout *pDescriptorSetLayout = itDescriptorSetLayout.second) {
				pDescriptorSetLayout->Create();

				for (const auto &itName : names[itDescriptorSetLayout.first]) {
					uint32_t set = itDescriptorSetLayout.first;
					uint32_t binding = itName.first;
					const std::string &name = itName.second;

					if (types[set][binding] == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
						m_bindings[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER][name].binding = binding;
						m_bindings[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER][name].vkDescriptorSetLayout = pDescriptorSetLayout->GetSetLayout();
					}

					if (types[set][binding] == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
						m_bindings[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER][name].binding = binding;
						m_bindings[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER][name].vkDescriptorSetLayout = pDescriptorSetLayout->GetSetLayout();
					}
				}

				m_pDescriptorSetLayouts.push_back(pDescriptorSetLayout);
				layouts.push_back(pDescriptorSetLayout->GetSetLayout());
			}
		}

		return TRUE;
	}

	BOOL CRendererPipeline::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages)
	{
		shaderStages.clear();

		for (const auto &itShaderStage : m_shaderStages) {
			if (itShaderStage.second.module != VK_NULL_HANDLE) {
				shaderStages.push_back(itShaderStage.second);
			}
		}

		return TRUE;
	}

}
