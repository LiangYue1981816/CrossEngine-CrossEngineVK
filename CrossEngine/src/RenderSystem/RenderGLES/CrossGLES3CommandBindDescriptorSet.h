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

	class CROSS_EXPORT CGLES3CommandBindDescriptorSet : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindDescriptorSet(const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexPass)
			: m_indexPass(indexPass)
		{
			m_ptrDescriptorSet = ptrDescriptorSet;
			m_ptrRenderPass = ptrRenderPass;
		}


	protected:
		virtual void Execute(void) const
		{
			GLuint indexTexUnit = 0;

			const CGLES3DescriptorSet *pDescriptorSet = (CGLES3DescriptorSet *)((CGfxDescriptorSet *)m_ptrDescriptorSet);
			const CGLES3RenderPass *pRenderPass = (CGLES3RenderPass *)((CGfxRenderPass *)m_ptrRenderPass);
			const GLSubpassInformation* pSubPass = pRenderPass->GetSubpass(m_indexPass);
			const CGLES3DescriptorSetLayout *pDescriptorSetLayout = pDescriptorSet->GetDescriptorSetLayout();

			const std::map<uint32_t, std::map<uint32_t, uint32_t>> &uniformBlockBindings = pDescriptorSetLayout->GetUniformBlockBindings();
			for (const auto &itUniformBlockBinding : uniformBlockBindings) {
				for (const auto &itBinding : itUniformBlockBinding.second) {
					GLuint program = itUniformBlockBinding.first;
					GLuint binding = itBinding.first;
					GLuint location = itBinding.second;

					const CGfxUniformBufferPtr &ptrUniformBuffer = pDescriptorSet->GetUniformBuffer(binding);
					if (ptrUniformBuffer.IsNull()) continue;

					const uint32_t offset = pDescriptorSet->GetUniformBufferOffset(binding);
					const VkDescriptorBufferInfo &bufferInfo = ptrUniformBuffer->GetDescriptorBufferInfo(pDescriptorSetLayout->GetSet(), binding);
					glBindBufferRange(GL_UNIFORM_BUFFER, binding, (GLuint)ptrUniformBuffer->GetHandle(), bufferInfo.offset + offset, bufferInfo.range);
					glUniformBlockBinding(program, location, binding);
				}
			}

			const std::map<uint32_t, std::map<uint32_t, uint32_t>> &inputAttachmentBindings = pDescriptorSetLayout->GetInputAttachmentBindings();
			for (const auto &itInputAttachmentBinding : inputAttachmentBindings) {
				for (const auto &itBinding : itInputAttachmentBinding.second) {
					GLuint program = itInputAttachmentBinding.first;
					GLuint binding = itBinding.first;
					GLuint location = itBinding.second;

					const CGfxRenderTexturePtr &ptrRenderTexture = pDescriptorSet->GetRenderTexture(binding);
					if (ptrRenderTexture.IsNull()) continue;

					const CGLES3RenderTexture *pRenderTexture = (CGLES3RenderTexture *)((CGfxRenderTexture *)ptrRenderTexture);
					glActiveTexture(GL_TEXTURE0 + indexTexUnit);
					glBindSampler(indexTexUnit, pRenderTexture->GetSampler());
					glBindTexture(pRenderTexture->GetTarget(), (GLuint)pRenderTexture->GetHandle());
					glProgramUniform1i(program, location, indexTexUnit);

					indexTexUnit++;
				}
			}

			const std::map<uint32_t, std::map<uint32_t, uint32_t>> &sampledImageBindings = pDescriptorSetLayout->GetSampledImageBindings();
			for (const auto &itSampledImageBinding : sampledImageBindings) {
				for (const auto &itBinding : itSampledImageBinding.second) {
					GLuint program = itSampledImageBinding.first;
					GLuint binding = itBinding.first;
					GLuint location = itBinding.second;

					const CGfxTexturePtr &ptrTexture = pDescriptorSet->GetTexture(binding);
					if (ptrTexture.IsNull()) continue;

					const CGLES3Texture *pTexture = (CGLES3Texture *)((CGfxTexture *)ptrTexture);
					glActiveTexture(GL_TEXTURE0 + indexTexUnit);
					glBindSampler(indexTexUnit, pTexture->GetSampler());
					glBindTexture(pTexture->GetTarget(), (GLuint)pTexture->GetHandle());
					glProgramUniform1i(program, location, indexTexUnit);

					indexTexUnit++;
				}
			}
		}


	protected:
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
		CGfxRenderPassPtr m_ptrRenderPass;
		uint32_t m_indexPass;
	};

}
