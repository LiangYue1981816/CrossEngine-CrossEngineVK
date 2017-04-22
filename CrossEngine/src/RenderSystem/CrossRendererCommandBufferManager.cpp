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

	CVulkanCommandBufferManager::CVulkanCommandBufferManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CVulkanCommandBufferManager::~CVulkanCommandBufferManager(void)
	{
		ASSERT(m_pCommandPools.empty());
	}

	BOOL CVulkanCommandBufferManager::Create(void)
	{
		return m_pCommandPools.empty() ? TRUE : FALSE;
	}

	void CVulkanCommandBufferManager::Destroy(void)
	{
		for (const auto &itCommandPool : m_pCommandPools) {
			if (CVulkanCommandPool *pCommandPool = itCommandPool.second) {
				SAFE_DELETE(pCommandPool);
			}
		}

		m_pCommandPools.clear();
	}

	CVulkanCommandBuffer* CVulkanCommandBufferManager::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return GetCommandPool(pool)->AllocCommandBuffer(level);
	}

	void CVulkanCommandBufferManager::FreeCommandBuffer(uint32_t pool, CVulkanCommandBuffer *pCommandBuffer)
	{
		GetCommandPool(pool)->FreeCommandBuffer(pCommandBuffer);
	}

	void CVulkanCommandBufferManager::ResetCommandPool(uint32_t pool, BOOL bReleaseResources)
	{
		GetCommandPool(pool)->ResetCommandPool(bReleaseResources);
	}

	CVulkanCommandPool* CVulkanCommandBufferManager::GetCommandPool(uint32_t pool)
	{
		const auto &itCommandPool = m_pCommandPools.find(pool);
		if (itCommandPool != m_pCommandPools.end()) return itCommandPool->second;

		CVulkanCommandPool *pCommandPool = SAFE_NEW CVulkanCommandPool(m_pDevice);
		m_pCommandPools[pool] = pCommandPool;

		return pCommandPool;
	}

}
