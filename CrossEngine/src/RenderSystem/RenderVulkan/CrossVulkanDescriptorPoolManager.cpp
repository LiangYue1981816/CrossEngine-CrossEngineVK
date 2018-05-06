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

	CVulkanDescriptorPoolManager::CVulkanDescriptorPoolManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CVulkanDescriptorPoolManager::~CVulkanDescriptorPoolManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	int CVulkanDescriptorPoolManager::Create(void)
	{
		return VK_SUCCESS;
	}

	void CVulkanDescriptorPoolManager::Destroy(void)
	{
		for (const auto &itDescriptorPool : m_pDescriptorPoolListHeads) {
			if (CVulkanDescriptorPool *pDescriptorPool = itDescriptorPool.second) {
				CVulkanDescriptorPool *pDescriptorPoolNext = NULL;
				do {
					pDescriptorPoolNext = pDescriptorPool->pNext;
					SAFE_DELETE(pDescriptorPool);
				} while (pDescriptorPool = pDescriptorPoolNext);
			}
		}

		m_pDescriptorPoolListHeads.clear();
	}

	void CVulkanDescriptorPoolManager::ResetDescriptorSetPool(uint32_t pool)
	{
		if (CVulkanDescriptorPool *pDescriptorPool = m_pDescriptorPoolListHeads[pool]) {
			do {
				pDescriptorPool->ResetDescriptorSetPool();
			} while (pDescriptorPool = pDescriptorPool->pNext);
		}
	}

	void CVulkanDescriptorPoolManager::AllocDescriptorSetPool(uint32_t pool)
	{
		mutex_autolock mutex(&m_mutex);

		if (m_pDescriptorPoolListHeads[pool] == NULL) {
			m_pDescriptorPoolListHeads[pool] = SAFE_NEW CVulkanDescriptorPool(m_pDevice);
		}
	}

	CGfxDescriptorSetPtr CVulkanDescriptorPoolManager::AllocDescriptorSet(uint32_t pool, const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout)
	{
		do {
			if (CVulkanDescriptorPool *pDescriptorPool = m_pDescriptorPoolListHeads[pool]) {
				do {
					if (CVulkanDescriptorSet *pDescriptorSet = pDescriptorPool->AllocDescriptorSet(ptrDescriptorSetLayout)) {
						return CGfxDescriptorSetPtr(pDescriptorSet);
					}
				} while (pDescriptorPool = pDescriptorPool->pNext);
			}

			CVulkanDescriptorPool *pDescriptorPool = SAFE_NEW CVulkanDescriptorPool(m_pDevice);
			pDescriptorPool->pNext = m_pDescriptorPoolListHeads[pool];
			m_pDescriptorPoolListHeads[pool] = pDescriptorPool;
		} while (TRUE);
	}

	void CVulkanDescriptorPoolManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;

		LOGI("%s\n", szTitle);
		{
			for (const auto &itDescriptorPool : m_pDescriptorPoolListHeads) {
				if (CVulkanDescriptorPool *pDescriptorPool = itDescriptorPool.second) {
					do {
						LOGI("\tPool = %d\n", itDescriptorPool.first);
						pDescriptorPool->DumpLog();
						count += pDescriptorPool->GetDescriptorSetCount();
					} while (pDescriptorPool = pDescriptorPool->pNext);
				}
			}
		}
		LOGI("*** %d objects found\n", count);
		LOGI("\n");
	}

}
