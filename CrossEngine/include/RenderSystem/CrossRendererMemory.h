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

	class CROSS_EXPORT CRendererMemory
	{
		friend class CRendererMemoryManager;
		friend class CRendererMemoryAllocator;


	protected:
		CRendererMemory(CRendererMemoryAllocator *pAllocator, CRendererDevice *pDevice, VkDeviceMemory vkMemory, VkFlags flags, VkDeviceSize size, VkDeviceSize offset);
		virtual ~CRendererMemory(void);


	protected:
		CRendererMemoryAllocator* GetAllocator(void) const;

	public:
		BOOL IsHostVisible(void) const;
		BOOL IsHostCoherent(void) const;
		BOOL IsHostCached(void) const;

		VkDeviceSize GetSize(void) const;

		VkResult BindImage(VkImage vkImage) const;
		VkResult BindBuffer(VkBuffer vkBuffer) const;

		VkResult BeginMapMemory(VkDeviceSize size, VkDeviceSize offset, void **ppAddress) const;
		VkResult FlushMappedMemory(VkDeviceSize size, VkDeviceSize offset) const;
		VkResult EndMapMemory(void) const;


	protected:
		VkDeviceMemory m_vkMemory;

	protected:
		VkFlags m_flags;
		VkDeviceSize m_size;
		VkDeviceSize m_offset;

	protected:
		CRendererMemoryAllocator *m_pAllocator;
		CRendererDevice *m_pDevice;

	protected:
		BOOL bInUse;

		CRendererMemory *pNext;
		CRendererMemory *pPrev;
		CRendererMemory *pFreeNext;
		CRendererMemory *pFreePrev;
	};

}
