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

				CGLES3PipelineGraphics *pPipeline = (CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics);
				const VkPipelineRasterizationStateCreateInfo& rasterizationState = pPipeline->GetRasterizationState();
				const VkPipelineMultisampleStateCreateInfo& multisampleState = pPipeline->GetMultisampleState();
				const VkPipelineDepthStencilStateCreateInfo& depthStencilState = pPipeline->GetDepthStencilState();
				const VkPipelineColorBlendStateCreateInfo& colorBlendState = pPipeline->GetColorBlendState();

				SetRasterizerDiscard(rasterizationState);
				SetCullMode(rasterizationState);
				SetFrontFace(rasterizationState);
				SetPolygonOffset(rasterizationState);
				SetLineWidth(rasterizationState);

				SetDepthTest(depthStencilState);
				SetDepthWrite(depthStencilState);
				SetDepthFunc(depthStencilState);

				SetStencilTest(depthStencilState);
				SetStencilFunc(depthStencilState);
				SetStencilOp(depthStencilState);
				SetStencilMask(depthStencilState);
			}
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

		void SetCullMode(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			switch (rasterizationState.cullMode) {
			case VK_CULL_MODE_NONE:
				glDisable(GL_CULL_FACE);
				break;

			case VK_CULL_MODE_FRONT_BIT:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;

			case VK_CULL_MODE_BACK_BIT:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;

			case VK_CULL_MODE_FRONT_AND_BACK:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT_AND_BACK);
				break;
			}
		}

		void SetFrontFace(const VkPipelineRasterizationStateCreateInfo& rasterizationState)
		{
			switch (rasterizationState.frontFace) {
			case VK_FRONT_FACE_COUNTER_CLOCKWISE:
				glFrontFace(GL_CCW);
				break;

			case VK_FRONT_FACE_CLOCKWISE:
				glFrontFace(GL_CW);
				break;
			}
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
			switch (depthStencilState.depthCompareOp) {
			case VK_COMPARE_OP_NEVER:
				glDepthFunc(GL_NEVER);
				break;

			case VK_COMPARE_OP_LESS:
				glDepthFunc(GL_LESS);
				break;

			case VK_COMPARE_OP_EQUAL:
				glDepthFunc(GL_EQUAL);
				break;

			case VK_COMPARE_OP_LESS_OR_EQUAL:
				glDepthFunc(GL_LEQUAL);
				break;

			case VK_COMPARE_OP_GREATER:
				glDepthFunc(GL_GREATER);
				break;

			case VK_COMPARE_OP_NOT_EQUAL:
				glDepthFunc(GL_NOTEQUAL);
				break;

			case VK_COMPARE_OP_GREATER_OR_EQUAL:
				glDepthFunc(GL_GEQUAL);
				break;

			case VK_COMPARE_OP_ALWAYS:
				glDepthFunc(GL_ALWAYS);
				break;
			}
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


	protected:
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;
	};

}
