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
		CGLES3CommandBindPipelineGraphics(CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
		{
			m_ptrPipelineGraphics = ptrPipelineGraphics;
		}
		virtual ~CGLES3CommandBindPipelineGraphics(void)
		{
			m_ptrPipelineGraphics.Release();
		}


	protected:
		virtual void Execute(void)
		{
			if (m_ptrPipelineGraphics.IsNull()) {
				glUseProgram(0);
				glBindProgramPipeline(0);
			}
			else {
				glUseProgram(0);
				glBindProgramPipeline((GLuint)m_ptrPipelineGraphics->GetHandle());

				const CGLES3PipelineGraphics *pPipeline = (CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics);
				const VkPipelineRasterizationStateCreateInfo& rasterizationState = pPipeline->GetRasterizationState();
				const VkPipelineMultisampleStateCreateInfo& multisampleState = pPipeline->GetMultisampleState();
				const VkPipelineDepthStencilStateCreateInfo& depthStencilState = pPipeline->GetDepthStencilState();
				const VkPipelineColorBlendStateCreateInfo& colorBlendState = pPipeline->GetColorBlendState();

				SetDepthClamp(rasterizationState);
				SetRasterizerDiscard(rasterizationState);
				SetPolygonMode(rasterizationState);
				SetCullMode(rasterizationState);
				SetFrontFace(rasterizationState);
				SetPolygonOffset(rasterizationState);
				SetLineWidth(rasterizationState);

				SetDepthTest(depthStencilState);
				SetDepthWrite(depthStencilState);
				SetDepthFunc(depthStencilState);
				SetDepthBoundTest(depthStencilState);

				SetStencilTest(depthStencilState);
				SetStencilFunc(depthStencilState);
				SetStencilOp(depthStencilState);
				SetStencilMask(depthStencilState);

				SetLogicOp(colorBlendState);
				SetBlendEnable(colorBlendState);
				SetBlendColor(colorBlendState);
				SetBlendEquationSeparate(colorBlendState);
				SetBlendFuncSeparate(colorBlendState);
				SetColorMask(colorBlendState);
			}
		}

		void SetDepthClamp(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			// Not support
		}

		void SetRasterizerDiscard(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			if (rasterizationState.rasterizerDiscardEnable) {
				glEnable(GL_RASTERIZER_DISCARD);
			}
			else {
				glDisable(GL_RASTERIZER_DISCARD);
			}
		}

		void SetPolygonMode(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			// Not support
		}

		void SetCullMode(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			if (rasterizationState.cullMode != VK_CULL_MODE_NONE) {
				glEnable(GL_CULL_FACE);
				glCullFace(CGLES3Helper::glTranslateCullMode(rasterizationState.cullMode));
			}
			else {
				glDisable(GL_CULL_FACE);
			}
		}

		void SetFrontFace(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			glFrontFace(CGLES3Helper::glTranslateFrontFace(rasterizationState.frontFace));
		}

		void SetPolygonOffset(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			if (rasterizationState.depthBiasEnable) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(rasterizationState.depthBiasSlopeFactor, rasterizationState.depthBiasConstantFactor);
			}
			else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
		}

		void SetLineWidth(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			glLineWidth(rasterizationState.lineWidth);
		}

		void SetDepthTest(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			if (depthStencilState.depthTestEnable) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
		}

		void SetDepthWrite(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			if (depthStencilState.depthWriteEnable) {
				glDepthMask(GL_TRUE);
			}
			else {
				glDepthMask(GL_FALSE);
			}
		}

		void SetDepthFunc(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			glDepthFunc(CGLES3Helper::glTranslateCompareOp(depthStencilState.depthCompareOp));
		}

		void SetDepthBoundTest(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			// Not support
		}

		void SetStencilTest(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			if (depthStencilState.stencilTestEnable) {
				glEnable(GL_STENCIL_TEST);
			}
			else {
				glDisable(GL_STENCIL_TEST);
			}
		}

		void SetStencilFunc(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
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
		}

		void SetStencilOp(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
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
		}

		void SetStencilMask(const VkPipelineDepthStencilStateCreateInfo& depthStencilState)
		{
			glStencilMaskSeparate(GL_FRONT, depthStencilState.front.writeMask);
			glStencilMaskSeparate(GL_BACK, depthStencilState.back.writeMask);
		}

		void SetLogicOp(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			// Not support
		}

		void SetBlendEnable(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			// Not support for different draw buffer
			/*
			for (int indexAttachment = 0; indexAttachment < colorBlendState.attachmentCount; indexAttachment++) {
				if (colorBlendState.pAttachments[0].blendEnable) {
					glEnablei(GL_BLEND, indexAttachment);
				}
				else {
					glDisablei(GL_BLEND, indexAttachment);
				}
			}
			/*/
			if (colorBlendState.pAttachments[0].blendEnable) {
				glEnable(GL_BLEND);
			}
			else {
				glDisable(GL_BLEND);
			}
			//*/
		}

		void SetBlendColor(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			glBlendColor(colorBlendState.blendConstants[0], colorBlendState.blendConstants[1], colorBlendState.blendConstants[2], colorBlendState.blendConstants[3]);
		}

		void SetBlendEquationSeparate(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			// Not support for different draw buffer
			/*
			for (int indexAttachment = 0; indexAttachment < colorBlendState.attachmentCount; indexAttachment++) {
				glBlendEquationSeparatei(
					indexAttachment,
					CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments[0].colorBlendOp),
					CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments[0].alphaBlendOp));
			}
			/*/
			glBlendEquationSeparate(
				CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments[0].colorBlendOp),
				CGLES3Helper::glTranslateBlendOp(colorBlendState.pAttachments[0].alphaBlendOp));
			//*/
		}

		void SetBlendFuncSeparate(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			// Not support for different draw buffer
			/*
			for (int indexAttachment = 0; indexAttachment < colorBlendState.attachmentCount; indexAttachment++) {
				glBlendFuncSeparatei(
					indexAttachment,
					CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].srcColorBlendFactor),
					CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].dstColorBlendFactor),
					CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].srcAlphaBlendFactor),
					CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].dstAlphaBlendFactor));
			}
			/*/
			glBlendFuncSeparate(
				CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].srcColorBlendFactor),
				CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].dstColorBlendFactor),
				CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].srcAlphaBlendFactor),
				CGLES3Helper::glTranslateBlendFactor(colorBlendState.pAttachments[0].dstAlphaBlendFactor));
			//*/
		}

		void SetColorMask(const VkPipelineColorBlendStateCreateInfo& colorBlendState)
		{
			// Not support for different draw buffer
			/*
			for (int indexAttachment = 0; indexAttachment < colorBlendState.attachmentCount; indexAttachment++) {
				glColorMaski(
					indexAttachment,
					colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_R_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_G_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_B_BIT ? GL_TRUE : GL_FALSE,
					colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_A_BIT ? GL_TRUE : GL_FALSE);
			}
			/*/
			glColorMask(
				colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_R_BIT ? GL_TRUE : GL_FALSE,
				colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_G_BIT ? GL_TRUE : GL_FALSE,
				colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_B_BIT ? GL_TRUE : GL_FALSE,
				colorBlendState.pAttachments[0].colorWriteMask & VK_COLOR_COMPONENT_A_BIT ? GL_TRUE : GL_FALSE);
			//*/
		}


	protected:
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;
	};

}
