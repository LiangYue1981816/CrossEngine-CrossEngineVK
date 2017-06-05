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

	class CROSS_EXPORT CVulkanBuffer
	{
	protected:
		CVulkanBuffer(CVulkanDevice *pDevice);
		virtual ~CVulkanBuffer(void);


	protected:
		BOOL Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
		void Destroy(void);

	protected:
		BOOL UpdateData(size_t offset, size_t size, const void *pBuffer) const;
		BOOL CopyData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;
		BOOL TransferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;


	protected:
		VkBuffer m_vkBuffer;
		CVulkanMemory *m_pMemory;

	protected:
		VkDeviceSize m_bufferSize;
		VkBufferUsageFlags m_usage;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
