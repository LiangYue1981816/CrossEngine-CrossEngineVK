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

#include "_CrossEngine.h"


namespace CrossEngine {

	CVulkanIndexBuffer::CVulkanIndexBuffer(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanBuffer(pDevice)
		, CGfxIndexBuffer(pResourceManager)
	{

	}

	CVulkanIndexBuffer::~CVulkanIndexBuffer(void)
	{

	}

	HANDLE CVulkanIndexBuffer::GetHandle(void) const
	{
		return m_vkBuffer;
	}

	BOOL CVulkanIndexBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic)
	{
		CALL_BOOL_FUNCTION_RETURN(CVulkanBuffer::Create(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bDynamic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		CALL_BOOL_FUNCTION_RETURN(UpdateData(0, size, pBuffer));
		return TRUE;
	}

	void CVulkanIndexBuffer::Destroy(void)
	{
		CVulkanBuffer::Destroy();
	}

	BOOL CVulkanIndexBuffer::UpdateData(size_t offset, size_t size, const void *pBuffer) const
	{
		return CVulkanBuffer::UpdateData(offset, size, pBuffer);
	}

	size_t CVulkanIndexBuffer::GetBufferSize(void) const
	{
		return m_bufferSize;
	}

	size_t CVulkanIndexBuffer::GetMemorySize(void) const
	{
		return m_pMemory->GetSize();
	}

	void CVulkanIndexBuffer::DumpLog(void) const
	{
		if (m_vkBuffer) {
			LOGI("\t\tIndexBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_vkBuffer, m_bufferSize, m_pMemory->GetSize(), CVulkanHelper::vkBufferUsageFlagsToString(m_usage));
		}
	}

}
