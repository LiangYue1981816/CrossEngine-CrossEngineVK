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

	int CVulkanStagingBufferManager::Create(void)
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		createInfo.queueFamilyIndex = m_pDevice->GetQueueFamilyIndex();
		CALL_VK_FUNCTION_RETURN(vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool));

		return VK_SUCCESS;
	}

	void CVulkanStagingBufferManager::Destroy(void)
	{
		for (auto &itBuffer : m_pBuffers) {
			SAFE_DELETE(itBuffer.second);
		}

		if (m_vkCommandPool) {
			vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_pBuffers.clear();
		m_vkCommandPool = VK_NULL_HANDLE;
	}

	CVulkanStagingBuffer* CVulkanStagingBufferManager::AllocBuffer(VkDeviceSize size)
	{
		mutex_autolock mutex(&m_mutex);

		CVulkanStagingBuffer *pBuffer = SAFE_NEW CVulkanStagingBuffer(m_pDevice, m_vkCommandPool, size);
		m_pBuffers[pBuffer] = pBuffer;

		return pBuffer;
	}

	void CVulkanStagingBufferManager::FreeBuffer(CVulkanStagingBuffer *pBuffer)
	{
		if (pBuffer) {
			mutex_autolock mutex(&m_mutex);
			m_pBuffers.erase(pBuffer);
			SAFE_DELETE(pBuffer);
		}
	}

	void CVulkanStagingBufferManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize bufferSize = 0;
		VkDeviceSize memorySize = 0;

		LOGI("%s\n", szTitle);
		{
			for (const auto &itBuffer : m_pBuffers) {
				itBuffer.second->DumpLog();
				bufferSize += itBuffer.second->GetBufferSize();
				memorySize += itBuffer.second->GetMemorySize();
				count++;
			}
		}
		LOGI("*** %d objects found, total buffer size %d total memory size %d\n", count, bufferSize, memorySize);
		LOGI("\n");
	}

}
