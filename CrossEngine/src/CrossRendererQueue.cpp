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

	CRendererQueue::CRendererQueue(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkQueue(VK_NULL_HANDLE)
		, m_queueFamilyIndex(UINT32_MAX)
	{

	}

	CRendererQueue::~CRendererQueue(void)
	{
		ASSERT(m_vkQueue == VK_NULL_HANDLE);
	}

	BOOL CRendererQueue::Create(uint32_t queueFamilyIndex)
	{
		m_queueFamilyIndex = queueFamilyIndex;
		vkGetDeviceQueue(m_pDevice->GetDevice(), m_queueFamilyIndex, 0, &m_vkQueue);
		return TRUE;
	}

	void CRendererQueue::Destroy(void)
	{
		m_vkQueue = VK_NULL_HANDLE;
	}

	VkQueue CRendererQueue::GetQueue(void) const
	{
		return m_vkQueue;
	}

	uint32_t CRendererQueue::GetQueueFamilyIndex(void) const
	{
		return m_queueFamilyIndex;
	}

	VkResult CRendererQueue::Submit(uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers, const VkSemaphore *pWaitSemaphore, const VkPipelineStageFlags *pWaitDstStageMask, const VkSemaphore *pSignalSemaphore, VkFence vkFence) const
	{
		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = NULL;
		submitInfo.waitSemaphoreCount = pWaitSemaphore == NULL ? 0 : 1;
		submitInfo.pWaitSemaphores = pWaitSemaphore;
		submitInfo.pWaitDstStageMask = pWaitDstStageMask;
		submitInfo.commandBufferCount = commandBufferCount;
		submitInfo.pCommandBuffers = pCommandBuffers;
		submitInfo.signalSemaphoreCount = pSignalSemaphore == NULL ? 0 : 1;
		submitInfo.pSignalSemaphores = pSignalSemaphore;
		return vkQueueSubmit(m_vkQueue, 1, &submitInfo, vkFence);
	}

	VkResult CRendererQueue::WaitIdle(void) const
	{
		return vkQueueWaitIdle(m_vkQueue);
	}

}
