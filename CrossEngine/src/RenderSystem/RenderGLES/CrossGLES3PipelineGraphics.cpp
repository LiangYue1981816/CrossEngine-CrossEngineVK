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

	CGLES3PipelineGraphics::CGLES3PipelineGraphics(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGLES3Pipeline(pDevice)
		, CGfxPipelineGraphics(pResourceManager)
		, m_vertexFormat(0)
	{
		m_inputAssemblyState = {};
		m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_inputAssemblyState.pNext = NULL;
		m_inputAssemblyState.flags = 0;

		m_tessellationState = {};
		m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_tessellationState.pNext = NULL;
		m_tessellationState.flags = 0;

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

		SetDefault();
	}

	CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
	{

	}

	CGLES3Device* CGLES3PipelineGraphics::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3PipelineGraphics::GetHandle(void) const
	{
		return (HANDLE)m_pipeline;
	}

	BOOL CGLES3PipelineGraphics::Create(HANDLE hRenderPass, uint32_t indexSubPass)
	{
		if (m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT].IsNull() ||
			m_ptrShaders[VK_SHADER_STAGE_FRAGMENT_BIT].IsNull()) {
			return FALSE;
		}

		glGenProgramPipelines(1, &m_pipeline);
		glBindProgramPipeline(m_pipeline);
		{
			glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, (GLuint)m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT]->GetHandle());
			glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, (GLuint)m_ptrShaders[VK_SHADER_STAGE_FRAGMENT_BIT]->GetHandle());
		}
		glBindProgramPipeline(0);

		CreateDescriptorSetLayouts();
		CreateVertexInputAttributeDescriptions();

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::CreateVertexInputAttributeDescriptions(void)
	{
		m_vertexFormat = 0;

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
				m_vertexInputAttributeDescriptions[attribute] = inputAttributeDescription;
			}
		}

		return TRUE;
	}

	void CGLES3PipelineGraphics::Destroy(void)
	{
		CGLES3Pipeline::Destroy();
		SetDefault();
	}

	void CGLES3PipelineGraphics::SetDefault(void)
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

	BOOL CGLES3PipelineGraphics::SetVertexShader(const CGfxShaderPtr &ptrShader)
	{
		m_ptrShaders[VK_SHADER_STAGE_VERTEX_BIT] = ptrShader;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetTessellationControlShader(const CGfxShaderPtr &ptrShader)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetTessellationEvaluationShader(const CGfxShaderPtr &ptrShader)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetGeometryShader(const CGfxShaderPtr &ptrShader)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetFragmentShader(const CGfxShaderPtr &ptrShader)
	{
		m_ptrShaders[VK_SHADER_STAGE_FRAGMENT_BIT] = ptrShader;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetPrimitiveTopology(VkPrimitiveTopology topology, BOOL primitiveRestartEnable)
	{
		m_inputAssemblyState.topology = topology;
		m_inputAssemblyState.primitiveRestartEnable = primitiveRestartEnable;

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetTessellationPatchControlPoints(uint32_t patchControlPoints)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetPolygonMode(VkPolygonMode polygonMode)
	{
		m_rasterizationState.polygonMode = polygonMode;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetCullMode(VkCullModeFlags cullMode)
	{
		m_rasterizationState.cullMode = cullMode;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetFrontFace(VkFrontFace frontFace)
	{
		m_rasterizationState.frontFace = frontFace;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthClamp(BOOL depthClampEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthBias(BOOL depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		m_rasterizationState.depthBiasEnable = depthBiasEnable;
		m_rasterizationState.depthBiasConstantFactor = depthBiasConstantFactor;
		m_rasterizationState.depthBiasClamp = depthBiasClamp;
		m_rasterizationState.depthBiasSlopeFactor = depthBiasSlopeFactor;

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetRasterizerDiscard(BOOL rasterizerDiscardEnable)
	{
		m_rasterizationState.rasterizerDiscardEnable = rasterizerDiscardEnable;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleCounts(VkSampleCountFlagBits rasterizationSamples)
	{
		m_multiSampleState.rasterizationSamples = rasterizationSamples;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleShading(BOOL sampleShadingEnable, float minSampleShading)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleMask(const VkSampleMask* pMask)
	{
		m_multiSampleState.pSampleMask = pMask;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleAlphaToCoverage(BOOL alphaToCoverageEnable)
	{
		m_multiSampleState.alphaToCoverageEnable = alphaToCoverageEnable;
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleAlphaToOne(BOOL alphaToOneEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthTest(BOOL depthTestEnable, BOOL depthWriteEnable, VkCompareOp depthCompareOp)
	{
		m_depthStencilState.depthTestEnable = depthTestEnable;
		m_depthStencilState.depthWriteEnable = depthWriteEnable;
		m_depthStencilState.depthCompareOp = depthCompareOp;

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthBoundsTest(BOOL depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetStencilTest(BOOL stencilTestEnable, VkStencilOp frontFailOp, VkStencilOp frontPassOp, VkStencilOp frontDepthFailOp, VkCompareOp frontCompareOp, uint32_t frontCompareMask, uint32_t frontWriteMask, uint32_t frontReference, VkStencilOp backFailOp, VkStencilOp backPassOp, VkStencilOp backDepthFailOp, VkCompareOp backCompareOp, uint32_t backCompareMask, uint32_t backWriteMask, uint32_t backReference)
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

	BOOL CGLES3PipelineGraphics::SetColorBlendLogic(BOOL logicOpEnable, VkLogicOp logicOp)
	{
		m_colorBlendState.logicOpEnable = logicOpEnable;
		m_colorBlendState.logicOp = logicOp;

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetColorBlendConstants(float r, float g, float b, float a)
	{
		m_colorBlendState.blendConstants[0] = r;
		m_colorBlendState.blendConstants[1] = g;
		m_colorBlendState.blendConstants[2] = b;
		m_colorBlendState.blendConstants[3] = a;

		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetColorBlendAttachment(uint32_t attachment, BOOL blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask)
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

	const VkPipelineInputAssemblyStateCreateInfo& CGLES3PipelineGraphics::GetInputAssemblyState(void) const
	{
		return m_inputAssemblyState;
	}

	const VkPipelineTessellationStateCreateInfo& CGLES3PipelineGraphics::GetTessellationState(void) const
	{
		return m_tessellationState;
	}

	const VkPipelineRasterizationStateCreateInfo& CGLES3PipelineGraphics::GetRasterizationState(void) const
	{
		return m_rasterizationState;
	}

	const VkPipelineMultisampleStateCreateInfo& CGLES3PipelineGraphics::GetMultisampleState(void) const
	{
		return m_multiSampleState;
	}

	const VkPipelineDepthStencilStateCreateInfo& CGLES3PipelineGraphics::GetDepthStencilState(void) const
	{
		return m_depthStencilState;
	}

	const VkPipelineColorBlendStateCreateInfo& CGLES3PipelineGraphics::GetColorBlendState(void) const
	{
		return m_colorBlendState;
	}

	uint32_t CGLES3PipelineGraphics::GetVertexFormat(void) const
	{
		return m_vertexFormat;
	}

	uint32_t CGLES3PipelineGraphics::GetBinding(uint32_t set, const char *szName) const
	{
		return GetBinding(set, HashValue(szName));
	}

	uint32_t CGLES3PipelineGraphics::GetBinding(uint32_t set, uint32_t dwName) const
	{
		if (const CGLES3DescriptorSetLayout *pDescriptorSetLayout = GetDescriptorSetLayout(set)) {
			return pDescriptorSetLayout->GetBinding(dwName);
		}

		return -1;
	}

	void CGLES3PipelineGraphics::DumpLog(void) const
	{
		if (m_pipeline) {
			LOGI("\t\tPipelineGraphics 0x%x\n", m_pipeline);
		}
	}

}
