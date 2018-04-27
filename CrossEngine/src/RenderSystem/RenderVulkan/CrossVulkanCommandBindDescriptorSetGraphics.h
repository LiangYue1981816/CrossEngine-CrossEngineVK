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

	class CROSS_EXPORT CVulkanCommandBindDescriptorSetGraphics : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandBindDescriptorSetGraphics(VkCommandBuffer vkCommandBuffer, const CGfxDescriptorSetPtr &ptrDescriptorSet, const CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
			: m_vkCommandBuffer(vkCommandBuffer)
		{
			m_ptrDescriptorSet = ptrDescriptorSet;
			m_ptrPipelineGraphics = ptrPipelineGraphics;
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrDescriptorSet.IsNull() || m_ptrDescriptorSet->GetHandle() == NULL) {
				return;
			}

			if (m_ptrPipelineGraphics.IsNull() || m_ptrPipelineGraphics->GetHandle() == NULL) {
				return;
			}

			VkDescriptorSet vkDescriptorSet = (VkDescriptorSet)m_ptrDescriptorSet->GetHandle();
			VkPipelineLayout vkPipelineLayout = ((CVulkanPipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics))->GetPipelineLayout();
			std::vector<uint32_t> offsets = ((CVulkanDescriptorSet *)((CGfxDescriptorSet *)m_ptrDescriptorSet))->GetUniformBufferOffsets();
			vkCmdBindDescriptorSets(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet, offsets.size(), offsets.data());
		}


	protected:
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
