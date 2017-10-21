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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	enum MEMTYPE { MEMTYPE_HEAP = 0, MEMTYPE_STACK };

	struct POOL_ALLOCATOR;
	struct HEAP_ALLOCATOR;
	struct STACK_ALLOCATOR;

	class CROSS_EXPORT CMemAllocator
	{
	public:
		CMemAllocator(BOOL bHeap, BOOL bStack, BOOL bMultiThread);
		virtual ~CMemAllocator(void);


	public:
		void* Alloc(size_t size, MEMTYPE memType);
		void Free(void *ptr);

	public:
		uint32_t GetMemSize(void *ptr) const;
		uint32_t GetPointerCount(void);
		uint32_t GetAllocatedSize(void);
		uint32_t GetMaxPointerCount(void);
		uint32_t GetMaxAllocatedSize(void);


	protected:
		pthread_mutex_t m_mutex;
		POOL_ALLOCATOR *m_pPoolAllocator;
		HEAP_ALLOCATOR *m_pHeapAllocator;
		STACK_ALLOCATOR *m_pStackAllocator;

	protected:
		uint32_t m_dwPointerCount;
		uint32_t m_dwAllocatedSize;

		uint32_t m_dwMaxPointerCount;
		uint32_t m_dwMaxAllocatedSize;

	protected:
		static uint32_t dwAllocatorCount;
	};

	CROSS_EXPORT void* Alloc(size_t size, MEMTYPE memType);
	CROSS_EXPORT void  Free(void *ptr);

	CROSS_EXPORT void** AllocMatrix(size_t nx, size_t ny, size_t nsize, MEMTYPE memType);
	CROSS_EXPORT void   FreeMatrix(void **ptr);

	CROSS_EXPORT void*** AllocVolume(size_t nx, size_t ny, size_t nz, size_t nsize, MEMTYPE memType);
	CROSS_EXPORT void    FreeVolume(void ***ptr);

	CROSS_EXPORT uint32_t GetPointerCount(void);
	CROSS_EXPORT uint32_t GetAllocatedSize(void);

	CROSS_EXPORT uint32_t GetMaxPointerCount(void);
	CROSS_EXPORT uint32_t GetMaxAllocatedSize(void);

}


#define SAFE_ALLOC(s, t) CrossEngine::Alloc((s), (t))
#define SAFE_FREE(p) do { if ((p)) { CrossEngine::Free((p)); (p) = NULL; } } while (0);


#define SAFE_NEW new
#define SAFE_DELETE(p) do { if ((p)) { delete (p); (p) = NULL; } } while (0);
#define SAFE_DELETE_ARRAY(p) do { if ((p)) { delete [] (p); (p) = NULL; } } while (0);


#define MEM_SIZE(ptr) *((uint32_t *)(ptr) - 1)
