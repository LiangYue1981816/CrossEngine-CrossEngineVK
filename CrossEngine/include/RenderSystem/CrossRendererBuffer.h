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

	class CROSS_EXPORT CRendererBuffer : public CRendererResource
	{
		friend class CRendererBufferManager;


	protected:
		CRendererBuffer(CRendererDevice *pDevice, CRendererResourceManager *pManager);
		virtual ~CRendererBuffer(void);


	public:
		virtual BOOL Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
		virtual void Destroy(void);
		virtual BOOL UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const;
		virtual void DumpLog(void) const;


	public:
		VkBuffer GetBuffer(void) const;
		VkDeviceSize GetBufferSize(void) const;
		VkDeviceSize GetMemorySize(void) const;
		VkBufferUsageFlags GetUsage(void) const;


	protected:
		VkDeviceSize m_bufferSize;
		VkDeviceSize m_memorySize;
		VkBufferUsageFlags m_usage;

	protected:
		VkBuffer m_vkBuffer;
		CRendererMemory *m_pMemory;
	};

}
