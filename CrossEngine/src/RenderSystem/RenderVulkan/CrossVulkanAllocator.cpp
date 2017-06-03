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

	CVulkanAllocator::CVulkanAllocator(void)
		: m_memAllocator(TRUE, FALSE, TRUE)

		, m_dwAllocatedSize(0)
		, m_dwMaxAllocatedSize(0)
	{
		m_vkCallback = { 0 };
		m_vkCallback.pUserData = this;
		m_vkCallback.pfnAllocation = CVulkanAllocator::vkAllocationFunction;
		m_vkCallback.pfnReallocation = CVulkanAllocator::vkReallocationFunction;
		m_vkCallback.pfnFree = CVulkanAllocator::vkFreeFunction;
		m_vkCallback.pfnInternalAllocation = CVulkanAllocator::vkInternalAllocationNotification;
		m_vkCallback.pfnInternalFree = CVulkanAllocator::vkInternalFreeNotification;
	}

	CVulkanAllocator::~CVulkanAllocator(void)
	{

	}

	DWORD CVulkanAllocator::GetAllocatedSize(void) const
	{
		return m_dwAllocatedSize;
	}

	DWORD CVulkanAllocator::GetMaxAllocatedSize(void) const
	{
		return m_dwMaxAllocatedSize;
	}

	const VkAllocationCallbacks* CVulkanAllocator::GetAllocationCallbacks(void) const
	{
		return &m_vkCallback;
	}

	void* CVulkanAllocator::Alloc(size_t size, MEMTYPE type)
	{
		return m_memAllocator.Alloc(size, type);
	}

	void CVulkanAllocator::Free(void *ptr)
	{
		m_memAllocator.Free(ptr);
	}

	void* VKAPI_PTR CVulkanAllocator::vkAllocationFunction(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		if (CVulkanAllocator *pAllocator = (CVulkanAllocator *)pUserData) {
			void *pPointer = pAllocator->Alloc(size, MEMTYPE_HEAP);
			pAllocator->m_dwAllocatedSize += MEM_SIZE(pPointer);
			pAllocator->m_dwMaxAllocatedSize = pAllocator->m_dwMaxAllocatedSize >= pAllocator->m_dwAllocatedSize ? pAllocator->m_dwMaxAllocatedSize : pAllocator->m_dwAllocatedSize;
			return pPointer;
		}

		return NULL;
	}

	void* VKAPI_PTR CVulkanAllocator::vkReallocationFunction(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
	{
		if (CVulkanAllocator *pAllocator = (CVulkanAllocator *)pUserData) {
			void *pPointer = pAllocator->Alloc(size, MEMTYPE_HEAP);
			memcpy(pPointer, pOriginal, min(MEM_SIZE(pPointer), MEM_SIZE(pOriginal)));
			pAllocator->m_dwAllocatedSize += MEM_SIZE(pPointer);
			pAllocator->m_dwAllocatedSize -= MEM_SIZE(pOriginal);
			pAllocator->m_dwMaxAllocatedSize = pAllocator->m_dwMaxAllocatedSize >= pAllocator->m_dwAllocatedSize ? pAllocator->m_dwMaxAllocatedSize : pAllocator->m_dwAllocatedSize;
			pAllocator->Free(pOriginal);
			return pPointer;
		}

		return NULL;
	}

	void VKAPI_PTR CVulkanAllocator::vkFreeFunction(void *pUserData, void *pPointer)
	{
		if (CVulkanAllocator *pAllocator = (CVulkanAllocator *)pUserData) {
			if (pPointer) {
				pAllocator->m_dwAllocatedSize -= MEM_SIZE(pPointer);
				pAllocator->Free(pPointer);
			}
		}
	}

	void VKAPI_PTR CVulkanAllocator::vkInternalAllocationNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
	{
		if (CVulkanAllocator *pAllocator = (CVulkanAllocator *)pUserData) {
			pAllocator->m_dwAllocatedSize += size;
			pAllocator->m_dwMaxAllocatedSize = pAllocator->m_dwMaxAllocatedSize >= pAllocator->m_dwAllocatedSize ? pAllocator->m_dwMaxAllocatedSize : pAllocator->m_dwAllocatedSize;
		}
	}

	void VKAPI_PTR CVulkanAllocator::vkInternalFreeNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
	{
		if (CVulkanAllocator *pAllocator = (CVulkanAllocator *)pUserData) {
			pAllocator->m_dwAllocatedSize -= size;
			pAllocator->m_dwMaxAllocatedSize = pAllocator->m_dwMaxAllocatedSize >= pAllocator->m_dwAllocatedSize ? pAllocator->m_dwMaxAllocatedSize : pAllocator->m_dwAllocatedSize;
		}
	}

}
