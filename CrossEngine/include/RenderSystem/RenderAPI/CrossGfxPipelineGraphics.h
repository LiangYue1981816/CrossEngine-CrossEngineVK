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

	class CROSS_EXPORT CGfxPipelineGraphics : public CGfxResource
	{
	protected:
		CGfxPipelineGraphics(CGfxResourceManager *pResourceManager)
			: CGfxResource(pResourceManager)
		{

		}
		virtual ~CGfxPipelineGraphics(void)
		{

		}


	public:
		virtual BOOL Create(void) = 0;
		virtual void Destroy(void) = 0;
		virtual void DumpLog(void) const = 0;

	public:
		virtual BOOL SetVertexShader(const CGfxShaderPtr &ptrShader) = 0;
		virtual BOOL SetTessellationControlShader(const CGfxShaderPtr &ptrShader) = 0;
		virtual BOOL SetTessellationEvaluationShader(const CGfxShaderPtr &ptrShader) = 0;
		virtual BOOL SetGeometryShader(const CGfxShaderPtr &ptrShader) = 0;
		virtual BOOL SetFragmentShader(const CGfxShaderPtr &ptrShader) = 0;
		virtual BOOL SetPrimitiveTopology(PrimitiveTopology topology, BOOL primitiveRestartEnable = FALSE) = 0;
		virtual BOOL SetTessellationPatchControlPoints(uint32_t patchControlPoints) = 0;
		virtual BOOL SetPolygonMode(PolygonMode polygonMode) = 0;
		virtual BOOL SetCullMode(CullModeFlags cullMode) = 0;
		virtual BOOL SetFrontFace(FrontFace frontFace) = 0;
		virtual BOOL SetDepthClamp(BOOL depthClampEnable) = 0;
		virtual BOOL SetDepthBias(BOOL depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) = 0;
		virtual BOOL SetRasterizerDiscard(BOOL rasterizerDiscardEnable) = 0;
		virtual BOOL SetSampleCounts(SampleCountFlagBits rasterizationSamples) = 0;
		virtual BOOL SetSampleShading(BOOL sampleShadingEnable, float minSampleShading) = 0;
		virtual BOOL SetSampleMask(const uint32_t* pMask) = 0;
		virtual BOOL SetSampleAlphaToCoverage(BOOL alphaToCoverageEnable) = 0;
		virtual BOOL SetSampleAlphaToOne(BOOL alphaToOneEnable) = 0;
		virtual BOOL SetDepthTest(BOOL depthTestEnable, BOOL depthWriteEnable, CompareOp depthCompareOp) = 0;
		virtual BOOL SetDepthBoundsTest(BOOL depthBoundsTestEnable, float minDepthBounds, float maxDepthBounds) = 0;
		virtual BOOL SetStencilTest(BOOL stencilTestEnable, StencilOpState front, StencilOpState back) = 0;
		virtual BOOL SetColorBlendLogic(BOOL logicOpEnable, LogicOp logicOp) = 0;
		virtual BOOL SetColorBlendConstants(float r, float g, float b, float a) = 0;
		virtual BOOL SetColorBlendAttachment(uint32_t attachment, BOOL blendEnable, BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp, ColorComponentFlags colorWriteMask) = 0;

	public:
		virtual uint32_t GetVertexFormat(void) const = 0;
	};

}
