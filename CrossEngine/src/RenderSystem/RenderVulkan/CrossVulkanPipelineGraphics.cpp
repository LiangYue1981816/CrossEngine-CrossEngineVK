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

	CVulkanPipelineGraphics::CVulkanPipelineGraphics(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanPipeline(pDevice)
		, CGfxPipelineGraphics(pResourceManager)
		, m_vertexFormat(0)
	{
		m_vertexInputState = {};
		m_vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		m_vertexInputState.pNext = NULL;
		m_vertexInputState.flags = 0;

		m_inputAssemblyState = {};
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.pNext = NULL;
		m_inputAssemblyState.flags = 0;

		m_tessellationState = {};
		m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_tessellationState.pNext = NULL;
		m_tessellationState.flags = 0;

		m_viewportState = {};
		m_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_viewportState.pNext = NULL;
		m_viewportState.flags = 0;
		m_viewportState.viewportCount = 1;
		m_viewportState.scissorCount = 1;

		m_rasterizationState = {};
		m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_rasterizationState.pNext = NULL;
		m_rasterizationState.flags = 0;
		m_rasterizationState.lineWidth = 1.0f;

		m_multiSampleState = {};
		m_multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_multiSampleState.pNext = NULL;
		m_multiSampleState.flags = 0;

		m_depthStencilState = {};
		m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_depthStencilState.pNext = NULL;
		m_depthStencilState.flags = 0;

		m_colorBlendState = {};
		m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_colorBlendState.pNext = NULL;
		m_colorBlendState.flags = 0;

		static VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_DEPTH_BIAS,
			VK_DYNAMIC_STATE_DEPTH_BOUNDS,
			VK_DYNAMIC_STATE_STENCIL_REFERENCE
		};
		m_dynamicState = {};
		m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_dynamicState.pNext = NULL;
		m_dynamicState.flags = 0;
		m_dynamicState.dynamicStateCount = 5;
		m_dynamicState.pDynamicStates = dynamicStates;

		SetDefault();
	}

	CVulkanPipelineGraphics::~CVulkanPipelineGraphics(void)
	{

	}

	CVulkanDevice* CVulkanPipelineGraphics::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanPipelineGraphics::GetHandle(void) const
	{
		return m_vkPipeline;
	}

	BOOL CVulkanPipelineGraphics::Create(HANDLE hRenderPass, uint32_t indexSubPass)
	{
		try {
			std::vector<VkDescriptorSetLayout> layouts;
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

			CALL_BOOL_FUNCTION_THROW(CreateDescriptorSetLayouts(layouts));
			CALL_BOOL_FUNCTION_THROW(CreateShaderStages(shaderStages));
			CALL_BOOL_FUNCTION_THROW(CreateVertexInputState(inputBindingDescriptions, inputAttributeDescriptions));
			CALL_BOOL_FUNCTION_THROW(CreateColorBlendState(colorBlendAttachments));

			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.pNext = NULL;
			pipelineLayoutCreateInfo.flags = 0;
			pipelineLayoutCreateInfo.setLayoutCount = layouts.size();
			pipelineLayoutCreateInfo.pSetLayouts = layouts.data();
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
			CALL_VK_FUNCTION_THROW(vkCreatePipelineLayout(m_pDevice->GetDevice(), &pipelineLayoutCreateInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = NULL;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.stageCount = shaderStages.size();
			pipelineCreateInfo.pStages = shaderStages.data();
			pipelineCreateInfo.pVertexInputState = &m_vertexInputState;
			pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
			pipelineCreateInfo.pTessellationState = &m_tessellationState;
			pipelineCreateInfo.pViewportState = &m_viewportState;
			pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
			pipelineCreateInfo.pMultisampleState = &m_multiSampleState;
			pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
			pipelineCreateInfo.pColorBlendState = &m_colorBlendState;
			pipelineCreateInfo.pDynamicState = &m_dynamicState;
			pipelineCreateInfo.layout = m_vkPipelineLayout;
			pipelineCreateInfo.renderPass = (VkRenderPass)hRenderPass;
			pipelineCreateInfo.subpass = indexSubPass;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineCreateInfo.basePipelineIndex = 0;
			CALL_VK_FUNCTION_THROW(vkCreateGraphicsPipelines(m_pDevice->GetDevice(), ((CVulkanPipelineManager *)m_pResourceManager)->GetPipelineCache(), 1, &pipelineCreateInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanPipelineGraphics::CreateVertexInputState(std::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, std::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions)
	{
		m_vertexFormat = 0;

		inputBindingDescriptions.clear();
		inputAttributeDescriptions.clear();

		if (m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT].IsNull()) {
			return FALSE;
		}

		const spirv_cross::Compiler *pShaderCompiler = m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT]->GetShaderCompiler();
		const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

		for (const auto &itInput : shaderResources.stage_inputs) {
			if (uint32_t attribute = m_pDevice->GetVertexAttributeFlag(itInput.name.c_str())) {
				m_vertexFormat |= attribute;
			}
		}

		for (const auto &itInput : shaderResources.stage_inputs) {
			if (uint32_t attribute = m_pDevice->GetVertexAttributeFlag(itInput.name.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription;
				inputAttributeDescription.binding = 0;
				inputAttributeDescription.location = pShaderCompiler->get_decoration(itInput.id, spv::DecorationLocation);
				inputAttributeDescription.format = m_pDevice->GetVertexAttributeFormat(attribute);
				inputAttributeDescription.offset = m_pDevice->GetVertexAttributeOffset(m_vertexFormat, attribute);
				inputAttributeDescriptions.push_back(inputAttributeDescription);
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

	BOOL CVulkanPipelineGraphics::CreateColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> &colorBlendAttachments)
	{
		colorBlendAttachments.clear();

		for (const auto &itColorBlendAttachment : m_colorBlendAttachmentStates) {
			colorBlendAttachments.push_back(itColorBlendAttachment.second);
		}

		m_colorBlendState.attachmentCount = colorBlendAttachments.size();
		m_colorBlendState.pAttachments = colorBlendAttachments.data();

		return TRUE;
	}

	void CVulkanPipelineGraphics::Destroy(void)
	{
		CVulkanPipeline::Destroy();
		SetDefault();
	}

	void CVulkanPipelineGraphics::SetDefault(void)
	{
		VkStencilOpState front = {};
		VkStencilOpState back = {};
		back.failOp = front.failOp = VK_STENCIL_OP_KEEP;
		back.passOp = front.passOp = VK_STENCIL_OP_KEEP;
		back.compareOp = front.compareOp = VK_COMPARE_OP_ALWAYS;

		m_vertexFormat = 0;
		m_colorBlendAttachmentStates.clear();

		SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, FALSE);
		SetTessellationPatchControlPoints(0);
		SetPolygonMode(VK_POLYGON_MODE_FILL);
		SetCullMode(VK_CULL_MODE_BACK_BIT);
		SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
		SetDepthClamp(FALSE);
		SetDepthBias(FALSE, 0.0f, 0.0f, 0.0f);
		SetRasterizerDiscard(FALSE);
		SetSampleCounts(VK_SAMPLE_COUNT_1_BIT);
		SetSampleShading(FALSE, 0.0f);
		SetSampleMask(NULL);
		SetSampleAlphaToCoverage(FALSE);
		SetSampleAlphaToOne(FALSE);
		SetDepthTest(TRUE, TRUE, VK_COMPARE_OP_LESS);
		SetDepthBoundsTest(FALSE, 0.0f, 1.0f);
		SetStencilTest(FALSE, front.failOp, front.passOp, front.depthFailOp, front.compareOp, front.compareMask, front.writeMask, front.reference, back.failOp, back.passOp, back.depthFailOp, back.compareOp, back.compareMask, back.writeMask, back.reference);
		SetColorBlendLogic(FALSE, VK_LOGIC_OP_CLEAR);
		SetColorBlendConstants(0.0f, 0.0f, 0.0f, 0.0f);
	}

	BOOL CVulkanPipelineGraphics::SetVertexShader(const CGfxShaderPtr &ptrShader)
	{
		m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT] = ptrShader;
		m_shaderStages[VK_SHADER_STAGE_VERTEX_BIT].module = (VkShaderModule)ptrShader->GetHandle();

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetTessellationControlShader(const CGfxShaderPtr &ptrShader)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().tessellationShader) {
			m_ptrShaders[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT] = ptrShader;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT].module = (VkShaderModule)ptrShader->GetHandle();
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetTessellationEvaluationShader(const CGfxShaderPtr &ptrShader)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().tessellationShader) {
			m_ptrShaders[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT] = ptrShader;
			m_shaderStages[VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT].module = (VkShaderModule)ptrShader->GetHandle();
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetGeometryShader(const CGfxShaderPtr &ptrShader)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().geometryShader) {
			m_ptrShaders[VK_SHADER_STAGE_GEOMETRY_BIT] = ptrShader;
			m_shaderStages[VK_SHADER_STAGE_GEOMETRY_BIT].module = (VkShaderModule)ptrShader->GetHandle();
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetFragmentShader(const CGfxShaderPtr &ptrShader)
	{
		m_ptrShaders[VK_SHADER_STAGE_FRAGMENT_BIT] = ptrShader;
		m_shaderStages[VK_SHADER_STAGE_FRAGMENT_BIT].module = (VkShaderModule)ptrShader->GetHandle();

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetPrimitiveTopology(VkPrimitiveTopology topology, BOOL primitiveRestartEnable)
	{
		m_inputAssemblyState.topology = topology;
		m_inputAssemblyState.primitiveRestartEnable = primitiveRestartEnable;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetTessellationPatchControlPoints(uint32_t patchControlPoints)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().tessellationShader) {
			if (patchControlPoints > m_pDevice->GetPhysicalDeviceLimits().maxTessellationPatchSize) {
				patchControlPoints = m_pDevice->GetPhysicalDeviceLimits().maxTessellationPatchSize;
			}

			m_tessellationState.patchControlPoints = patchControlPoints;
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetPolygonMode(VkPolygonMode polygonMode)
	{
		m_rasterizationState.polygonMode = polygonMode;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetCullMode(VkCullModeFlags cullMode)
	{
		m_rasterizationState.cullMode = cullMode;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetFrontFace(VkFrontFace frontFace)
	{
		m_rasterizationState.frontFace = frontFace;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetDepthClamp(BOOL depthClampEnable)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().depthClamp) {
			m_rasterizationState.depthClampEnable = depthClampEnable;
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetDepthBias(BOOL depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		m_rasterizationState.depthBiasEnable = depthBiasEnable;
		m_rasterizationState.depthBiasConstantFactor = depthBiasConstantFactor;
		m_rasterizationState.depthBiasClamp = depthBiasClamp;
		m_rasterizationState.depthBiasSlopeFactor = depthBiasSlopeFactor;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetRasterizerDiscard(BOOL rasterizerDiscardEnable)
	{
		m_rasterizationState.rasterizerDiscardEnable = rasterizerDiscardEnable;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetSampleCounts(VkSampleCountFlagBits rasterizationSamples)
	{
		m_multiSampleState.rasterizationSamples = rasterizationSamples;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetSampleShading(BOOL sampleShadingEnable, float minSampleShading)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().sampleRateShading) {
			m_multiSampleState.sampleShadingEnable = sampleShadingEnable;
			m_multiSampleState.minSampleShading = minSampleShading;
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetSampleMask(const VkSampleMask* pMask)
	{
		m_multiSampleState.pSampleMask = pMask;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetSampleAlphaToCoverage(BOOL alphaToCoverageEnable)
	{
		m_multiSampleState.alphaToCoverageEnable = alphaToCoverageEnable;
		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetSampleAlphaToOne(BOOL alphaToOneEnable)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().alphaToOne) {
			m_multiSampleState.alphaToOneEnable = alphaToOneEnable;
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetDepthTest(BOOL depthTestEnable, BOOL depthWriteEnable, VkCompareOp depthCompareOp)
	{
		m_depthStencilState.depthTestEnable = depthTestEnable;
		m_depthStencilState.depthWriteEnable = depthWriteEnable;
		m_depthStencilState.depthCompareOp = depthCompareOp;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetDepthBoundsTest(BOOL depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds)
	{
		if (m_pDevice->GetPhysicalDeviceFeatures().depthBounds) {
			m_depthStencilState.depthBoundsTestEnable = depthBoundsTestEnable;
			m_depthStencilState.minDepthBounds = minDepthBounds;
			m_depthStencilState.maxDepthBounds = maxDepthBounds;
		}

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetStencilTest(BOOL stencilTestEnable, VkStencilOp frontFailOp, VkStencilOp frontPassOp, VkStencilOp frontDepthFailOp, VkCompareOp frontCompareOp, uint32_t frontCompareMask, uint32_t frontWriteMask, uint32_t frontReference, VkStencilOp backFailOp, VkStencilOp backPassOp, VkStencilOp backDepthFailOp, VkCompareOp backCompareOp, uint32_t backCompareMask, uint32_t backWriteMask, uint32_t backReference)
	{
		m_depthStencilState.stencilTestEnable = stencilTestEnable;
		m_depthStencilState.front.failOp = frontFailOp;
		m_depthStencilState.front.passOp = frontPassOp;
		m_depthStencilState.front.depthFailOp = frontDepthFailOp;
		m_depthStencilState.front.compareOp = frontCompareOp;
		m_depthStencilState.front.compareMask = frontCompareMask;
		m_depthStencilState.front.writeMask = frontWriteMask;
		m_depthStencilState.front.reference = frontReference;
		m_depthStencilState.back.failOp = backFailOp;
		m_depthStencilState.back.passOp = backPassOp;
		m_depthStencilState.back.depthFailOp = backDepthFailOp;
		m_depthStencilState.back.compareOp = backCompareOp;
		m_depthStencilState.back.compareMask = backCompareMask;
		m_depthStencilState.back.writeMask = backWriteMask;
		m_depthStencilState.back.reference = backReference;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetColorBlendLogic(BOOL logicOpEnable, VkLogicOp logicOp)
	{
		m_colorBlendState.logicOpEnable = logicOpEnable;
		m_colorBlendState.logicOp = logicOp;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetColorBlendConstants(float r, float g, float b, float a)
	{
		m_colorBlendState.blendConstants[0] = r;
		m_colorBlendState.blendConstants[1] = g;
		m_colorBlendState.blendConstants[2] = b;
		m_colorBlendState.blendConstants[3] = a;

		return TRUE;
	}

	BOOL CVulkanPipelineGraphics::SetColorBlendAttachment(uint32_t attachment, BOOL blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask)
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

	uint32_t CVulkanPipelineGraphics::GetVertexFormat(void) const
	{
		return m_vertexFormat;
	}

	uint32_t CVulkanPipelineGraphics::GetBinding(uint32_t set, const char *szName) const
	{
		return GetBinding(set, HashValue(szName));
	}

	uint32_t CVulkanPipelineGraphics::GetBinding(uint32_t set, uint32_t dwName) const
	{
		if (const CVulkanDescriptorSetLayout *pDescriptorSetLayout = GetDescriptorSetLayout(set)) {
			return pDescriptorSetLayout->GetBinding(dwName);
		}

		return -1;
	}

	void CVulkanPipelineGraphics::DumpLog(void) const
	{
		if (m_vkPipeline) {
			LOGI("\t\tPipelineGraphics 0x%x\n", m_vkPipeline);
		}
	}

}
