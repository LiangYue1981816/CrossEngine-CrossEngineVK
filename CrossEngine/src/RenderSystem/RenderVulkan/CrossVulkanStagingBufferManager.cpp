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

	CVulkanStagingBufferManager::CVulkanStagingBufferManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)

		, m_vkCommandPool(VK_NULL_HANDLE)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CVulkanStagingBufferManager::~CVulkanStagingBufferManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	BOOL CVulkanStagingBufferManager::Create(void)
	{
		try {
			VkCommandPoolCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			createInfo.queueFamilyIndex = m_pDevice->GetQueue()->GetQueueFamilyIndex();
			CALL_VK_FUNCTION_THROW(vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanStagingBufferManager::Destroy(void)
	{
		for (const auto &itBuffer : m_pBuffers) {
			if (CVulkanStagingBuffer *pBuffer = itBuffer.second) {
				SAFE_DELETE(pBuffer);
			}
		}

		if (m_vkCommandPool) {
			vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_pBuffers.clear();
		m_vkCommandPool = VK_NULL_HANDLE;
	}

	CVulkanStagingBuffer* CVulkanStagingBufferManager::AllocBuffer(VkDeviceSize size)
	{
		mutex_autolock mutex(m_mutex);

		CVulkanStagingBuffer *pBuffer = SAFE_NEW CVulkanStagingBuffer(m_pDevice, m_vkCommandPool, size);
		m_pBuffers[pBuffer] = pBuffer;

		return pBuffer;
	}

	void CVulkanStagingBufferManager::FreeBuffer(CVulkanStagingBuffer *pBuffer)
	{
		mutex_autolock mutex(m_mutex);

		const auto &itBuffer = m_pBuffers.find(pBuffer);
		if (itBuffer != m_pBuffers.end()) m_pBuffers.erase(itBuffer);

		SAFE_DELETE(pBuffer);
	}

}
