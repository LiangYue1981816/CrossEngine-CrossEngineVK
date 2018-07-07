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

	CVulkanVertexBuffer::CVulkanVertexBuffer(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanBuffer(pDevice)
		, CGfxVertexBuffer(pResourceManager)
		, m_binding(0)
	{

	}

	CVulkanVertexBuffer::~CVulkanVertexBuffer(void)
	{

	}

	CVulkanDevice* CVulkanVertexBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanVertexBuffer::GetHandle(void) const
	{
		return m_vkBuffer;
	}

	BOOL CVulkanVertexBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic, uint32_t format, uint32_t binding)
	{
		CALL_BOOL_FUNCTION_RETURN(CVulkanBuffer::Create(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, bDynamic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		CALL_BOOL_FUNCTION_RETURN(CVulkanBuffer::SetData(0, size, pBuffer));
		m_binding = binding;
		return TRUE;
	}

	void CVulkanVertexBuffer::Destroy(void)
	{
		m_binding = 0;
		CVulkanBuffer::Destroy();
	}

	BOOL CVulkanVertexBuffer::SetData(size_t offset, size_t size, const void *pBuffer) const
	{
		return CVulkanBuffer::SetData(offset, size, pBuffer);
	}

	BOOL CVulkanVertexBuffer::IsDynamic(void) const
	{
		return CVulkanBuffer::IsDynamic();
	}

	size_t CVulkanVertexBuffer::GetBufferSize(void) const
	{
		return CVulkanBuffer::GetBufferSize();
	}

	size_t CVulkanVertexBuffer::GetMemorySize(void) const
	{
		return CVulkanBuffer::GetMemorySize();
	}

	uint32_t CVulkanVertexBuffer::GetBinding(void) const
	{
		return m_binding;
	}

	void CVulkanVertexBuffer::DumpLog(void) const
	{
		if (m_vkBuffer) {
			char szUsage[_MAX_STRING];
			LOGI("\t\tVertexBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_vkBuffer, GetBufferSize(), GetMemorySize(), CVulkanHelper::BufferUsageFlagsToString(m_usage, szUsage));
		}
	}

}
