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

	class CROSS_EXPORT CGLES3CommandBindPipelineGraphics : public CGLES3CommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
		{
			m_ptrPipelineGraphics = ptrPipelineGraphics;
		}


	protected:
		virtual void Execute(void) const
		{
			const CGLES3PipelineGraphics *pPipeline = (CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics);
			const VkPipelineInputAssemblyStateCreateInfo& inputAssemblyState = pPipeline->GetInputAssemblyState();
			const VkPipelineRasterizationStateCreateInfo& rasterizationState = pPipeline->GetRasterizationState();
			const VkPipelineMultisampleStateCreateInfo& multisampleState = pPipeline->GetMultisampleState();
			const VkPipelineDepthStencilStateCreateInfo& depthStencilState = pPipeline->GetDepthStencilState();
			const VkPipelineColorBlendStateCreateInfo& colorBlendState = pPipeline->GetColorBlendState();

			InitStates();

			SetProgram(pPipeline);
			SetPrimitiveRestart(inputAssemblyState);
			SetRasterizerDiscard(rasterizationState);
			SetCullMode(rasterizationState);
			SetFrontFace(rasterizationState);
			SetPolygonOffset(rasterizationState);
			SetLineWidth(rasterizationState);
			SetSampleMask(multisampleState);
			SetAlphaToCoverage(multisampleState);
			SetDepthWrite(depthStencilState);
			SetDepthTest(depthStencilState);
			SetStencilTest(depthStencilState);
			SetBlend(colorBlendState);
			SetColorMask(colorBlendState);
		}

		void InitStates(void) const
		{
			glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
			glDisable(GL_RASTERIZER_DISCARD);
			glDisable(GL_CULL_FACE);
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_BLEND);
			glDisable(GL_SAMPLE_MASK);
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}

		void SetProgram(const CGLES3PipelineGraphics *pPipeline) const
		{
			glBindProgramPipeline((GLuint)pPipeline->GetHandle());
		}

		void SetPrimitiveRestart(const VkPipelineInputAssemblyStateCreateInfo& inputAssemblyState) const
		{
			if (inputAssemblyState.primitiveRestartEnable) {
				glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
			}
		}

		void SetRasterizerDiscard(const VkPipelineRasterizationStateCreateInfo& rasterizationState) const
		{
			if (rasterizationState.rasterizerDiscardEnable) {
				glEnable(GL_RASTERIZER_DISCARD);
			}
		}

		void SetCullMode(const VkPipelineRasterizationStateCreateInfo& rasterizationState) const
		{
			if (rasterizationState.cullMode != VK_CULL_MODE_NONE) {
				glEnable(GL_CULL_FACE);
				glCullFace(CGLES3Helper::glTranslateCullMode(rasterizationState.cullMode));
			}
		}

		void SetFrontFace(const VkPipelineRasterizationStateCreateInfo& rasterizationState) const
		{
			glFrontFace(CGLES3Helper::glTranslateFrontFace(rasterizationState.frontFace));
		}

		void SetPolygonOffset(const VkPipelineRasterizationStateCreateInfo& rasterizationState) const
		{
			if (rasterizationState.depthBiasEnable) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(rasterizationState.depthBiasSlopeFactor, rasterizationState.depthBiasConstantFactor);
			}
		}

		void SetLineWidth(const VkPipelineRasterizationStateCreateInfo& rasterizationState) const
		{
			glLineWidth(rasterizationState.lineWidth);
		}

		void SetSampleMask(const VkPipelineMultisampleStateCreateInfo& multisampleState) const
		{
			if (multisampleState.pSampleMask) {
				glEnable(GL_SAMPLE_MASK);
				glSampleMaski(multisampleState.rasterizationSamples, *multisampleState.pSampleMask);
			}
		}

		void SetAlphaToCoverage(const VkPipelineMultisampleStateCreateInfo& multisampleState) const
		{
			if (multisampleState.alphaToCoverageEnable) {
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
		}

		void SetDepthWrite(const VkPipelineDepthStencilStateCreateInfo& depthStencilState) const
		{
			glDepthMask(depthStencilState.depthWriteEnable ? GL_TRUE : GL_FALSE);
		}

		void SetDepthTest(const VkPipelineDepthStencilStateCreateInfo& depthStencilState) const
		{
			if (depthStencilState.depthTestEnable) {
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(CGLES3Helper::glTranslateCompareOp(depthStencilState.depthCompareOp));
			}
		}

		void SetStencilTest(const VkPipelineDepthStencilStateCreateInfo& depthStencilState) const
		{
			if (depthStencilState.stencilTestEnable) {
				glEnable(GL_STENCIL_TEST);

				glStencilFuncSeparate(
					GL_FRONT, 
					CGLES3Helper::glTranslateCompareOp(depthStencilState.front.compareOp), 
					depthStencilState.front.reference, 
					depthStencilState.front.compareMask);
				glStencilFuncSeparate(
					GL_BACK, 
					CGLES3Helper::glTranslateCompareOp(depthStencilState.back.compareOp), 
					depthStencilState.back.reference, 
					depthStencilState.back.compareMask);

				glStencilOpSeparate(
					GL_FRONT,
					CGLES3Helper::glTranslateStencilOp(depthStencilState.front.failOp),
					CGLES3Helper::glTranslateStencilOp(depthStencilState.front.depthFailOp),
					CGLES3Helper::glTranslateStencilOp(depthStencilState.front.passOp));
				glStencilOpSeparate(
					GL_BACK,
					CGLES3Helper::glTranslateStencilOp(depthStencilState.back.failOp),
					CGLES3Helper::glTranslateStencilOp(depthStencilState.back.depthFailOp),
					CGLES3Helper::glTranslateStencilOp(depthStencilState.back.passOp));

				glStencilMaskSeparate(GL_FRONT, depthStencilState.front.writeMask);
				glStencilMaskSeparate(GL_BACK, depthStencilState.back.writeMask);
			}
		}

		void SetBlend(const VkPipelineColorBlendStateCreateInfo& colorBlendState) const
		{
			if (colorBlendState.pAttachments) {
				if (colorBlendState.pAttachments->blendEnable) {
					glEnable(GL_BLEND);

					glBlendFuncSeparate(
						CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments->srcColorBlendFactor),
						CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments->dstColorBlendFactor),
						CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments->srcAlphaBlendFactor),
						CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments->dstAlphaBlendFactor));

					glBlendEquationSeparate(
						CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments->colorBlendOp),
						CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments->alphaBlendOp));

					glBlendColor(colorBlendState.blendConstants[0], colorBlendState.blendConstants[1], colorBlendState.blendConstants[2], colorBlendState.blendConstants[3]);
				}
			}
		}

		void SetColorMask(const VkPipelineColorBlendStateCreateInfo& colorBlendState) const
		{
			if (colorBlendState.pAttachments) {
				glColorMask(
					colorBlendState.pAttachments->colorWriteMask & VK_COLOR_COMPONENT_R_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments->colorWriteMask & VK_COLOR_COMPONENT_G_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments->colorWriteMask & VK_COLOR_COMPONENT_B_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments->colorWriteMask & VK_COLOR_COMPONENT_A_BIT ? GL_TRUE : GL_FALSE);
			}
		}


	protected:
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;
	};

}
