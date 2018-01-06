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

	class CROSS_EXPORT CVulkanCommandSetDepthBias : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandSetDepthBias(VkCommandBuffer vkCommandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_depthBiasConstantFactor(depthBiasConstantFactor)
			, m_depthBiasClamp(depthBiasClamp)
			, m_depthBiasSlopeFactor(depthBiasSlopeFactor)
		{
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			vkCmdSetDepthBias(m_vkCommandBuffer, m_depthBiasConstantFactor, m_depthBiasClamp, m_depthBiasSlopeFactor);
		}


	protected:
		float m_depthBiasConstantFactor;
		float m_depthBiasClamp;
		float m_depthBiasSlopeFactor;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
