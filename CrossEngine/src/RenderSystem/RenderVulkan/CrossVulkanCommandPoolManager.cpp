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

	CVulkanCommandPoolManager::CVulkanCommandPoolManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CVulkanCommandPoolManager::~CVulkanCommandPoolManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	int CVulkanCommandPoolManager::Create(void)
	{
		return VK_SUCCESS;
	}

	void CVulkanCommandPoolManager::Destroy(void)
	{
		for (auto &itCommandPool : m_pCommandPools) {
			SAFE_DELETE(itCommandPool.second);
		}

		m_pCommandPools.clear();
	}

	void CVulkanCommandPoolManager::ResetCommandBufferPool(uint32_t pool)
	{
		if (m_pCommandPools[pool]) {
			m_pCommandPools[pool]->ResetCommandBufferPool();
		}
	}

	void CVulkanCommandPoolManager::AllocCommandBufferPool(uint32_t pool)
	{
		mutex_autolock mutex(&m_mutex);

		if (m_pCommandPools[pool] == NULL) {
			m_pCommandPools[pool] = SAFE_NEW CVulkanCommandPool(m_pDevice);
		}
	}

	CGfxCommandBufferPtr CVulkanCommandPoolManager::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return m_pCommandPools[pool] ? CGfxCommandBufferPtr(m_pCommandPools[pool]->AllocCommandBuffer(level)) : CGfxCommandBufferPtr(NULL);
	}

}
