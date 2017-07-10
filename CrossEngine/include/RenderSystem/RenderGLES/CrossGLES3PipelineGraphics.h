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

	class CROSS_EXPORT CGLES3PipelineGraphics : public CGfxPipelineGraphics, public CGLES3Pipeline
	{
		friend class CGLES3PipelineManager;


	protected:
		CGLES3PipelineGraphics(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager);
		virtual ~CGLES3PipelineGraphics(void);


	public:
		HANDLE GetHandle(void) const;

	public:
		BOOL Create(HANDLE hRenderPass);
		void Destroy(void);
		void DumpLog(void) const;

	protected:
		BOOL CreateVertexInputAttributeDescriptions(void);

	public:
		void SetDefault(void);
		BOOL SetVertexShader(const CGfxShaderPtr &ptrShader);
		BOOL SetTessellationControlShader(const CGfxShaderPtr &ptrShader);
		BOOL SetTessellationEvaluationShader(const CGfxShaderPtr &ptrShader);
		BOOL SetGeometryShader(const CGfxShaderPtr &ptrShader);
		BOOL SetFragmentShader(const CGfxShaderPtr &ptrShader);
		BOOL SetPrimitiveTopology(VkPrimitiveTopology topology, BOOL primitiveRestartEnable = FALSE);
		BOOL SetTessellationPatchControlPoints(uint32_t patchControlPoints);
		BOOL SetPolygonMode(VkPolygonMode polygonMode);
		BOOL SetCullMode(VkCullModeFlags cullMode);
		BOOL SetFrontFace(VkFrontFace frontFace);
		BOOL SetDepthClamp(BOOL depthClampEnable);
		BOOL SetDepthBias(BOOL depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
		BOOL SetRasterizerDiscard(BOOL rasterizerDiscardEnable);
		BOOL SetSampleCounts(VkSampleCountFlagBits rasterizationSamples);
		BOOL SetSampleShading(BOOL sampleShadingEnable, float minSampleShading);
		BOOL SetSampleMask(const VkSampleMask* pMask);
		BOOL SetSampleAlphaToCoverage(BOOL alphaToCoverageEnable);
		BOOL SetSampleAlphaToOne(BOOL alphaToOneEnable);
		BOOL SetDepthTest(BOOL depthTestEnable, BOOL depthWriteEnable, VkCompareOp depthCompareOp);
		BOOL SetDepthBoundsTest(BOOL depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds);
		BOOL SetStencilTest(BOOL stencilTestEnable, VkStencilOpState front, VkStencilOpState back);
		BOOL SetColorBlendLogic(BOOL logicOpEnable, VkLogicOp logicOp);
		BOOL SetColorBlendConstants(float r, float g, float b, float a);
		BOOL SetColorBlendAttachment(uint32_t attachment, BOOL blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask);

	public:
		uint32_t GetVertexFormat(void) const;


	protected:
		uint32_t m_vertexFormat;

	protected:
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		VkPipelineTessellationStateCreateInfo m_tessellationState;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		VkPipelineMultisampleStateCreateInfo m_multiSampleState;
		VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
		VkPipelineColorBlendStateCreateInfo m_colorBlendState;
		std::map<uint32_t, VkVertexInputAttributeDescription> m_vertexInputAttributeDescriptions;
		std::map<uint32_t, VkPipelineColorBlendAttachmentState> m_colorBlendAttachmentStates;
	};

}
