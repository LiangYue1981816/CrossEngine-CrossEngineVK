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

	class CROSS_EXPORT CVulkanCommandSetViewport : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandSetViewport(VkCommandBuffer vkCommandBuffer, float x, float y, float width, float height, float minDepth, float maxDepth)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_x(x)
			, m_y(y)
			, m_width(width)
			, m_height(height)
			, m_minDepth(minDepth)
			, m_maxDepth(maxDepth)
		{

		}


	protected:
		virtual void Execute(void) const
		{
			VkViewport viewport = {};
			viewport.x = m_x;
			viewport.y = m_y;
			viewport.width = m_width;
			viewport.height = m_height;
			viewport.minDepth = m_minDepth;
			viewport.maxDepth = m_maxDepth;
			vkCmdSetViewport(m_vkCommandBuffer, 0, 1, &viewport);
		}


	protected:
		float m_x;
		float m_y;
		float m_width;
		float m_height;
		float m_minDepth;
		float m_maxDepth;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
