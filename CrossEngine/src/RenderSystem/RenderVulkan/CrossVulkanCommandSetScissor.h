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

	class CROSS_EXPORT CVulkanCommandSetScissor : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandSetScissor(VkCommandBuffer vkCommandBuffer, int x, int y, uint32_t width, uint32_t height)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_x(x)
			, m_y(y)
			, m_width(width)
			, m_height(height)
		{

		}


	protected:
		virtual void Execute(void) const
		{
			VkRect2D scissor = {};
			scissor.offset.x = m_x;
			scissor.offset.y = m_y;
			scissor.extent.width = m_width;
			scissor.extent.height = m_height;
			vkCmdSetScissor(m_vkCommandBuffer, 0, 1, &scissor);
		}


	protected:
		int m_x;
		int m_y;
		uint32_t m_width;
		uint32_t m_height;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
