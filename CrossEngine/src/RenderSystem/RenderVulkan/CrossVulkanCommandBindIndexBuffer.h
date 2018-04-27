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

	class CROSS_EXPORT CVulkanCommandBindIndexBuffer : public CGfxCommandBase
	{
		friend class CVulkanCommandBuffer;


	protected:
		CVulkanCommandBindIndexBuffer(VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, VkIndexType indexType)
			: m_vkCommandBuffer(vkCommandBuffer)
			, m_offset(offset)
			, m_indexType(indexType)
		{
			m_ptrIndexBuffer = ptrIndexBuffer;
			Execute();
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrIndexBuffer.IsNull() || m_ptrIndexBuffer->GetHandle() == NULL) {
				return;
			}

			VkBuffer vkBuffer = (VkBuffer)m_ptrIndexBuffer->GetHandle();
			vkCmdBindIndexBuffer(m_vkCommandBuffer, vkBuffer, m_offset, m_indexType);
		}


	protected:
		VkDeviceSize m_offset;
		VkIndexType m_indexType;
		CGfxIndexBufferPtr m_ptrIndexBuffer;

	protected:
		VkCommandBuffer m_vkCommandBuffer;
	};

}
