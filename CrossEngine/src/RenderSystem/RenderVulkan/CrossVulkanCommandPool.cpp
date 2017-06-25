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

	CVulkanCommandPool::CVulkanCommandPool(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkCommandPool(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = m_pDevice->GetQueue()->GetQueueFamilyIndex();
		vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool);
	}

	CVulkanCommandPool::~CVulkanCommandPool(void)
	{
		for (const auto &itCommandBuffer : m_pCommandBuffers) {
			if (CVulkanCommandBuffer *pCommandBuffer = itCommandBuffer.second) {
				SAFE_DELETE(pCommandBuffer);
			}
		}

		m_pCommandBuffers.clear();
		vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	CVulkanCommandBuffer* CVulkanCommandPool::AllocCommandBuffer(VkCommandBufferLevel level)
	{
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.pNext = NULL;
		commandBufferInfo.commandPool = m_vkCommandPool;
		commandBufferInfo.level = level;
		commandBufferInfo.commandBufferCount = 1;

		VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
		int result = vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferInfo, &vkCommandBuffer);
		if (result != VK_SUCCESS) return NULL;

		CVulkanCommandBuffer *pCommandBuffer = SAFE_NEW CVulkanCommandBuffer(this, m_pDevice, vkCommandBuffer);
		m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;

		return pCommandBuffer;
	}

	void CVulkanCommandPool::FreeCommandBuffer(CVulkanCommandBuffer *pCommandBuffer)
	{
		m_pCommandBuffers.erase(pCommandBuffer);
		SAFE_DELETE(pCommandBuffer);
	}

}
