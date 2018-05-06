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

	#define UNIFORM_BUFFER_ALIGNMENT 0x100

	class CROSS_EXPORT CVulkanUniformBuffer : public CGfxUniformBuffer, public CVulkanBuffer
	{
		friend class CVulkanBufferManager;


	protected:
		CVulkanUniformBuffer(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager);
		virtual ~CVulkanUniformBuffer(void);


	public:
		CVulkanDevice* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		BOOL Create(size_t size, const void *pBuffer, BOOL bDynamic);
		void Destroy(void);
		void DumpLog(void) const;

	public:
		BOOL SetData(size_t offset, size_t size, const void *pBuffer) const;

	public:
		const VkDescriptorBufferInfo& GetDescriptorBufferInfo(void) const;

	public:
		BOOL IsDynamic(void) const;
		size_t GetBufferSize(void) const;
		size_t GetMemorySize(void) const;


	protected:
		VkDescriptorBufferInfo m_vkDescriptorBufferInfo;
	};

}
