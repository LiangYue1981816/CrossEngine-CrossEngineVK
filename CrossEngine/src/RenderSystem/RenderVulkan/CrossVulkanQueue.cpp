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
		return TRUE;
	}

	void CVulkanQueue::Destroy(void)
	{

	}

	VkQueue CVulkanQueue::GetQueue(void) const
	{
		return m_vkQueue;
	}

	uint32_t CVulkanQueue::GetQueueFamilyIndex(void) const
	{
		return m_queueFamilyIndex;
	}

	BOOL CVulkanQueue::Submit(CGfxCommandBuffer *pCommandBuffer) const
	{
		return TRUE;
	}

	BOOL CVulkanQueue::Submit(CGfxCommandBuffer *pCommandBuffer, CGfxSemaphore waitSemaphore, PipelineStageFlags waitStageFlags, CGfxSemaphore signalSemaphore) const
	{
		return TRUE;
	}

	BOOL CVulkanQueue::WaitIdle(void) const
	{
		return TRUE;
	}

}
