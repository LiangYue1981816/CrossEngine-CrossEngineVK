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
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_DEPTH_BOUNDS, VK_DYNAMIC_STATE_STENCIL_REFERENCE };
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
		SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_FLAG_BITS_MAX_ENUM);

	}

	CRendererPipelineGraphics::~CRendererPipelineGraphics(void)
	{

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

	BOOL CRendererPipelineGraphics::Create(VkPipelineLayout vkLayout, VkRenderPass vkRenderPass)
	{
		try {
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

			CreateShaderStages(shaderStages);
			CreateVertexInputState(inputBindingDescriptions, inputAttributeDescriptions);
			CreateColorBlendState(colorBlendAttachments);

			VkGraphicsPipelineCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.stageCount = shaderStages.size();
			createInfo.pStages = shaderStages.data();
			createInfo.pVertexInputState = &m_vertexInputState;
			createInfo.pInputAssemblyState = &m_inputAssemblyState;
			createInfo.pTessellationState = m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].module != VK_NULL_HANDLE && m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].module != VK_NULL_HANDLE ? &m_tessellationState : NULL;
			createInfo.pViewportState = &m_viewportState;
			createInfo.pRasterizationState = &m_rasterizationState;
			createInfo.pMultisampleState = &m_multiSampleState;
			createInfo.pDepthStencilState = &m_depthStencilState;
			createInfo.pColorBlendState = &m_colorBlendState;
			createInfo.pDynamicState = &m_dynamicState;
			createInfo.layout = vkLayout;
			createInfo.renderPass = vkRenderPass;
			createInfo.subpass = VK_NULL_HANDLE;
			createInfo.basePipelineHandle = VK_NULL_HANDLE;
			createInfo.basePipelineIndex = 0;
			CALL_VK_FUNCTION_THROW(vkCreateGraphicsPipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	BOOL CRendererPipelineGraphics::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages)
	{
		shaderStages.clear();

		for (std::map<VkShaderStageFlagBits, VkPipelineShaderStageCreateInfo>::const_iterator itShaderStage = m_shaderStages.begin(); itShaderStage != m_shaderStages.end(); ++itShaderStage) {
			if (itShaderStage->second.module != VK_NULL_HANDLE) {
				shaderStages.push_back(itShaderStage->second);
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
			uint32_t attribute = m_pDevice->GetVertexAttributeFlag(variable.second.name.c_str());
			m_vertexFormat |= attribute;
		}

		for (const auto &variable : m_shaderModules[VK_SHADER_STAGE_VERTEX_BIT].variables) {
			uint32_t attribute = m_pDevice->GetVertexAttributeFlag(variable.second.name.c_str());
			VkVertexInputAttributeDescription inputAttributeDescription;
			inputAttributeDescription.binding = 0;
			inputAttributeDescription.location = variable.second.location;
			inputAttributeDescription.format = (VkFormat)variable.second.type_id;
			inputAttributeDescription.offset = m_pDevice->GetVertexAttributeOffset(m_vertexFormat, attribute);
			inputAttributeDescriptions.push_back(inputAttributeDescription);
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

		for (std::map<uint32_t, VkPipelineColorBlendAttachmentState>::const_iterator itColorBlendAttachment = m_colorBlendAttachmentStates.begin(); itColorBlendAttachment != m_colorBlendAttachmentStates.end(); ++itColorBlendAttachment) {
			colorBlendAttachments.push_back(itColorBlendAttachment->second);
		}

		m_colorBlendState.attachmentCount = colorBlendAttachments.size();
		m_colorBlendState.pAttachments = colorBlendAttachments.data();

		return TRUE;
	}

}
