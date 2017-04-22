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

	CVulkanUniformBuffer::CVulkanUniformBuffer(CVulkanDevice *pDevice, CVulkanResourceManager *pManager)
		: CVulkanBuffer(pDevice, pManager)
	{

	}

	CVulkanUniformBuffer::~CVulkanUniformBuffer(void)
	{

	}

	BOOL CVulkanUniformBuffer::Create(VkDeviceSize size, VkDeviceSize offset, const void *pBuffer)
	{
		CALL_BOOL_FUNCTION_RETURN(CVulkanBuffer::Create(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
		CALL_BOOL_FUNCTION_RETURN(UpdateData(size, offset, pBuffer));

		m_vkDescriptorBufferInfo.buffer = m_vkBuffer;
		m_vkDescriptorBufferInfo.offset = offset;
		m_vkDescriptorBufferInfo.range = size;

		return TRUE;
	}

	const VkDescriptorBufferInfo& CVulkanUniformBuffer::GetDescriptorBufferInfo(void) const
	{
		return m_vkDescriptorBufferInfo;
	}

}
