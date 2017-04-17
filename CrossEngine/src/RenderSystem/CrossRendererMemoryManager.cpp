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

	CRendererMemoryManager::CRendererMemoryManager(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CRendererMemoryManager::~CRendererMemoryManager(void)
	{
		ASSERT(m_pAllocatorListHeads.empty());
	}

	BOOL CRendererMemoryManager::Create(void)
	{
		return m_pAllocatorListHeads.size() == 0 ? TRUE : FALSE;
	}

	void CRendererMemoryManager::Destroy(void)
	{
		for (const auto &itAllocator : m_pAllocatorListHeads) {
			if (CRendererMemoryAllocator *pAllocator = itAllocator.second) {
				CRendererMemoryAllocator *pAllocatorNext = NULL;
				do {
					pAllocatorNext = pAllocator->pNext;
					SAFE_DELETE(pAllocator);
				} while (pAllocator = pAllocatorNext);
			}
		}

		m_pAllocatorListHeads.clear();
	}

	CRendererMemory* CRendererMemoryManager::AllocMemory(VkDeviceSize size, VkDeviceSize alignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
	{
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryTypeBits, memoryPropertyFlags);
		if (memoryTypeIndex == -1) return NULL;

		do {
			if (CRendererMemoryAllocator *pAllocator = m_pAllocatorListHeads[memoryTypeIndex]) {
				do {
					if (CRendererMemory *pMemory = pAllocator->AllocMemory(size, alignment)) {
						return pMemory;
					}
				} while (pAllocator = pAllocator->pNext);
			}

			VkDeviceSize memorySize = 0;
			memorySize = (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ? HOST_MEMORY_POOL_SIZE : memorySize;
			memorySize = (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) ? DEVICE_MEMORY_POOL_SIZE : memorySize;
			memorySize = max(size, memorySize);

			CRendererMemoryAllocator *pAllocator = SAFE_NEW CRendererMemoryAllocator(m_pDevice, memoryTypeIndex, memorySize, memoryPropertyFlags);

			if (m_pAllocatorListHeads[memoryTypeIndex]) {
				m_pAllocatorListHeads[memoryTypeIndex]->pPrev = pAllocator;
				pAllocator->pNext = m_pAllocatorListHeads[memoryTypeIndex];
			}

			m_pAllocatorListHeads[memoryTypeIndex] = pAllocator;
		} while (TRUE);
	}

	void CRendererMemoryManager::FreeMemory(CRendererMemory *pMemory)
	{
		CRendererMemoryAllocator *pAllocator = pMemory->GetAllocator();
		pAllocator->FreeMemory(pMemory);

		if (pAllocator->IsEmpty()) {
			uint32_t memoryTypeIndex = pAllocator->GetMemoryTypeIndex();

			if (m_pAllocatorListHeads[memoryTypeIndex] == pAllocator) {
				m_pAllocatorListHeads[memoryTypeIndex] =  pAllocator->pNext;
			}

			if (pAllocator->pPrev) {
				pAllocator->pPrev->pNext = pAllocator->pNext;
			}

			if (pAllocator->pNext) {
				pAllocator->pNext->pPrev = pAllocator->pPrev;
			}

			SAFE_DELETE(pAllocator);
		}
	}

	uint32_t CRendererMemoryManager::GetMemoryTypeIndex(VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags) const
	{
		for (uint32_t index = 0; index < m_pDevice->GetMemoryProperties().memoryTypeCount; index++) {
			if ((memoryTypeBits & 1) == 1 && (m_pDevice->GetMemoryProperties().memoryTypes[index].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
				return index;
			}
			memoryTypeBits >>= 1;
		}

		return -1;
	}

	void CRendererMemoryManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize size = 0;

		LOGI("\n");
		LOGI("%s\n", szTitle);
		{
			for (const auto &itAllocator : m_pAllocatorListHeads) {
				LOGI("\tMemory type index: %d\n", itAllocator.first);

				if (const CRendererMemoryAllocator *pAllocator = itAllocator.second) {
					do {
						pAllocator->DumpLog();
						size += pAllocator->GetFullSize();
						count += pAllocator->GetAllocationCount();
					} while (pAllocator = pAllocator->pNext);
				}
			}
		}
		LOGI("*** %d objects found, total size %d\n", count, size);
		LOGI("\n");
	}

}
