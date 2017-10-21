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

	static const uint32_t STACK_POOL_SIZE = 8 * 1024 * 1024;

	struct STACK_ALLOCATOR {
		uint8_t pAddress[STACK_POOL_SIZE];

		uint32_t *pPointer;
		uint32_t dwFullSize;
		uint32_t dwFreeSize;
	};

	STACK_ALLOCATOR* STACK_Create(void)
	{
		STACK_ALLOCATOR *pStackAllocator = (STACK_ALLOCATOR *)_malloc(sizeof(STACK_ALLOCATOR));
		{
			pStackAllocator->pPointer = (uint32_t *)pStackAllocator->pAddress;
			pStackAllocator->dwFullSize = sizeof(pStackAllocator->pAddress);
			pStackAllocator->dwFreeSize = sizeof(pStackAllocator->pAddress);
		}
		return pStackAllocator;
	}

	void STACK_Destroy(STACK_ALLOCATOR *pStackAllocator)
	{
		_free(pStackAllocator);
	}

	void* STACK_Alloc(STACK_ALLOCATOR *pStackAllocator, size_t size)
	{
		uint32_t *pPointer = NULL;

		if (pStackAllocator) {
			const uint32_t dwMemSize = (uint32_t)ALIGN_16BYTE(size);

			if (((uint8_t *)pStackAllocator->pPointer + dwMemSize + 16) <= ((uint8_t *)pStackAllocator->pAddress + pStackAllocator->dwFullSize)) {
				pPointer = pStackAllocator->pPointer;

				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = (uint32_t)dwMemSize;

				pStackAllocator->pPointer = (uint32_t *)((uint8_t *)pStackAllocator->pPointer + dwMemSize + 16);
				pStackAllocator->dwFreeSize = (uint32_t)((uint8_t *)pStackAllocator->pAddress + pStackAllocator->dwFullSize - (uint8_t *)pStackAllocator->pPointer);
			}
		}

		return pPointer;
	}

	BOOL STACK_Free(STACK_ALLOCATOR *pStackAllocator, void *ptr)
	{
		if (pStackAllocator) {
			const uint32_t dwMemSize = MEM_SIZE(ptr);

			if (((uint8_t *)pStackAllocator->pPointer - dwMemSize - 16) >= (uint8_t *)pStackAllocator->pAddress) {
				pStackAllocator->pPointer = (uint32_t *)((uint8_t *)pStackAllocator->pPointer - dwMemSize - 16);
				pStackAllocator->dwFreeSize = (uint32_t)((uint8_t *)pStackAllocator->pAddress + pStackAllocator->dwFullSize - (uint8_t *)pStackAllocator->pPointer);
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL IsStackMemory(STACK_ALLOCATOR *pStackAllocator, void *pPointer)
	{
		return pStackAllocator && (uint8_t *)pPointer >= (uint8_t *)pStackAllocator->pAddress && (uint8_t *)pPointer < (uint8_t *)pStackAllocator->pPointer ? TRUE : FALSE;
	}

}
