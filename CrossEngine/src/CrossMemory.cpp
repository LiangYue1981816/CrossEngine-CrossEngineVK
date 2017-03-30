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

	DWORD CMemAllocator::dwAllocatorCount = 0;

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

		if (bHeap) {
			m_pHeapAllocator = HEAP_Create();
			m_pPoolAllocator = POOL_Create(m_pHeapAllocator);
		}

		if (bStack) {
			m_pStackAllocator = STACK_Create();
		}

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

		if (dwAllocatorCount == 0) {
			LOGI("Memory Leak Check ...\n");
			dump_memory_objects();
		}
	}

	void* CMemAllocator::Alloc(size_t size, MEMTYPE memType)
	{
		DWORD *pPointer = NULL;
		const DWORD dwMemSize = (DWORD)ALIGN_16BYTE(size);

		if (dwMemSize > 0) {
			if (m_mutex) pthread_mutex_lock(&m_mutex);
			{
				if (pPointer == NULL && memType == MEMTYPE_STACK) {
					pPointer = (DWORD *)STACK_Alloc(m_pStackAllocator, dwMemSize);
				}

				if (pPointer == NULL) {
					pPointer = (DWORD *)POOL_Alloc(m_pHeapAllocator, m_pPoolAllocator, dwMemSize);
				}

				if (pPointer == NULL) {
					pPointer = (DWORD *)HEAP_Alloc(m_pHeapAllocator, dwMemSize);
				}

				if (pPointer == NULL) {
					pPointer = (DWORD *)_malloc(dwMemSize + 16);

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
			if (m_mutex) pthread_mutex_unlock(&m_mutex);
		}

		return pPointer;
	}

	void CMemAllocator::Free(void *ptr)
	{
		if (ptr) {
			BOOL bFree = FALSE;
			const DWORD dwMemSize = MEM_SIZE(ptr);

			if (dwMemSize > 0) {
				if (m_mutex) pthread_mutex_lock(&m_mutex);
				{
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
						_free((BYTE *)ptr - 16);
					}

					m_dwPointerCount--;
					m_dwAllocatedSize -= dwMemSize;
				}
				if (m_mutex) pthread_mutex_unlock(&m_mutex);
			}
		}
	}

	DWORD CMemAllocator::GetMemSize(void *ptr) const
	{
		return MEM_SIZE(ptr);
	}

	DWORD CMemAllocator::GetPointerCount(void)
	{
		DWORD dwPointerCount;
		if (m_mutex) pthread_mutex_lock(&m_mutex);
		{
			dwPointerCount = m_dwPointerCount;
		}
		if (m_mutex) pthread_mutex_unlock(&m_mutex);
		return dwPointerCount;
	}

	DWORD CMemAllocator::GetAllocatedSize(void)
	{
		DWORD dwAllocatedSize;
		if (m_mutex) pthread_mutex_lock(&m_mutex);
		{
			dwAllocatedSize = m_dwAllocatedSize;
		}
		if (m_mutex) pthread_mutex_unlock(&m_mutex);
		return dwAllocatedSize;
	}

	DWORD CMemAllocator::GetMaxPointerCount(void)
	{
		DWORD dwMaxPointerCount;
		if (m_mutex) pthread_mutex_lock(&m_mutex);
		{
			dwMaxPointerCount = m_dwMaxPointerCount;
		}
		if (m_mutex) pthread_mutex_unlock(&m_mutex);
		return dwMaxPointerCount;
	}

	DWORD CMemAllocator::GetMaxAllocatedSize(void)
	{
		DWORD dwMaxAllocatedSize;
		if (m_mutex) pthread_mutex_lock(&m_mutex);
		{
			dwMaxAllocatedSize = m_dwMaxAllocatedSize;
		}
		if (m_mutex) pthread_mutex_unlock(&m_mutex);
		return dwMaxAllocatedSize;
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

		DWORD dwHeaderSize = (DWORD)(nx*POINTER_SIZE);
		DWORD dwBufferSize = (DWORD)(dwHeaderSize + nx*ny*nsize);

		BYTE *pBuffer = (BYTE *)SAFE_ALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		BYTE **ppLine = (BYTE **)pBuffer;
		BYTE *pBody = pBuffer + dwHeaderSize;

		for (DWORD x = 0; x < nx; x++) {
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

		DWORD dwHeaderSize = (DWORD)(nx*POINTER_SIZE + nx*ny*POINTER_SIZE);
		DWORD dwBufferSize = (DWORD)(dwHeaderSize + nx*ny*nz*nsize);

		BYTE *pBuffer = (BYTE *)SAFE_ALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		BYTE ***pppLine = (BYTE ***)pBuffer;
		BYTE **ppLine = (BYTE **)(pBuffer + nx*POINTER_SIZE);
		BYTE *pBody = pBuffer + dwHeaderSize;

		for (DWORD x = 0; x < nx; x++) {
			*pppLine = ppLine;

			for (DWORD y = 0; y < ny; y++) {
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

	CROSS_EXPORT DWORD GetPointerCount(void)
	{
		return memAllocator.GetPointerCount();
	}

	CROSS_EXPORT DWORD GetAllocatedSize(void)
	{
		return memAllocator.GetAllocatedSize();
	}

	CROSS_EXPORT DWORD GetMaxPointerCount(void)
	{
		return memAllocator.GetMaxPointerCount();
	}

	CROSS_EXPORT DWORD GetMaxAllocatedSize(void)
	{
		return memAllocator.GetMaxAllocatedSize();
	}

}
