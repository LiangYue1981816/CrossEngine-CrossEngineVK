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

	class CROSS_EXPORT CRendererMemoryManager
	{
		friend class CRendererDevice;


	protected:
		CRendererMemoryManager(CRendererDevice *pDevice);
		virtual ~CRendererMemoryManager(void);


	protected:
		BOOL Create(void);
		void Destroy(void);


	public:
		CRendererMemory* AllocMemory(VkDeviceSize size, VkDeviceSize alignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags);
		void FreeMemory(CRendererMemory *pMemory);


	protected:
		uint32_t GetMemoryTypeIndex(VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags) const;

	public:
		void DumpLog(const char *szTitle) const;


	protected:
		static const VkDeviceSize HOST_MEMORY_POOL_SIZE = 16 * 1024 * 1024;
		static const VkDeviceSize DEVICE_MEMORY_POOL_SIZE = 32 * 1024 * 1024;

	protected:
		std::map<uint32_t, CRendererMemoryAllocator*> m_pAllocatorListHeads;

	protected:
		CRendererDevice *m_pDevice;
	};

}
