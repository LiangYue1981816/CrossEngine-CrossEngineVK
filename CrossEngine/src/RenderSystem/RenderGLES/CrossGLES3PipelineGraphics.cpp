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
		: CGfxPipelineGraphics(pResourceManager)
		, m_vertexFormat(0)
	{

	}

	CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
	{

	}

	HANDLE CGLES3PipelineGraphics::GetHandle(void) const
	{
		return (HANDLE)m_program;
	}

	BOOL CGLES3PipelineGraphics::Create(HANDLE hRenderPass)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::CreateVertexInputAttributeDescriptions(void)
	{
		return TRUE;
	}

	void CGLES3PipelineGraphics::Destroy(void)
	{

	}

	void CGLES3PipelineGraphics::SetDefault(void)
	{

	}

	BOOL CGLES3PipelineGraphics::SetVertexShader(const CGfxShaderPtr &ptrShader)
	{
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
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetPrimitiveTopology(VkPrimitiveTopology topology, BOOL primitiveRestartEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetTessellationPatchControlPoints(uint32_t patchControlPoints)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetPolygonMode(VkPolygonMode polygonMode)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetCullMode(VkCullModeFlags cullMode)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetFrontFace(VkFrontFace frontFace)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthClamp(BOOL depthClampEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthBias(BOOL depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetRasterizerDiscard(BOOL rasterizerDiscardEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleCounts(VkSampleCountFlagBits rasterizationSamples)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleShading(BOOL sampleShadingEnable, float minSampleShading)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleMask(const uint32_t* pMask)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleAlphaToCoverage(BOOL alphaToCoverageEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetSampleAlphaToOne(BOOL alphaToOneEnable)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthTest(BOOL depthTestEnable, BOOL depthWriteEnable, VkCompareOp depthCompareOp)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetDepthBoundsTest(BOOL depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetStencilTest(BOOL stencilTestEnable, VkStencilOpState front, VkStencilOpState back)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetColorBlendLogic(BOOL logicOpEnable, VkLogicOp logicOp)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetColorBlendConstants(float r, float g, float b, float a)
	{
		return TRUE;
	}

	BOOL CGLES3PipelineGraphics::SetColorBlendAttachment(uint32_t attachment, BOOL blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp, VkColorComponentFlags colorWriteMask)
	{
		return TRUE;
	}

	uint32_t CGLES3PipelineGraphics::GetVertexFormat(void) const
	{
		return m_vertexFormat;
	}

	void CGLES3PipelineGraphics::DumpLog(void) const
	{

	}

}
