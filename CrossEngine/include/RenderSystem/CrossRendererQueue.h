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

	class CROSS_EXPORT CRendererQueue
	{
		friend class CRendererDevice;
		friend class CRendererDeviceCompute;
		friend class CRendererDeviceGraphics;


	protected:
		CRendererQueue(CRendererDevice *pDevice);
		virtual ~CRendererQueue(void);


	protected:
		BOOL Create(uint32_t queueFamilyIndex);
		void Destroy(void);


	public:
		VkQueue GetQueue(void) const;
		uint32_t GetQueueFamilyIndex(void) const;

	public:
		VkResult Submit(VkCommandBuffer vkCommandBuffers, VkFence vkFence) const;
		VkResult Submit(VkCommandBuffer vkCommandBuffers, VkSemaphore vkWaitSemaphores, VkPipelineStageFlags vkWaitDstStageMask, VkSemaphore vkSignalSemaphores, VkFence vkFence) const;
		VkResult Submit(uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers, uint32_t waitSemaphoreCount, const VkSemaphore *pWaitSemaphores, const VkPipelineStageFlags *pWaitDstStageMask, uint32_t signalSemaphoreCount, const VkSemaphore *pSignalSemaphores, VkFence vkFence) const;
		VkResult WaitIdle(void) const;


	protected:
		VkQueue m_vkQueue;
		uint32_t m_queueFamilyIndex;

	protected:
		CRendererDevice *m_pDevice;
	};

}
