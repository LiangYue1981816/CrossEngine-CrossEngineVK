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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CVulkanPipelineGraphics : public CVulkanPipeline
	{
		friend class CVulkanPipelineManager;


	protected:
		CVulkanPipelineGraphics(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager);
		virtual ~CVulkanPipelineGraphics(void);


	public:
		virtual BOOL Create(VkRenderPass vkRenderPass);

	protected:
		BOOL CreateVertexInputState(std::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, std::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions);
		BOOL CreateColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> &colorBlendAttachments);

	public:
		BOOL SetVertexShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName = "main");
		BOOL SetTessellationControlShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName = "main");
		BOOL SetTessellationEvaluationShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName = "main");
		BOOL SetGeometryShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName = "main");
		BOOL SetFragmentShader(VkShaderModule vkShader, const spirv::module_type &module, const char *szName = "main");
		BOOL SetPrimitiveTopology(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable = VK_FALSE);
		BOOL SetTessellationPatchControlPoints(uint32_t patchControlPoints);
		BOOL SetPolygonMode(VkPolygonMode polygonMode);
		BOOL SetCullMode(VkCullModeFlags cullMode);
		BOOL SetFrontFace(VkFrontFace frontFace);
		BOOL SetDepthClamp(VkBool32 depthClampEnable);
		BOOL SetDepthBias(VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
		BOOL SetRasterizerDiscard(VkBool32 rasterizerDiscardEnable);
		BOOL SetSampleCounts(VkSampleCountFlagBits rasterizationSamples);
		BOOL SetSampleShading(VkBool32 sampleShadingEnable, float minSampleShading);
		BOOL SetSampleMask(const VkSampleMask* pSampleMask);
		BOOL SetSampleAlphaToCoverage(VkBool32 alphaToCoverageEnable);
		BOOL SetSampleAlphaToOne(VkBool32 alphaToOneEnable);
		BOOL SetDepthTest(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp);
		BOOL SetDepthBoundsTest(VkBool32 depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds);
		BOOL SetStencilTest(VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back);
		BOOL SetColorBlendLogic(VkBool32 logicOpEnable, VkLogicOp logicOp);
		BOOL SetColorBlendConstants(float r, float g, float b, float a);
		BOOL SetColorBlendAttachment(uint32_t attachment, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask);

	public:
		uint32_t GetVertexFormat(void) const;


	protected:
		uint32_t m_vertexFormat;

	protected:
		VkPipelineVertexInputStateCreateInfo m_vertexInputState;
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		VkPipelineTessellationStateCreateInfo m_tessellationState;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		VkPipelineMultisampleStateCreateInfo m_multiSampleState;
		VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
		VkPipelineColorBlendStateCreateInfo m_colorBlendState;
		VkPipelineDynamicStateCreateInfo m_dynamicState;

	protected:
		std::map<uint32_t, VkPipelineColorBlendAttachmentState> m_colorBlendAttachmentStates;
	};

	class CROSS_EXPORT CVulkanPipelineGraphicsPtr : public CVulkanResourcePtr<CVulkanPipelineGraphics>
	{
	public:
		CVulkanPipelineGraphicsPtr(void) : CVulkanResourcePtr<CVulkanPipelineGraphics>() {}
		CVulkanPipelineGraphicsPtr(const CVulkanPipelineGraphics *p) : CVulkanResourcePtr<CVulkanPipelineGraphics>(p) {}
		CVulkanPipelineGraphicsPtr(const CVulkanPipelineGraphicsPtr &ptr) : CVulkanResourcePtr<CVulkanPipelineGraphics>(ptr) {}
	};

}
