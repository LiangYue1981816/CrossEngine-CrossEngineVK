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
#include "debug_new.h"


namespace CrossEngine {

	void* _malloc(size_t size);
	void  _free(void *ptr);


	struct STACK_ALLOCATOR;
	STACK_ALLOCATOR* STACK_Create(void);
	void  STACK_Destroy(STACK_ALLOCATOR *pStackAllocator);
	void* STACK_Alloc(STACK_ALLOCATOR *pStackAllocator, size_t size);
	BOOL  STACK_Free(STACK_ALLOCATOR *pStackAllocator, void *ptr);
	BOOL  IsStackMemory(STACK_ALLOCATOR *pStackAllocator, void *pPointer);


	struct HEAP_ALLOCATOR;
	HEAP_ALLOCATOR* HEAP_Create(void);
	void  HEAP_Destroy(HEAP_ALLOCATOR *pHeapAllocator);
	void* HEAP_Alloc(HEAP_ALLOCATOR *pHeapAllocator, size_t size);
	BOOL  HEAP_Free(HEAP_ALLOCATOR *pHeapAllocator, void *pPointer);


	struct POOL_ALLOCATOR;
	POOL_ALLOCATOR* POOL_Create(HEAP_ALLOCATOR *pHeapAllocator);
	void  POOL_Destroy(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator);
	void* POOL_Alloc(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, size_t size);
	BOOL  POOL_Free(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, void *pPointer);

}
