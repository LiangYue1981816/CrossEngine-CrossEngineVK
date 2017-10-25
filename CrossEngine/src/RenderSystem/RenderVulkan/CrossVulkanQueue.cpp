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

	int CVulkanQueue::Create(uint32_t queueFamilyIndex, uint32_t queueIndex)
	{
		m_queueFamilyIndex = queueFamilyIndex;
		vkGetDeviceQueue(m_pDevice->GetDevice(), m_queueFamilyIndex, queueIndex, &m_vkQueue);

		return VK_SUCCESS;
	}

	void CVulkanQueue::Destroy(void)
	{
		m_vkQueue = VK_NULL_HANDLE;
		m_queueFamilyIndex = UINT32_MAX;
	}

	VkQueue CVulkanQueue::GetQueue(void) const
	{
		return m_vkQueue;
	}

	uint32_t CVulkanQueue::GetQueueFamilyIndex(void) const
	{
		return m_queueFamilyIndex;
	}

	int CVulkanQueue::Submit(const CGfxCommandBuffer *pCommandBuffer) const
	{
		return Submit(pCommandBuffer, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);
	}

	int CVulkanQueue::Submit(const CGfxCommandBuffer *pCommandBuffer, CGfxSemaphore waitSemaphore, VkPipelineStageFlags waitStageFlags, CGfxSemaphore signalSemaphore) const
	{
		const CVulkanCommandBuffer *pVkCommandBuffer = (CVulkanCommandBuffer *)pCommandBuffer;
		pVkCommandBuffer->FenceWait(UINT32_MAX);
		pVkCommandBuffer->FenceReset();

		VkSemaphore vkWaitSemaphore = (VkSemaphore)waitSemaphore;
		VkSemaphore vkSignalSemaphore = (VkSemaphore)signalSemaphore;

		VkFence vkFence = (VkFence)pVkCommandBuffer->GetFence();
		VkCommandBuffer vkCommandBuffer = (VkCommandBuffer)pVkCommandBuffer->GetHandle();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = NULL;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkCommandBuffer;
		submitInfo.waitSemaphoreCount = vkWaitSemaphore != VK_NULL_HANDLE ? 1 : 0;
		submitInfo.pWaitSemaphores = vkWaitSemaphore != VK_NULL_HANDLE ? &vkWaitSemaphore : NULL;
		submitInfo.pWaitDstStageMask = vkWaitSemaphore != VK_NULL_HANDLE ? &waitStageFlags : NULL;
		submitInfo.signalSemaphoreCount = vkSignalSemaphore != VK_NULL_HANDLE ? 1 : 0;
		submitInfo.pSignalSemaphores = vkSignalSemaphore != VK_NULL_HANDLE ? &vkSignalSemaphore : NULL;
		return vkQueueSubmit(m_vkQueue, 1, &submitInfo, vkFence);
	}

	int CVulkanQueue::WaitIdle(void) const
	{
		return vkQueueWaitIdle(m_vkQueue);
	}

}
