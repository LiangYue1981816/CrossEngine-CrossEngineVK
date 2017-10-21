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

	uint32_t CMemAllocator::dwAllocatorCount = 0;

	CMemAllocator::CMemAllocator(BOOL bHeap, BOOL bStack, BOOL bMultiThread)
		: m_mutex(NULL)

		, m_pPoolAllocator(NULL)
		, m_pHeapAllocator(NULL)
		, m_pStackAllocator(NULL)
		
		, m_dwPointerCount(0)
		, m_dwAllocatedSize(0)
		, m_dwMaxPointerCount(0)
		, m_dwMaxAllocatedSize(0)
	{
		dwAllocatorCount++;

#ifdef ENABLE_MEMORY_POOL

		if (bHeap) {
			m_pHeapAllocator = HEAP_Create();
			m_pPoolAllocator = POOL_Create(m_pHeapAllocator);
		}

		if (bStack) {
			m_pStackAllocator = STACK_Create();
		}

#endif

		if (bMultiThread) {
			pthread_mutex_init(&m_mutex, NULL);
		}
	}

	CMemAllocator::~CMemAllocator(void)
	{
		dwAllocatorCount--;

		LOGI("\n");
		LOGI("Host Memory ...\n");
		LOGI("\tLeak memory size = %2.2fMB (%dB)\n", m_dwAllocatedSize / (1024.0f*1024.0f), m_dwAllocatedSize);
		LOGI("\tLeak pointer number = %d\n", m_dwPointerCount);
		LOGI("\tMax pointer number = %d\n", m_dwMaxPointerCount);
		LOGI("\tMax allocated size = %2.2fMB (%dB)\n", m_dwMaxAllocatedSize / (1024.0f*1024.0f), m_dwMaxAllocatedSize);
		LOGI("\n");

		if (dwAllocatorCount == 0) {
			LOGI("Memory Leak Check ...\n");
			dump_memory_objects();
		}

		if (m_pHeapAllocator && m_pPoolAllocator) {
			POOL_Destroy(m_pHeapAllocator, m_pPoolAllocator);
			HEAP_Destroy(m_pHeapAllocator);
		}

		if (m_pStackAllocator) {
			STACK_Destroy(m_pStackAllocator);
		}

		if (m_mutex) {
			pthread_mutex_destroy(&m_mutex);
		}
	}

	void* CMemAllocator::Alloc(size_t size, MEMTYPE memType)
	{
		uint32_t *pPointer = NULL;
		const uint32_t dwMemSize = (uint32_t)ALIGN_16BYTE(size);

		if (dwMemSize > 0) {
			mutex_autolock mutex(m_mutex);

			if (pPointer == NULL && memType == MEMTYPE_STACK) {
				pPointer = (uint32_t *)STACK_Alloc(m_pStackAllocator, dwMemSize);
			}

			if (pPointer == NULL) {
				pPointer = (uint32_t *)POOL_Alloc(m_pHeapAllocator, m_pPoolAllocator, dwMemSize);
			}

			if (pPointer == NULL) {
				pPointer = (uint32_t *)HEAP_Alloc(m_pHeapAllocator, dwMemSize);
			}

			if (pPointer == NULL) {
				pPointer = (uint32_t *)_malloc(dwMemSize + 16);

				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = dwMemSize;
			}

			ASSERT(pPointer);
			ASSERT((uint64_t)pPointer == ((uint64_t)pPointer / 16) * 16);

			m_dwPointerCount++;
			m_dwAllocatedSize += MEM_SIZE(pPointer);

			if (m_dwMaxPointerCount < m_dwPointerCount) m_dwMaxPointerCount = m_dwPointerCount;
			if (m_dwMaxAllocatedSize < m_dwAllocatedSize) m_dwMaxAllocatedSize = m_dwAllocatedSize;
		}

		return pPointer;
	}

	void CMemAllocator::Free(void *ptr)
	{
		if (ptr) {
			BOOL bFree = FALSE;
			const uint32_t dwMemSize = MEM_SIZE(ptr);

			if (dwMemSize > 0) {
				mutex_autolock mutex(m_mutex);

				if (bFree == FALSE && IsStackMemory(m_pStackAllocator, ptr)) {
					bFree = STACK_Free(m_pStackAllocator, ptr);
				}

				if (bFree == FALSE) {
					bFree = POOL_Free(m_pHeapAllocator, m_pPoolAllocator, ptr);
				}

				if (bFree == FALSE) {
					bFree = HEAP_Free(m_pHeapAllocator, ptr);
				}

				if (bFree == FALSE) {
					_free((uint8_t *)ptr - 16);
				}

				m_dwPointerCount--;
				m_dwAllocatedSize -= dwMemSize;
			}
		}
	}

	uint32_t CMemAllocator::GetMemSize(void *ptr) const
	{
		return MEM_SIZE(ptr);
	}

	uint32_t CMemAllocator::GetPointerCount(void)
	{
		mutex_autolock mutex(m_mutex);
		return m_dwPointerCount;
	}

	uint32_t CMemAllocator::GetAllocatedSize(void)
	{
		mutex_autolock mutex(m_mutex);
		return m_dwAllocatedSize;
	}

	uint32_t CMemAllocator::GetMaxPointerCount(void)
	{
		mutex_autolock mutex(m_mutex);
		return m_dwMaxPointerCount;
	}

	uint32_t CMemAllocator::GetMaxAllocatedSize(void)
	{
		mutex_autolock mutex(m_mutex);
		return m_dwMaxAllocatedSize;
	}


	static CMemAllocator memAllocator(TRUE, TRUE, TRUE);

	CROSS_EXPORT void* Alloc(size_t size, MEMTYPE memType)
	{
		return memAllocator.Alloc(size, memType);
	}

	CROSS_EXPORT void Free(void *ptr)
	{
		memAllocator.Free(ptr);
	}

	CROSS_EXPORT void** AllocMatrix(size_t nx, size_t ny, size_t nsize, MEMTYPE memType)
	{
		ASSERT(nx);
		ASSERT(ny);
		ASSERT(nsize);

		uint32_t dwHeaderSize = (uint32_t)(nx*POINTER_SIZE);
		uint32_t dwBufferSize = (uint32_t)(dwHeaderSize + nx*ny*nsize);

		uint8_t *pBuffer = (uint8_t *)SAFE_ALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		uint8_t **ppLine = (uint8_t **)pBuffer;
		uint8_t *pBody = pBuffer + dwHeaderSize;

		for (uint32_t x = 0; x < nx; x++) {
			*ppLine = pBody; pBody += ny*nsize; ppLine++;
		}

		return (void **)pBuffer;
	}

	CROSS_EXPORT void FreeMatrix(void **ptr)
	{
		SAFE_FREE(ptr);
	}

	CROSS_EXPORT void*** AllocVolume(size_t nx, size_t ny, size_t nz, size_t nsize, MEMTYPE memType)
	{
		ASSERT(nx);
		ASSERT(ny);
		ASSERT(nz);
		ASSERT(nsize);

		uint32_t dwHeaderSize = (uint32_t)(nx*POINTER_SIZE + nx*ny*POINTER_SIZE);
		uint32_t dwBufferSize = (uint32_t)(dwHeaderSize + nx*ny*nz*nsize);

		uint8_t *pBuffer = (uint8_t *)SAFE_ALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		uint8_t ***pppLine = (uint8_t ***)pBuffer;
		uint8_t **ppLine = (uint8_t **)(pBuffer + nx*POINTER_SIZE);
		uint8_t *pBody = pBuffer + dwHeaderSize;

		for (uint32_t x = 0; x < nx; x++) {
			*pppLine = ppLine;

			for (uint32_t y = 0; y < ny; y++) {
				*ppLine = pBody; pBody += nz*nsize; ppLine++;
			}

			pppLine++;
		}

		return (void ***)pBuffer;
	}

	CROSS_EXPORT void FreeVolume(void ***ptr)
	{
		SAFE_FREE(ptr);
	}

	CROSS_EXPORT uint32_t GetPointerCount(void)
	{
		return memAllocator.GetPointerCount();
	}

	CROSS_EXPORT uint32_t GetAllocatedSize(void)
	{
		return memAllocator.GetAllocatedSize();
	}

	CROSS_EXPORT uint32_t GetMaxPointerCount(void)
	{
		return memAllocator.GetMaxPointerCount();
	}

	CROSS_EXPORT uint32_t GetMaxAllocatedSize(void)
	{
		return memAllocator.GetMaxAllocatedSize();
	}

}
