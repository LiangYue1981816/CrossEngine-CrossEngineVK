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

	CRendererPipelineGraphics::CRendererPipelineGraphics(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererPipeline(pDevice, pManager)
		, m_vertexFormat(0)
		, m_vkPipelineLayout(VK_NULL_HANDLE)
	{
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].stage = VK_SHADER_STAGE_VERTEX_BIT;

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].stage = VK_SHADER_STAGE_GEOMETRY_BIT;

		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pNext = NULL;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].flags = 0;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		memset(&m_vertexInputState, 0, sizeof(m_vertexInputState));
		m_vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		m_vertexInputState.pNext = NULL;
		m_vertexInputState.flags = 0;

		memset(&m_inputAssemblyState, 0, sizeof(m_inputAssemblyState));
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.pNext = NULL;
		m_inputAssemblyState.flags = 0;

		memset(&m_tessellationState, 0, sizeof(m_tessellationState));
		m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_tessellationState.pNext = NULL;
		m_tessellationState.flags = 0;

		memset(&m_viewportState, 0, sizeof(m_viewportState));
		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.pNext = NULL;
		m_viewportState.flags = 0;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		memset(&m_rasterizationState, 0, sizeof(m_rasterizationState));
		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.pNext = NULL;
		m_rasterizationState.flags = 0;
		m_rasterizationState.lineWidth = 1.0f;

		memset(&m_multiSampleState, 0, sizeof(m_multiSampleState));
		m_multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multiSampleState.pNext = NULL;
		m_multiSampleState.flags = 0;

		memset(&m_depthStencilState, 0, sizeof(m_depthStencilState));
		m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_depthStencilState.pNext = NULL;
		m_depthStencilState.flags = 0;

		memset(&m_colorBlendState, 0, sizeof(m_colorBlendState));
		m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_colorBlendState.pNext = NULL;
		m_colorBlendState.flags = 0;

		memset(&m_dynamicState, 0, sizeof(m_dynamicState));
		static VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_DEPTH_BOUNDS, VK_DYNAMIC_STATE_STENCIL_REFERENCE };
		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pNext = NULL;
		m_dynamicState.flags = 0;
		m_dynamicState.dynamicStateCount = 4;
		m_dynamicState.pDynamicStates = dynamicStates;

		VkStencilOpState front, back;
		memset(&back, 0, sizeof(back));
		memset(&front, 0, sizeof(front));
		back.failOp = front.failOp = VK_STENCIL_OP_KEEP;
		back.passOp = front.passOp = VK_STENCIL_OP_KEEP;
		back.compareOp = front.compareOp = VK_COMPARE_OP_ALWAYS;

		SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
		SetTessellationPatchControlPoints(0);
		SetPolygonMode(VK_POLYGON_MODE_FILL);
		SetCullMode(VK_CULL_MODE_BACK_BIT);
		SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
		SetDepthClamp(VK_FALSE);
		SetDepthBias(VK_FALSE, 0.0f, 0.0f, 0.0f);
		SetRasterizerDiscard(VK_FALSE);
		SetSampleCounts(VK_SAMPLE_COUNT_1_BIT);
		SetSampleShading(VK_FALSE, 0.0f);
		SetSampleMask(NULL);
		SetSampleAlphaToCoverage(VK_FALSE);
		SetSampleAlphaToOne(VK_FALSE);
		SetDepthTest(TRUE, TRUE, VK_COMPARE_OP_LESS);
		SetDepthBoundsTest(VK_FALSE, 0.0f, 1.0f);
		SetStencilTest(VK_FALSE, front, back);
		SetColorBlendLogic(VK_FALSE, VK_LOGIC_OP_CLEAR);
		SetColorBlendConstants(0.0f, 0.0f, 0.0f, 0.0f);
	}

	CRendererPipelineGraphics::~CRendererPipelineGraphics(void)
	{

	}

	uint32_t CRendererPipelineGraphics::GetVertexFormat(void) const
	{
		return m_vertexFormat;
	}

	BOOL CRendererPipelineGraphics::SetVertexShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName)
	{
		m_shaderModules[VK_SHADER_STAGE_VERTEX_BIT] = module;

		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].module = vkShader;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pName = szName;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].pSpecializationInfo = NULL;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetTessellationControlShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName)
	{
		if (m_pDevice->GetDeviceFeatures().tessellationShader) {
			m_shaderModules[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = module;

			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].module = vkShader;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pName = szName;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].pSpecializationInfo = NULL;
		}

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetTessellationEvaluationShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName)
	{
		if (m_pDevice->GetDeviceFeatures().tessellationShader) {
			m_shaderModules[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = module;

			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].module = vkShader;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pName = szName;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].pSpecializationInfo = NULL;
		}

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetGeometryShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName)
	{
		if (m_pDevice->GetDeviceFeatures().geometryShader) {
			m_shaderModules[VK_SHADER_STAGE_GEOMETRY_BIT] = module;

			m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].module = vkShader;
			m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pName = szName;
			m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].pSpecializationInfo = NULL;
		}

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetFragmentShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName)
	{
		m_shaderModules[VK_SHADER_STAGE_FRAGMENT_BIT] = module;

		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].module = vkShader;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pName = szName;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].pSpecializationInfo = NULL;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetPrimitiveTopology(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable)
	{
		m_inputAssemblyState.topology = topology;
		m_inputAssemblyState.primitiveRestartEnable = primitiveRestartEnable;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetTessellationPatchControlPoints(uint32_t patchControlPoints)
	{
		m_tessellationState.patchControlPoints = patchControlPoints;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetPolygonMode(VkPolygonMode polygonMode)
	{
		m_rasterizationState.polygonMode = polygonMode;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetCullMode(VkCullModeFlags cullMode)
	{
		m_rasterizationState.cullMode = cullMode;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetFrontFace(VkFrontFace frontFace)
	{
		m_rasterizationState.frontFace = frontFace;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetDepthClamp(VkBool32 depthClampEnable)
	{
		if (depthClampEnable) {
			depthClampEnable = m_pDevice->GetDeviceFeatures().depthClamp;
		}

		m_rasterizationState.depthClampEnable = depthClampEnable;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetDepthBias(VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		m_rasterizationState.depthBiasEnable = depthBiasEnable;
		m_rasterizationState.depthBiasConstantFactor = depthBiasConstantFactor;
		m_rasterizationState.depthBiasClamp = depthBiasClamp;
		m_rasterizationState.depthBiasSlopeFactor = depthBiasSlopeFactor;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetRasterizerDiscard(VkBool32 rasterizerDiscardEnable)
	{
		m_rasterizationState.rasterizerDiscardEnable = rasterizerDiscardEnable;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetSampleCounts(VkSampleCountFlagBits rasterizationSamples)
	{
		m_multiSampleState.rasterizationSamples = rasterizationSamples;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetSampleShading(VkBool32 sampleShadingEnable, float minSampleShading)
	{
		if (sampleShadingEnable) {
			sampleShadingEnable = m_pDevice->GetDeviceFeatures().sampleRateShading;
		}

		m_multiSampleState.sampleShadingEnable = sampleShadingEnable;
		m_multiSampleState.minSampleShading = minSampleShading;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetSampleMask(const VkSampleMask* pSampleMask)
	{
		m_multiSampleState.pSampleMask = pSampleMask;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetSampleAlphaToCoverage(VkBool32 alphaToCoverageEnable)
	{
		m_multiSampleState.alphaToCoverageEnable = alphaToCoverageEnable;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetSampleAlphaToOne(VkBool32 alphaToOneEnable)
	{
		m_multiSampleState.alphaToOneEnable = alphaToOneEnable;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetDepthTest(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp)
	{
		m_depthStencilState.depthTestEnable = depthTestEnable;
		m_depthStencilState.depthWriteEnable = depthWriteEnable;
		m_depthStencilState.depthCompareOp = depthCompareOp;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetDepthBoundsTest(VkBool32 depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds)
	{
		if (depthBoundsTestEnable) {
			depthBoundsTestEnable = m_pDevice->GetDeviceFeatures().depthBounds;
		}

		m_depthStencilState.depthBoundsTestEnable = depthBoundsTestEnable;
		m_depthStencilState.minDepthBounds = minDepthBounds;
		m_depthStencilState.maxDepthBounds = maxDepthBounds;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetStencilTest(VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back)
	{
		m_depthStencilState.stencilTestEnable = stencilTestEnable;
		m_depthStencilState.front = front;
		m_depthStencilState.back = back;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetColorBlendLogic(VkBool32 logicOpEnable, VkLogicOp logicOp)
	{
		m_colorBlendState.logicOpEnable = logicOpEnable;
		m_colorBlendState.logicOp = logicOp;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetColorBlendConstants(float r, float g, float b, float a)
	{
		m_colorBlendState.blendConstants[0] = r;
		m_colorBlendState.blendConstants[1] = g;
		m_colorBlendState.blendConstants[2] = b;
		m_colorBlendState.blendConstants[3] = a;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetColorBlendAttachment(uint32_t attachment, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask)
	{
		m_colorBlendAttachmentStates[attachment].blendEnable = blendEnable;
		m_colorBlendAttachmentStates[attachment].srcColorBlendFactor = srcColorBlendFactor;
		m_colorBlendAttachmentStates[attachment].dstColorBlendFactor = dstColorBlendFactor;
		m_colorBlendAttachmentStates[attachment].colorBlendOp = colorBlendOp;
		m_colorBlendAttachmentStates[attachment].srcAlphaBlendFactor = srcAlphaBlendFactor;
		m_colorBlendAttachmentStates[attachment].dstAlphaBlendFactor = dstAlphaBlendFactor;
		m_colorBlendAttachmentStates[attachment].alphaBlendOp = alphaBlendOp;
		m_colorBlendAttachmentStates[attachment].colorWriteMask = colorWriteMask;

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetUniformImage(const char *szName, VkSampler vkSampler, VkImageView vkImageView, VkImageLayout vkImageLayout)
	{
		const auto &itImage = m_images.find(szName);
		if (itImage == m_images.end()) return FALSE;

		itImage->second.vkDescriptorImageInfo.sampler = vkSampler;
		itImage->second.vkDescriptorImageInfo.imageView = vkImageView;
		itImage->second.vkDescriptorImageInfo.imageLayout = vkImageLayout;

		VkWriteDescriptorSet write;
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = NULL;
		write.dstSet = itImage->second.vkDescriptorSet;
		write.dstBinding = itImage->second.binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo = &itImage->second.vkDescriptorImageInfo;
		write.pBufferInfo = NULL;
		write.pTexelBufferView = NULL;
		vkUpdateDescriptorSets(m_pDevice->GetDevice(), 1, &write, 0, NULL);

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::SetUniformBuffer(const char *szName, VkBuffer vkBuffer, VkDeviceSize offset, VkDeviceSize range)
	{
		const auto &itBuffer = m_buffers.find(szName);
		if (itBuffer == m_buffers.end()) return FALSE;

		itBuffer->second.vkDescriptorBufferInfo.buffer = vkBuffer;
		itBuffer->second.vkDescriptorBufferInfo.offset = offset;
		itBuffer->second.vkDescriptorBufferInfo.range = range;

		VkWriteDescriptorSet write;
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = NULL;
		write.dstSet = itBuffer->second.vkDescriptorSet;
		write.dstBinding = itBuffer->second.binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write.pImageInfo = NULL;
		write.pBufferInfo = &itBuffer->second.vkDescriptorBufferInfo;
		write.pTexelBufferView = NULL;
		vkUpdateDescriptorSets(m_pDevice->GetDevice(), 1, &write, 0, NULL);

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::Create(VkRenderPass vkRenderPass)
	{
		try {
			std::vector<VkDescriptorSetLayout> layouts;
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

			CALL_BOOL_FUNCTION_THROW(CreateDescriptor(layouts));
			CALL_BOOL_FUNCTION_THROW(CreateShaderStages(shaderStages));
			CALL_BOOL_FUNCTION_THROW(CreateVertexInputState(inputBindingDescriptions, inputAttributeDescriptions));
			CALL_BOOL_FUNCTION_THROW(CreateColorBlendState(colorBlendAttachments));

			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.pNext = NULL;
			pipelineLayoutCreateInfo.flags = 0;
			pipelineLayoutCreateInfo.setLayoutCount = layouts.size();
			pipelineLayoutCreateInfo.pSetLayouts = layouts.data();
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
			CALL_VK_FUNCTION_THROW(vkCreatePipelineLayout(m_pDevice->GetDevice(), &pipelineLayoutCreateInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

			VkGraphicsPipelineCreateInfo pipelineCreateInfo;
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = NULL;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.stageCount = shaderStages.size();
			pipelineCreateInfo.pStages = shaderStages.data();
			pipelineCreateInfo.pVertexInputState = &m_vertexInputState;
			pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
			pipelineCreateInfo.pTessellationState = m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].module != VK_NULL_HANDLE && m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].module != VK_NULL_HANDLE ? &m_tessellationState : NULL;
			pipelineCreateInfo.pViewportState = &m_viewportState;
			pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
			pipelineCreateInfo.pMultisampleState = &m_multiSampleState;
			pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
			pipelineCreateInfo.pColorBlendState = &m_colorBlendState;
			pipelineCreateInfo.pDynamicState = &m_dynamicState;
			pipelineCreateInfo.layout = m_vkPipelineLayout;
			pipelineCreateInfo.renderPass = vkRenderPass;
			pipelineCreateInfo.subpass = VK_NULL_HANDLE;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineCreateInfo.basePipelineIndex = 0;
			CALL_VK_FUNCTION_THROW(vkCreateGraphicsPipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererPipelineGraphics::Destroy(void)
	{
		for (const auto &itDescriptorSet : m_pDescriptorSets) {
			if (CRendererDescriptorSet *pDescriptorSet = itDescriptorSet) {
				m_pDevice->GetDescriptorSetManager()->FreeDescriptorSet(0, pDescriptorSet);
			}
		}

		for (const auto &itDescriptorSetLayout : m_pDescriptorSetLayouts) {
			if (CRendererDescriptorSetLayout *pDescriptorSetLayout = itDescriptorSetLayout) {
				m_pDevice->GetDescriptorSetLayoutManager()->Free(pDescriptorSetLayout);
			}
		}

		if (m_vkPipelineLayout) {
			vkDestroyPipelineLayout(m_pDevice->GetDevice(), m_vkPipelineLayout, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkPipelineLayout = VK_NULL_HANDLE;

		m_images.clear();
		m_buffers.clear();
		m_pDescriptorSets.clear();
		m_pDescriptorSetLayouts.clear();

		CRendererPipeline::Destroy();
	}

	BOOL CRendererPipelineGraphics::CreateDescriptor(std::vector<VkDescriptorSetLayout> &layouts)
	{
		layouts.clear();
		m_images.clear();
		m_buffers.clear();
		m_pDescriptorSets.clear();
		m_pDescriptorSetLayouts.clear();

		std::map<uint32_t, std::map<uint32_t, std::string>> names;
		std::map<uint32_t, std::map<uint32_t, VkDescriptorType>> types;
		std::map<uint32_t, uint32_t[VK_DESCRIPTOR_TYPE_RANGE_SIZE]> counts;
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
					if (pDescriptorSetLayouts[set] == NULL) return FALSE;
				}

				if (variable.second.storage_class == SpvStorageClassUniform) {
					counts[set][VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER]++;
					types[set][binding] = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					names[set][binding] = itMoudle.second.struct_types.at(variable.second.type_id).name;
					pDescriptorSetLayouts[set]->SetBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStageFlags);
				}

				if (variable.second.storage_class == SpvStorageClassUniformConstant) {
					counts[set][VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER]++;
					types[set][binding] = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					names[set][binding] = variable.second.name;
					pDescriptorSetLayouts[set]->SetBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, shaderStageFlags);
				}
			}
		}

		for (const auto &itDescriptorSetLayout : pDescriptorSetLayouts) {
			if (CRendererDescriptorSetLayout *pDescriptorSetLayout = itDescriptorSetLayout.second) {
				CRendererDescriptorSet *pDescriptorSet = NULL;

				pDescriptorSetLayout->Create();
				pDescriptorSet = m_pDevice->GetDescriptorSetManager()->AllocDescriptorSet(0, pDescriptorSetLayout->GetSetLayout(), counts[itDescriptorSetLayout.first]);

				for (const auto &itName : names[itDescriptorSetLayout.first]) {
					uint32_t set = itDescriptorSetLayout.first;
					uint32_t binding = itName.first;
					const std::string &name = itName.second;

					if (types[set][binding] == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
						m_buffers[name].binding = binding;
						m_buffers[name].vkDescriptorSet = pDescriptorSet->GetDescriptorSet();
					}

					if (types[set][binding] == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
						m_images[name].binding = binding;
						m_images[name].vkDescriptorSet = pDescriptorSet->GetDescriptorSet();
					}
				}

				m_pDescriptorSets.push_back(pDescriptorSet);
				m_pDescriptorSetLayouts.push_back(pDescriptorSetLayout);

				layouts.push_back(pDescriptorSetLayout->GetSetLayout());
			}
		}

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages)
	{
		shaderStages.clear();

		for (const auto &itShaderStage : m_shaderStages) {
			if (itShaderStage.second.module != VK_NULL_HANDLE) {
				shaderStages.push_back(itShaderStage.second);
			}
		}

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::CreateVertexInputState(std::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, std::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions)
	{
		m_vertexFormat = 0;

		inputBindingDescriptions.clear();
		inputAttributeDescriptions.clear();

		for (const auto &variable : m_shaderModules[VK_SHADER_STAGE_VERTEX_BIT].variables) {
			if (variable.second.storage_class == SpvStorageClassInput) {
				if (uint32_t attribute = m_pDevice->GetVertexAttributeFlag(variable.second.name.c_str())) {
					m_vertexFormat |= attribute;
				}
			}
		}

		for (const auto &variable : m_shaderModules[VK_SHADER_STAGE_VERTEX_BIT].variables) {
			if (variable.second.storage_class == SpvStorageClassInput) {
				if (uint32_t attribute = m_pDevice->GetVertexAttributeFlag(variable.second.name.c_str())) {
					VkVertexInputAttributeDescription inputAttributeDescription;
					inputAttributeDescription.binding = 0;
					inputAttributeDescription.location = variable.second.location;
					inputAttributeDescription.format = m_pDevice->GetVertexAttributeFormat(attribute);
					inputAttributeDescription.offset = m_pDevice->GetVertexAttributeOffset(m_vertexFormat, attribute);
					inputAttributeDescriptions.push_back(inputAttributeDescription);
				}
			}
		}

		VkVertexInputBindingDescription inputBindingDescription;
		inputBindingDescription.binding = 0;
		inputBindingDescription.stride = m_pDevice->GetVertexSize(m_vertexFormat);
		inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		inputBindingDescriptions.push_back(inputBindingDescription);

		m_vertexInputState.vertexBindingDescriptionCount = inputBindingDescriptions.size();
		m_vertexInputState.pVertexBindingDescriptions = inputBindingDescriptions.data();
		m_vertexInputState.vertexAttributeDescriptionCount = inputAttributeDescriptions.size();
		m_vertexInputState.pVertexAttributeDescriptions = inputAttributeDescriptions.data();

		return TRUE;
	}

	BOOL CRendererPipelineGraphics::CreateColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> &colorBlendAttachments)
	{
		colorBlendAttachments.clear();

		for (const auto &itColorBlendAttachment : m_colorBlendAttachmentStates) {
			colorBlendAttachments.push_back(itColorBlendAttachment.second);
		}

		m_colorBlendState.attachmentCount = colorBlendAttachments.size();
		m_colorBlendState.pAttachments = colorBlendAttachments.data();

		return TRUE;
	}

}
