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

	class CROSS_EXPORT CVulkanMemoryAllocator
	{
		friend class CVulkanMemory;
		friend class CVulkanMemoryManager;


	protected:
		typedef struct mem_node {
			rb_node node;
			VkDeviceSize size;
			CVulkanMemory *pFreeListHead;
		} mem_node;


	protected:
		CVulkanMemoryAllocator(CVulkanDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize, VkMemoryPropertyFlags memoryPropertyFlags);
		virtual ~CVulkanMemoryAllocator(void);


	protected:
		CVulkanMemory* AllocMemory(VkDeviceSize size, VkDeviceSize alignment);
		void FreeMemory(CVulkanMemory *pMemory);

	protected:
		void InitNodes(uint32_t numNodes);
		void InsertMemory(CVulkanMemory *pMemory);
		void RemoveMemory(CVulkanMemory *pMemory);
		CVulkanMemory* SearchMemory(VkDeviceSize size) const;

	protected:
		BOOL IsEmpty(void) const;
		uint32_t GetMemoryTypeIndex(void) const;

	protected:
		VkDeviceSize GetFullSize(void) const;
		VkDeviceSize GetAllocatedSize(void) const;
		uint32_t GetAllocationCount(void) const;

	protected:
		void DumpLog(void) const;


	protected:
		VkDeviceMemory m_vkMemory;

	protected:
		uint32_t m_type;
		VkFlags m_flags;
		VkDeviceSize m_size;

	protected:
		rb_root m_root;
		mem_node *m_nodes;
		CVulkanMemory *m_pListHead;

	protected:
		CVulkanDevice *m_pDevice;

	protected:
		CVulkanMemoryAllocator *pNext;
		CVulkanMemoryAllocator *pPrev;
	};

}
