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

	CVulkanBuffer::CVulkanBuffer(CVulkanDevice *pDevice, CVulkanResourceManager *pManager)
		: CVulkanResource(pDevice, pManager)
		, m_vkBuffer(VK_NULL_HANDLE)

		, m_usage(0)
		, m_bufferSize(0)
		, m_memorySize(0)

		, m_pMemory(NULL)
	{

	}

	CVulkanBuffer::~CVulkanBuffer(void)
	{
		ASSERT(m_pMemory == NULL);
		ASSERT(m_vkBuffer == VK_NULL_HANDLE);
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
			CALL_VK_FUNCTION_THROW(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
			m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
			m_pMemory->BindBuffer(m_vkBuffer);

			m_usage = usage;
			m_bufferSize = size;
			m_memorySize = m_pMemory->GetSize();

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanBuffer::Destroy(void)
	{
		if (m_vkBuffer) {
			vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}
		
		if (m_pMemory) {
			m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
		}
		
		m_pMemory = NULL;
		m_vkBuffer = VK_NULL_HANDLE;
	}

	BOOL CVulkanBuffer::UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		if (pBuffer == NULL) {
			return FALSE;
		}

		try {
			if (m_pMemory->IsHostVisible()) {
				CALL_VK_FUNCTION_THROW(CopyData(size, offset, pBuffer));
			}
			else {
				CALL_VK_FUNCTION_THROW(TransferData(size, offset, pBuffer));
			}

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);

			return FALSE;
		}
	}

	VkResult CVulkanBuffer::CopyData(VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		void *pAddress = NULL;

		CALL_VK_FUNCTION_RETURN(m_pMemory->BeginMapMemory(size, offset, &pAddress));
		{
			memcpy(pAddress, pBuffer, size);
		}
		CALL_VK_FUNCTION_RETURN(m_pMemory->EndMapMemory());

		return VK_SUCCESS;
	}

	VkResult CVulkanBuffer::TransferData(VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		VkAccessFlags dstAccessMask = 0;
		VkPipelineStageFlags dstStageMask = 0;
		if (m_usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_INDEX_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; }
		if (m_usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; }
		if (m_usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) { dstAccessMask |= VK_ACCESS_UNIFORM_READ_BIT; dstStageMask |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; }

		CVulkanStagingBufferAutoRelease buffer(m_pDevice, size);
		{
			CALL_VK_FUNCTION_RETURN(buffer.GetBuffer()->TransferBuffer(m_vkBuffer, VK_ACCESS_MEMORY_WRITE_BIT, dstAccessMask, VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask, size, offset, pBuffer));
			CALL_VK_FUNCTION_RETURN(m_pDevice->GetQueue()->Submit(buffer.GetBuffer()->GetCommandBuffer()->GetCommandBuffer(), VK_NULL_HANDLE));
			CALL_VK_FUNCTION_RETURN(m_pDevice->GetQueue()->WaitIdle());
		}

		return VK_SUCCESS;
	}

	VkBuffer CVulkanBuffer::GetBuffer(void) const
	{
		return m_vkBuffer;
	}

	VkDeviceSize CVulkanBuffer::GetBufferSize(void) const
	{
		return m_bufferSize;
	}

	VkDeviceSize CVulkanBuffer::GetMemorySize(void) const
	{
		return m_memorySize;
	}

	VkBufferUsageFlags CVulkanBuffer::GetUsage(void) const
	{
		return m_usage;
	}

	void CVulkanBuffer::DumpLog(void) const
	{
		if (m_vkBuffer) {
			LOGI("\t\tBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_vkBuffer, m_bufferSize, m_memorySize, CVulkanHelper::vkBufferUsageFlagsToString(m_usage));
		}
	}

}
