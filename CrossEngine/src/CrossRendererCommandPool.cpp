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

	CRendererCommandPool::CRendererCommandPool(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkCommandPool(VK_NULL_HANDLE)

		, m_pFreeListHead{ NULL }
		, m_pActiveListHead{ NULL }
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = m_pDevice->GetQueue()->GetQueueFamilyIndex();
		vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool);
	}

	CRendererCommandPool::~CRendererCommandPool(void)
	{
		for (uint32_t level = 0; level < VK_COMMAND_BUFFER_LEVEL_RANGE_SIZE; level++) {
			if (CRendererCommandBuffer *pCommandBuffer = m_pFreeListHead[level]) {
				CRendererCommandBuffer *pCommandBufferNext = NULL;
				do {
					pCommandBufferNext = pCommandBuffer->pFreeNext;
					SAFE_DELETE(pCommandBuffer);
				} while (pCommandBuffer = pCommandBufferNext);
			}

			if (CRendererCommandBuffer *pCommandBuffer = m_pActiveListHead[level]) {
				CRendererCommandBuffer *pCommandBufferNext = NULL;
				do {
					pCommandBufferNext = pCommandBuffer->pActiveNext;
					SAFE_DELETE(pCommandBuffer);
				} while (pCommandBuffer = pCommandBufferNext);
			}
		}

		vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
	}

	CRendererCommandBuffer* CRendererCommandPool::AllocCommandBuffer(VkCommandBufferLevel level)
	{
		CRendererCommandBuffer *pCommandBuffer = NULL;

		try {
			if (m_pFreeListHead[level] == NULL) {
				VkCommandBuffer vkCommandBuffer;
				VkCommandBufferAllocateInfo commandBufferInfo = {};
				commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				commandBufferInfo.pNext = NULL;
				commandBufferInfo.commandPool = m_vkCommandPool;
				commandBufferInfo.level = level;
				commandBufferInfo.commandBufferCount = 1;
				CALL_VK_FUNCTION_THROW(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferInfo, &vkCommandBuffer));

				m_pFreeListHead[level] = SAFE_NEW CRendererCommandBuffer(m_pDevice, vkCommandBuffer, level);
			}

			pCommandBuffer = m_pFreeListHead[level];
			m_pFreeListHead[level] = pCommandBuffer->pFreeNext;

			pCommandBuffer->pActiveNext = m_pActiveListHead[level];
			pCommandBuffer->pActivePrev = NULL;

			if (m_pActiveListHead[level]) {
				m_pActiveListHead[level]->pActivePrev = pCommandBuffer;
			}

			m_pActiveListHead[level] = pCommandBuffer;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
		}

		return pCommandBuffer;
	}

	void CRendererCommandPool::FreeCommandBuffer(CRendererCommandBuffer *pCommandBuffer)
	{
		VkCommandBufferLevel level = pCommandBuffer->GetCommandBufferLevel();

		pCommandBuffer->pFreeNext = m_pFreeListHead[level];
		m_pFreeListHead[level] = pCommandBuffer;

		if (m_pActiveListHead[level] = pCommandBuffer) {
			m_pActiveListHead[level] = pCommandBuffer->pActiveNext;
		}

		if (pCommandBuffer->pActivePrev) {
			pCommandBuffer->pActivePrev->pActiveNext = pCommandBuffer->pActiveNext;
		}

		if (pCommandBuffer->pActiveNext) {
			pCommandBuffer->pActiveNext->pActivePrev = pCommandBuffer->pActivePrev;
		}
	}

	void CRendererCommandPool::ResetCommandPool(BOOL bReleaseResources) const
	{
		vkResetCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, bReleaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0);
	}

}
