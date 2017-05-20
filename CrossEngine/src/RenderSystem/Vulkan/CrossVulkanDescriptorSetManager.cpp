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

	CVulkanDescriptorSetManager::CVulkanDescriptorSetManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CVulkanDescriptorSetManager::~CVulkanDescriptorSetManager(void)
	{

	}

	BOOL CVulkanDescriptorSetManager::Create(void)
	{
		return TRUE;
	}

	void CVulkanDescriptorSetManager::Destroy(void)
	{
		for (const auto &itDescriptorPool : m_pDescriptorPools) {
			if (CVulkanDescriptorPool *pDescriptorPool = itDescriptorPool.second) {
				SAFE_DELETE(pDescriptorPool);
			}
		}

		m_pDescriptorPools.clear();
	}

	CVulkanDescriptorSet* CVulkanDescriptorSetManager::AllocDescriptorSet(uint32_t pool, VkDescriptorSetLayout vkSetLayout, const uint32_t *typesUsedCount)
	{
		return GetDescriptorPool(pool)->AllocDescriptorSet(vkSetLayout, typesUsedCount);
	}

	void CVulkanDescriptorSetManager::FreeDescriptorSet(uint32_t pool, CVulkanDescriptorSet *pDescriptorSet)
	{
		GetDescriptorPool(pool)->FreeDescriptorSet(pDescriptorSet);
	}

	CVulkanDescriptorPool* CVulkanDescriptorSetManager::GetDescriptorPool(uint32_t pool)
	{
		const auto &itDescriptorPool = m_pDescriptorPools.find(pool);
		if (itDescriptorPool != m_pDescriptorPools.end()) return itDescriptorPool->second;

		CVulkanDescriptorPool *pDescriptorPool = SAFE_NEW CVulkanDescriptorPool(m_pDevice);
		m_pDescriptorPools[pool] = pDescriptorPool;

		return pDescriptorPool;
	}

	void CVulkanDescriptorSetManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;

		LOGI("\n");
		LOGI("%s\n", szTitle);
		{
			for (const auto &itDescriptorPool : m_pDescriptorPools) {
				if (const CVulkanDescriptorPool *pDescriptorPool = itDescriptorPool.second) {
					LOGI("\tPool = %d\n", itDescriptorPool.first);
					pDescriptorPool->DumpLog();
					count += pDescriptorPool->GetDescriptorSetCount();
				}
			}
		}
		LOGI("*** %d objects found\n", count);
		LOGI("\n");
	}

}
