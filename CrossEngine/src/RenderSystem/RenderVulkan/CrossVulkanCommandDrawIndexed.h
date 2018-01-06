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

	class CROSS_EXPORT CVulkanCommandDrawIndexed : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandDrawIndexed(VkCommandBuffer vkCommandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_indexCount(indexCount)
			, m_instanceCount(instanceCount)
			, m_firstIndex(firstIndex)
			, m_vertexOffset(vertexOffset)
			, m_firstInstance(firstInstance)
		{
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			vkCmdDrawIndexed(m_vkCommandBuffer, m_indexCount, m_instanceCount, m_firstIndex, m_vertexOffset, m_firstInstance);
		}


	protected:
		uint32_t m_indexCount;
		uint32_t m_instanceCount;
		uint32_t m_firstIndex;
		int32_t m_vertexOffset;
		uint32_t m_firstInstance;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
