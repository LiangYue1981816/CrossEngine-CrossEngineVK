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

	#define NODE_INDEX(size) (((size) / BLOCK_UNIT_SIZE) - 1)

	static const uint32_t BLOCK_POOL_SIZE = 4 * 1024 * 1024;
	static const uint32_t BLOCK_UNIT_SIZE = 4 * 1024;

	struct BLOCK;
	struct BLOCK_POOL;

	struct BLOCK {
		BLOCK_POOL *pPool;

		BLOCK *pNext;
		BLOCK *pPrev;

		BLOCK *pFreeNext;
		BLOCK *pFreePrev;

		uint32_t dwSize;
		uint32_t dwInUse;

#ifdef __LP64__
		uint32_t dwReserve0;
		uint32_t dwReserve1;
		uint32_t dwReserve2;
#endif
		uint32_t dwAddress;
	};

	struct BLOCK_NODE {
		rb_node node;

		uint32_t dwSize;
		BLOCK *pBlockHead;
	};

	struct BLOCK_POOL {
		rb_root root;
		BLOCK_NODE *nodes;

		BLOCK_POOL *pNext;
		BLOCK_POOL *pPrev;

		BLOCK *pBlockHead;
	};

	struct HEAP_ALLOCATOR {
		BLOCK_POOL *pBlockPoolHead;
	};

	
	static void HEAP_InitNodes(BLOCK_POOL *pBlockPool, uint32_t dwNodeCount)
	{
		pBlockPool->root = RB_ROOT;

		for (uint32_t indexNode = 0; indexNode < dwNodeCount; indexNode++) {
			pBlockPool->nodes[indexNode].dwSize = (indexNode + 1) * BLOCK_UNIT_SIZE;
			pBlockPool->nodes[indexNode].pBlockHead = NULL;
		}
	}

	static BLOCK* HEAP_SearchBlock(BLOCK_POOL *pBlockPool, uint32_t dwSize)
	{
		BLOCK_NODE *pBlockNode = NULL;
		rb_node *node = pBlockPool->root.rb_node;

		while (node) {
			BLOCK_NODE *pBlockNodeCur = container_of(node, BLOCK_NODE, node);

			if (dwSize > pBlockNodeCur->dwSize) {
				node = node->rb_right;
				continue;
			}

			pBlockNode = pBlockNodeCur;

			if (dwSize < pBlockNodeCur->dwSize) {
				node = node->rb_left;
				continue;
			}

			ASSERT(pBlockNode->pBlockHead);
			ASSERT(pBlockNode->pBlockHead->dwInUse == FALSE);
			ASSERT(pBlockNode->pBlockHead->dwSize / BLOCK_UNIT_SIZE * BLOCK_UNIT_SIZE >= dwSize);

			break;
		}
		
		return pBlockNode ? pBlockNode->pBlockHead : NULL;
	}

	static void HEAP_InsertBlock(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
	{
		ASSERT(pBlock->dwInUse == FALSE);

		BLOCK_NODE *pBlockNode = &pBlockPool->nodes[NODE_INDEX(pBlock->dwSize)];
		rb_node **node = &(pBlockPool->root.rb_node);
		rb_node *parent = NULL;

		while (*node) {
			BLOCK_NODE *pBlockNodeCur = container_of(*node, BLOCK_NODE, node);

			parent = *node;

			if (pBlockNode->dwSize > pBlockNodeCur->dwSize) {
				node = &(*node)->rb_right;
				continue;
			}

			if (pBlockNode->dwSize < pBlockNodeCur->dwSize) {
				node = &(*node)->rb_left;
				continue;
			}

			ASSERT(pBlockNode == pBlockNodeCur);

			pBlock->pFreePrev = NULL;
			pBlock->pFreeNext = pBlockNode->pBlockHead;
			pBlockNode->pBlockHead->pFreePrev = pBlock;
			pBlockNode->pBlockHead = pBlock;

			return;
		}

		pBlock->pFreeNext = NULL;
		pBlock->pFreePrev = NULL;
		pBlockNode->pBlockHead = pBlock;

		rb_init_node(&pBlockNode->node);
		rb_link_node(&pBlockNode->node, parent, node);
		rb_insert_color(&pBlockNode->node, &pBlockPool->root);
	}

	static void HEAP_RemoveBlock(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
	{
		ASSERT(pBlock->dwInUse == FALSE);
		BLOCK_NODE *pBlockNode = &pBlockPool->nodes[NODE_INDEX(pBlock->dwSize)];

		if (pBlock->pFreeNext) {
			pBlock->pFreeNext->pFreePrev = pBlock->pFreePrev;
		}

		if (pBlock->pFreePrev) {
			pBlock->pFreePrev->pFreeNext = pBlock->pFreeNext;
		}

		if (pBlockNode->pBlockHead == pBlock) {
			pBlockNode->pBlockHead = pBlock->pFreeNext;

			if (pBlockNode->pBlockHead == NULL) {
				rb_erase(&pBlockNode->node, &pBlockPool->root);
			}
		}
	}

	static BLOCK_POOL* HEAP_CreatePool(uint32_t dwMemSize)
	{
		const uint32_t dwBlockPoolSize = ALIGN_16BYTE(max(dwMemSize, BLOCK_POOL_SIZE));
		const uint32_t dwNodeCount = dwBlockPoolSize / BLOCK_UNIT_SIZE;

		BLOCK_POOL *pBlockPool = (BLOCK_POOL *)_malloc(ALIGN_16BYTE(sizeof(BLOCK_POOL)) + ALIGN_16BYTE(sizeof(BLOCK)) + dwBlockPoolSize);
		{
			pBlockPool->pNext = NULL;
			pBlockPool->pPrev = NULL;

			pBlockPool->pBlockHead = (BLOCK *)((uint8_t *)pBlockPool + ALIGN_16BYTE(sizeof(BLOCK_POOL)));
			pBlockPool->pBlockHead->pPool = pBlockPool;
			pBlockPool->pBlockHead->pNext = NULL;
			pBlockPool->pBlockHead->pPrev = NULL;
			pBlockPool->pBlockHead->dwSize = dwBlockPoolSize;
			pBlockPool->pBlockHead->dwInUse = FALSE;

			pBlockPool->nodes = (BLOCK_NODE *)_malloc(sizeof(BLOCK_NODE) * dwNodeCount);
			{
				HEAP_InitNodes(pBlockPool, dwNodeCount);
				HEAP_InsertBlock(pBlockPool, pBlockPool->pBlockHead);
			}
		}

		return pBlockPool;
	}

	static void HEAP_DestroyPool(BLOCK_POOL *pBlockPool)
	{
		_free(pBlockPool->nodes);
		_free(pBlockPool);
	}

	static void* HEAP_PoolAlloc(BLOCK_POOL *pBlockPool, uint32_t dwMemSize)
	{
		uint32_t *pPointer = NULL;

		if (BLOCK *pBlock = HEAP_SearchBlock(pBlockPool, dwMemSize)) {
			HEAP_RemoveBlock(pBlockPool, pBlock);

			pBlock->dwInUse = TRUE;

			if (pBlock->dwSize >= dwMemSize + ALIGN_16BYTE(sizeof(BLOCK)) + BLOCK_UNIT_SIZE) {
				BLOCK *pBlockNext = (BLOCK *)((uint8_t *)pBlock + dwMemSize + ALIGN_16BYTE(sizeof(BLOCK)));
				pBlockNext->pPool = pBlock->pPool;
				pBlockNext->dwSize = pBlock->dwSize - dwMemSize - ALIGN_16BYTE(sizeof(BLOCK));
				pBlockNext->dwInUse = FALSE;

				pBlockNext->pNext = pBlock->pNext;
				pBlockNext->pPrev = pBlock;
				pBlock->pNext = pBlockNext;

				if (pBlockNext->pNext) {
					pBlockNext->pNext->pPrev = pBlockNext;
				}

				pBlock->dwSize = dwMemSize;
				HEAP_InsertBlock(pBlockPool, pBlockNext);
			}

			pPointer = &pBlock->dwAddress; *pPointer++ = dwMemSize;
		}

		return pPointer;
	}

	static BLOCK* HEAP_Merge(BLOCK_POOL *pBlockPool, BLOCK *pBlock, BLOCK *pBlockNext)
	{
		ASSERT((uint8_t *)pBlock + pBlock->dwSize + ALIGN_16BYTE(sizeof(BLOCK)) == (uint8_t *)pBlockNext);

		HEAP_RemoveBlock(pBlockPool, pBlockNext);

		pBlock->dwSize = pBlock->dwSize + pBlockNext->dwSize + ALIGN_16BYTE(sizeof(BLOCK));
		pBlock->pNext = pBlockNext->pNext;

		if (pBlockNext->pNext) {
			pBlockNext->pNext->pPrev = pBlock;
		}

		return pBlock;
	}

	static void HEAP_PoolFree(BLOCK_POOL *pBlockPool, BLOCK *pBlock)
	{
		pBlock->dwInUse = FALSE;

		if (pBlock->pNext && pBlock->pNext->dwInUse == FALSE) {
			pBlock = HEAP_Merge(pBlockPool, pBlock, pBlock->pNext);
		}

		if (pBlock->pPrev && pBlock->pPrev->dwInUse == FALSE) {
			pBlock = HEAP_Merge(pBlockPool, pBlock->pPrev, pBlock);
		}

		HEAP_InsertBlock(pBlockPool, pBlock);
	}

	HEAP_ALLOCATOR* HEAP_Create(void)
	{
		HEAP_ALLOCATOR *pHeapAllocator = (HEAP_ALLOCATOR *)_malloc(sizeof(HEAP_ALLOCATOR));
		pHeapAllocator->pBlockPoolHead = NULL;
		return pHeapAllocator;
	}

	void HEAP_Destroy(HEAP_ALLOCATOR *pHeapAllocator)
	{
		if (BLOCK_POOL *pBlockPool = pHeapAllocator->pBlockPoolHead) {
			BLOCK_POOL *pBlockPoolNext = NULL;

			do {
				pBlockPoolNext = pBlockPool->pNext;
				HEAP_DestroyPool(pBlockPool);
			} while (pBlockPool = pBlockPoolNext);
		}

		_free(pHeapAllocator);
	}

	void* HEAP_Alloc(HEAP_ALLOCATOR *pHeapAllocator, size_t size)
	{
		uint32_t *pPointer = NULL;
		
		if (pHeapAllocator) {
			const uint32_t dwMemSize = (uint32_t)ALIGN_4KBYTE(size);

			do {
				if (BLOCK_POOL *pBlockPool = pHeapAllocator->pBlockPoolHead) {
					do {
						if (pPointer = (uint32_t *)HEAP_PoolAlloc(pBlockPool, dwMemSize)) {
							return pPointer;
						}
					} while (pBlockPool = pBlockPool->pNext);
				}

				BLOCK_POOL *pBlockPool = HEAP_CreatePool(dwMemSize);

				if (pHeapAllocator->pBlockPoolHead) {
					pHeapAllocator->pBlockPoolHead->pPrev = pBlockPool;
					pBlockPool->pNext = pHeapAllocator->pBlockPoolHead;
				}

				pHeapAllocator->pBlockPoolHead = pBlockPool;
			} while (TRUE);
		}

		return pPointer;
	}

	BOOL HEAP_Free(HEAP_ALLOCATOR *pHeapAllocator, void *pPointer)
	{
		if (pHeapAllocator) {
			BLOCK *pBlock = (BLOCK *)((uint8_t *)pPointer - ALIGN_16BYTE(sizeof(BLOCK)));
			BLOCK_POOL *pBlockPool = pBlock->pPool;

			HEAP_PoolFree(pBlockPool, pBlock);
			/*
			if (pBlockPool->dwSize == pBlockPool->pBlockHead->dwSize) {
				ASSERT(pBlockPool->pBlockHead->pNext == NULL);
				ASSERT(pBlockPool->pBlockHead->pPrev == NULL);

				if (pHeapAllocator->pBlockPoolHead == pBlockPool) {
					pHeapAllocator->pBlockPoolHead =  pBlockPool->pNext;
				}

				if (pBlockPool->pPrev) {
					pBlockPool->pPrev->pNext = pBlockPool->pNext;
				}

				if (pBlockPool->pNext) {
					pBlockPool->pNext->pPrev = pBlockPool->pPrev;
				}

				HEAP_DestroyPool(pBlockPool);
			}
			*/
			return TRUE;
		}

		return FALSE;
	}

}
