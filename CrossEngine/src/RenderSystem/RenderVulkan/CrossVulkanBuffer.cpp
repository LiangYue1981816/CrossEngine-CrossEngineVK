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

	CVulkanBuffer::CVulkanBuffer(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_pMemory(NULL)
		, m_vkBuffer(VK_NULL_HANDLE)

		, m_size(0)
		, m_usage(0)
	{

	}

	CVulkanBuffer::~CVulkanBuffer(void)
	{

	}

	BOOL CVulkanBuffer::Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags)
	{
		try {
			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.size = size;
			createInfo.usage = usage;
			createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = NULL;
			CALL_VK_FUNCTION_THROW(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
			m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
			m_pMemory->BindBuffer(m_vkBuffer);

			m_size = size;
			m_usage = usage;

			return TRUE;
		}
		catch (int err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanBuffer::Destroy(void)
	{
		if (m_vkBuffer) {
			vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_pMemory) {
			m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
		}

		m_pMemory = NULL;
		m_vkBuffer = VK_NULL_HANDLE;
	}

	BOOL CVulkanBuffer::UpdateData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
	{
		try {
			if (pBuffer) {
				if (m_pMemory->IsHostVisible()) {
					CALL_VK_FUNCTION_THROW(CopyData(offset, size, pBuffer));
				}
				else {
					CALL_VK_FUNCTION_THROW(TransferData(offset, size, pBuffer));
				}
			}

			return TRUE;
		}
		catch (int err) {
			CVulkanInstance::SetLastError(err);
			return FALSE;
		}
	}

	int CVulkanBuffer::CopyData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
	{
		void *pAddress = NULL;

		CALL_VK_FUNCTION_RETURN(m_pMemory->BeginMapMemory(offset, size, &pAddress));
		{
			memcpy(pAddress, pBuffer, size);
		}
		CALL_VK_FUNCTION_RETURN(m_pMemory->FlushMappedMemory(offset, size));
		CALL_VK_FUNCTION_RETURN(m_pMemory->EndMapMemory());

		return VK_SUCCESS;
	}

	int CVulkanBuffer::TransferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const
	{
		VkAccessFlags dstAccessMask = 0;
		VkPipelineStageFlags dstStageMask = 0;

		if (m_usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_INDEX_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; }
		if (m_usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; }
		if (m_usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_UNIFORM_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; }

		CVulkanStagingBufferPtr ptrStagingBuffer = m_pDevice->GetStagingBufferManager()->AllocBuffer(size);
		CALL_VK_FUNCTION_RETURN(ptrStagingBuffer->TransferBuffer(m_vkBuffer, VK_ACCESS_MEMORY_WRITE_BIT, dstAccessMask, VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask, size, offset, pBuffer));

		return VK_SUCCESS;
	}

	VkBufferUsageFlags CVulkanBuffer::GetUsage(void) const
	{
		return m_usage;
	}

}
