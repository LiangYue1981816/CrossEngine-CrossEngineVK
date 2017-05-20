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

	CVulkanQueue::CVulkanQueue(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkQueue(VK_NULL_HANDLE)
		, m_queueFamilyIndex(UINT32_MAX)
	{

	}

	CVulkanQueue::~CVulkanQueue(void)
	{

	}

	BOOL CVulkanQueue::Create(uint32_t queueFamilyIndex)
	{
		m_queueFamilyIndex = queueFamilyIndex;
		vkGetDeviceQueue(m_pDevice->GetDevice(), m_queueFamilyIndex, 0, &m_vkQueue);

		return TRUE;
	}

	void CVulkanQueue::Destroy(void)
	{
		m_vkQueue = VK_NULL_HANDLE;
	}

	VkQueue CVulkanQueue::GetQueue(void) const
	{
		return m_vkQueue;
	}

	uint32_t CVulkanQueue::GetQueueFamilyIndex(void) const
	{
		return m_queueFamilyIndex;
	}

	VkResult CVulkanQueue::Submit(CVulkanCommandBuffer *pCommandBuffer, CVulkanSemaphore *pWaitSemaphore, VkPipelineStageFlags waitStageFlags, CVulkanSemaphore *pSignalSemaphore) const
	{
		VkFence vkFence = pCommandBuffer->GetFence()->GetFence();
		VkCommandBuffer vkCommandBuffer = pCommandBuffer->GetCommandBuffer();
		VkSemaphore vkWaitSemaphore = pWaitSemaphore ? pWaitSemaphore->GetSemaphore() : VK_NULL_HANDLE;
		VkSemaphore vkSignalSemaphore = pSignalSemaphore ? pSignalSemaphore->GetSemaphore() : VK_NULL_HANDLE;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = NULL;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkCommandBuffer;
		submitInfo.waitSemaphoreCount = pWaitSemaphore ? 1 : 0;
		submitInfo.pWaitSemaphores = pWaitSemaphore ? &vkWaitSemaphore : NULL;
		submitInfo.pWaitDstStageMask = pWaitSemaphore ? &waitStageFlags : NULL;
		submitInfo.signalSemaphoreCount = pSignalSemaphore ? 1 : 0;
		submitInfo.pSignalSemaphores = pSignalSemaphore ? &vkSignalSemaphore : NULL;
		return vkQueueSubmit(m_vkQueue, 1, &submitInfo, vkFence);
	}

	VkResult CVulkanQueue::WaitIdle(void) const
	{
		return vkQueueWaitIdle(m_vkQueue);
	}

}
