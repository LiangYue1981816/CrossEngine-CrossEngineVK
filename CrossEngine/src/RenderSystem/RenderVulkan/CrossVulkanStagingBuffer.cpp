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

	CVulkanStagingBuffer::CVulkanStagingBuffer(CVulkanDevice *pDevice, VkCommandPool vkCommandPool, VkDeviceSize size)
		: m_pDevice(pDevice)

		, m_size(size)
		, m_pMemory(NULL)
		, m_vkBuffer(VK_NULL_HANDLE)

		, m_vkCommandPool(vkCommandPool)
		, m_vkCommandBuffer(VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.pNext = NULL;
		commandBufferInfo.commandPool = m_vkCommandPool;
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandBufferCount = 1;
		vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferInfo, &m_vkCommandBuffer);

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = NULL;
		vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer);

		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);
		m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_pMemory->BindBuffer(m_vkBuffer);
	}

	CVulkanStagingBuffer::~CVulkanStagingBuffer(void)
	{
		vkFreeCommandBuffers(m_pDevice->GetDevice(), m_vkCommandPool, 1, &m_vkCommandBuffer);

		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}

	int CVulkanStagingBuffer::TransferImage(VkImage vkImage, uint32_t mipLevels, uint32_t arrayLayers, uint32_t regionCount, const VkBufferImageCopy *pRegions, VkDeviceSize size, const void *pPixels) const
	{
		void *pAddress = NULL;

		CALL_VK_FUNCTION_RETURN(m_pMemory->BeginMapMemory(0, size, &pAddress));
		{
			memcpy(pAddress, pPixels, size);
		}
		CALL_VK_FUNCTION_RETURN(m_pMemory->FlushMappedMemory(0, size));
		CALL_VK_FUNCTION_RETURN(m_pMemory->EndMapMemory());

		CALL_VK_FUNCTION_RETURN(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
		{
			VkImageSubresourceRange range;
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = mipLevels;
			range.baseArrayLayer = 0;
			range.layerCount = arrayLayers;

			vkCmdSetImageLayout(m_vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, range);
			vkCmdCopyBufferToImage(m_vkCommandBuffer, m_vkBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regionCount, pRegions);
			vkCmdSetImageLayout(m_vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, range);
		}
		CALL_VK_FUNCTION_RETURN(vkEndCommandBuffer(m_vkCommandBuffer));

		CALL_VK_FUNCTION_RETURN(vkSubmitCommandBuffer(m_pDevice->GetQueue()->GetQueue(), m_vkCommandBuffer, VK_NULL_HANDLE));
		CALL_VK_FUNCTION_RETURN(vkQueueWaitIdle(m_pDevice->GetQueue()->GetQueue()));

		return VK_SUCCESS;
	}

	int CVulkanStagingBuffer::TransferBuffer(VkBuffer vkBuffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		void *pAddress = NULL;

		CALL_VK_FUNCTION_RETURN(m_pMemory->BeginMapMemory(0, size, &pAddress));
		{
			memcpy(pAddress, pBuffer, size);
		}
		CALL_VK_FUNCTION_RETURN(m_pMemory->FlushMappedMemory(0, size));
		CALL_VK_FUNCTION_RETURN(m_pMemory->EndMapMemory());

		CALL_VK_FUNCTION_RETURN(vkBeginCommandBufferPrimary(m_vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));
		{
			VkBufferCopy region;
			region.srcOffset = 0;
			region.dstOffset = offset;
			region.size = size;
			vkCmdCopyBuffer(m_vkCommandBuffer, m_vkBuffer, vkBuffer, 1, &region);

			VkBufferMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			barrier.pNext = NULL;
			barrier.srcAccessMask = srcAccessMask;
			barrier.dstAccessMask = dstAccessMask;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = vkBuffer;
			barrier.offset = offset;
			barrier.size = size;
			vkCmdPipelineBarrier(m_vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, NULL, 1, &barrier, 0, NULL);
		}
		CALL_VK_FUNCTION_RETURN(vkEndCommandBuffer(m_vkCommandBuffer));

		CALL_VK_FUNCTION_RETURN(vkSubmitCommandBuffer(m_pDevice->GetQueue()->GetQueue(), m_vkCommandBuffer, VK_NULL_HANDLE));
		CALL_VK_FUNCTION_RETURN(vkQueueWaitIdle(m_pDevice->GetQueue()->GetQueue()));

		return VK_SUCCESS;
	}

	int CVulkanStagingBuffer::TransferIndexBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		return TransferBuffer(vkBuffer, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, size, offset, pBuffer);
	}

	int CVulkanStagingBuffer::TransferVertexBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		return TransferBuffer(vkBuffer, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, size, offset, pBuffer);
	}

	int CVulkanStagingBuffer::TransferUniformBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const
	{
		return TransferBuffer(vkBuffer, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT | VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, size, offset, pBuffer);
	}

	size_t CVulkanStagingBuffer::GetBufferSize(void) const
	{
		return m_size;
	}

	size_t CVulkanStagingBuffer::GetMemorySize(void) const
	{
		return m_pMemory->GetSize();
	}

	void CVulkanStagingBuffer::DumpLog(void) const
	{
		if (m_vkBuffer) {
			LOGI("\t\tBuffer 0x%x: buffer size = %d memory size = %d\n", m_vkBuffer, GetBufferSize(), GetMemorySize());
		}
	}

}
