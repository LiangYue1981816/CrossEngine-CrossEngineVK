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

	#define GET_BLOCK_POOL(pBlock) (BLOCK_POOL *)((uint8_t *)(pBlock) - (pBlock)->dwOffset - ALIGN_16BYTE(sizeof(BLOCK_POOL)))
	#define GET_BLOCK_NEXT(pBlock) (BLOCK *)((uint8_t *)GET_BLOCK_POOL(pBlock) + (pBlock)->dwOffsetNext + ALIGN_16BYTE(sizeof(BLOCK_POOL)))

	static const uint32_t BLOCK_POOL_COUNT = 4 * 1024 / 16;
	static const uint32_t BLOCK_POOL_SIZE = 64 * 1024;

	struct BLOCK;
	struct BLOCK_POOL;

	struct BLOCK {
		uint32_t dwOffset;
		uint32_t dwOffsetNext;
		uint32_t dwReserve;
		uint32_t dwAddress;
	};

	struct BLOCK_POOL {
		BLOCK *pBlockHead;

		BLOCK_POOL *pNext;
		BLOCK_POOL *pPrev;

		BLOCK_POOL *pFreeNext;
		BLOCK_POOL *pFreePrev;

		uint32_t dwBlockIndex;
		uint32_t dwBlockCount;
	};

	struct BLOCK_POOL_HEAD {
		BLOCK_POOL *pBlockPoolHead;
		BLOCK_POOL *pBlockPoolFreeHead;
	};

	struct POOL_ALLOCATOR {
		BLOCK_POOL_HEAD pools[BLOCK_POOL_COUNT];
	};

	
	static BLOCK_POOL* POOL_CreatePool(HEAP_ALLOCATOR *pHeapAllocator, uint32_t dwMemSize)
	{
		const uint32_t dwBlockSize = ALIGN_16BYTE(sizeof(BLOCK)) + ALIGN_16BYTE(dwMemSize);
		const uint32_t dwBlockCount = (BLOCK_POOL_SIZE - ALIGN_16BYTE(sizeof(BLOCK_POOL))) / ALIGN_16BYTE(dwBlockSize);

		BLOCK_POOL *pBlockPool = (BLOCK_POOL *)HEAP_Alloc(pHeapAllocator, BLOCK_POOL_SIZE);
		{
			pBlockPool->pBlockHead = (BLOCK *)((uint8_t *)pBlockPool + ALIGN_16BYTE(sizeof(BLOCK_POOL)));
			pBlockPool->pNext = NULL;
			pBlockPool->pPrev = NULL;
			pBlockPool->pFreeNext = NULL;
			pBlockPool->pFreePrev = NULL;
			pBlockPool->dwBlockIndex = 0;
			pBlockPool->dwBlockCount = dwBlockCount;
		}

		BLOCK *pBlock = pBlockPool->pBlockHead;
		{
			for (uint32_t indexBlock = 0; indexBlock < dwBlockCount; indexBlock++) {
				pBlock->dwOffset = indexBlock * dwBlockSize;
				pBlock->dwOffsetNext = pBlock->dwOffset + dwBlockSize;
				pBlock = GET_BLOCK_NEXT(pBlock);
			}
		}

		return pBlockPool;
	}

	static void POOL_DestroyPool(HEAP_ALLOCATOR *pHeapAllocator, BLOCK_POOL *pBlockPool)
	{
		HEAP_Free(pHeapAllocator, pBlockPool);
	}

	POOL_ALLOCATOR* POOL_Create(HEAP_ALLOCATOR *pHeapAllocator)
	{
		POOL_ALLOCATOR *pPoolAllocator = (POOL_ALLOCATOR *)HEAP_Alloc(pHeapAllocator, sizeof(POOL_ALLOCATOR));

		for (uint32_t indexPool = 0; indexPool < BLOCK_POOL_COUNT; indexPool++) {
			pPoolAllocator->pools[indexPool].pBlockPoolHead = NULL;
			pPoolAllocator->pools[indexPool].pBlockPoolFreeHead = NULL;
		}

		return pPoolAllocator;
	}

	void POOL_Destroy(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator)
	{
		for (uint32_t indexPool = 0; indexPool < BLOCK_POOL_COUNT; indexPool++) {
			if (BLOCK_POOL *pBlockPool = pPoolAllocator->pools[indexPool].pBlockPoolHead) {
				BLOCK_POOL *pBlockPoolNext = NULL;

				do {
					pBlockPoolNext = pBlockPool->pNext;
					POOL_DestroyPool(pHeapAllocator, pBlockPool);
				} while (pBlockPool = pBlockPoolNext);
			}
		}

		HEAP_Free(pHeapAllocator, pPoolAllocator);
	}

	void* POOL_Alloc(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, size_t size)
	{
		uint32_t *pPointer = NULL;
		
		if (pHeapAllocator && pPoolAllocator) {
			const uint32_t dwMemSize = (uint32_t)ALIGN_16BYTE(size);
			const uint32_t dwIndexPool = dwMemSize / 16;

			if (dwIndexPool < BLOCK_POOL_COUNT) {
				BLOCK_POOL_HEAD *pPoolHead = &pPoolAllocator->pools[dwIndexPool];

				if (pPoolHead->pBlockPoolFreeHead == NULL) {
					pPoolHead->pBlockPoolFreeHead = POOL_CreatePool(pHeapAllocator, dwMemSize);

					if (pPoolHead->pBlockPoolHead) {
						pPoolHead->pBlockPoolHead->pPrev = pPoolHead->pBlockPoolFreeHead;
						pPoolHead->pBlockPoolFreeHead->pNext = pPoolHead->pBlockPoolHead;
					}

					pPoolHead->pBlockPoolHead = pPoolHead->pBlockPoolFreeHead;
				}

				pPointer = &pPoolHead->pBlockPoolFreeHead->pBlockHead->dwAddress;
				pPoolHead->pBlockPoolFreeHead->pBlockHead = GET_BLOCK_NEXT(pPoolHead->pBlockPoolFreeHead->pBlockHead);
				pPoolHead->pBlockPoolFreeHead->dwBlockIndex++;

				if (pPoolHead->pBlockPoolFreeHead->dwBlockIndex == pPoolHead->pBlockPoolFreeHead->dwBlockCount) {
					pPoolHead->pBlockPoolFreeHead = pPoolHead->pBlockPoolFreeHead->pFreeNext;
				}

				*pPointer++ = dwMemSize;
			}
		}

		return pPointer;
	}

	BOOL POOL_Free(HEAP_ALLOCATOR *pHeapAllocator, POOL_ALLOCATOR *pPoolAllocator, void *pPointer)
	{
		if (pHeapAllocator && pPoolAllocator) {
			const uint32_t dwMemSize = MEM_SIZE(pPointer);
			const uint32_t dwIndexPool = dwMemSize / 16;

			if (dwIndexPool < BLOCK_POOL_COUNT) {
				BLOCK *pBlock = (BLOCK *)((uint8_t *)pPointer - ALIGN_16BYTE(sizeof(BLOCK)));
				BLOCK_POOL *pBlockPool = GET_BLOCK_POOL(pBlock);
				BLOCK_POOL_HEAD *pPoolHead = &pPoolAllocator->pools[dwIndexPool];

				if (pBlockPool->dwBlockIndex == pBlockPool->dwBlockCount) {
					pBlockPool->pFreeNext = NULL;
					pBlockPool->pFreePrev = NULL;

					if (pPoolHead->pBlockPoolFreeHead) {
						pPoolHead->pBlockPoolFreeHead->pFreePrev = pBlockPool;
						pBlockPool->pFreeNext = pPoolHead->pBlockPoolFreeHead;
					}

					pPoolHead->pBlockPoolFreeHead = pBlockPool;
				}
				else {
					pBlock->dwOffsetNext = pBlockPool->pBlockHead->dwOffset;
				}

				pBlockPool->dwBlockIndex--;

				if (pBlockPool->dwBlockIndex == 0) {
					if (pBlockPool->pPrev) pBlockPool->pPrev->pNext = pBlockPool->pNext;
					if (pBlockPool->pNext) pBlockPool->pNext->pPrev = pBlockPool->pPrev;
					if (pPoolHead->pBlockPoolHead == pBlockPool) pPoolHead->pBlockPoolHead = pBlockPool->pNext;

					if (pBlockPool->pFreePrev) pBlockPool->pFreePrev->pFreeNext = pBlockPool->pFreeNext;
					if (pBlockPool->pFreeNext) pBlockPool->pFreeNext->pFreePrev = pBlockPool->pFreePrev;
					if (pPoolHead->pBlockPoolFreeHead == pBlockPool) pPoolHead->pBlockPoolFreeHead = pBlockPool->pFreeNext;

					POOL_DestroyPool(pHeapAllocator, pBlockPool);
				}
				else {
					pBlockPool->pBlockHead = pBlock;
				}

				return TRUE;
			}
		}

		return FALSE;
	}

}
