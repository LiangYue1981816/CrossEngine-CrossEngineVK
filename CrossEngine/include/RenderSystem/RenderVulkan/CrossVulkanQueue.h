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

	class CROSS_EXPORT CVulkanQueue : public CGfxQueue
	{
		friend class CVulkanDevice;
		friend class CVulkanSwapchain;
		friend class CVulkanStagingBuffer;


	protected:
		CVulkanQueue(CVulkanDevice *pDevice);
		virtual ~CVulkanQueue(void);


	protected:
		int Create(uint32_t queueFamilyIndex, uint32_t queueIndex);
		void Destroy(void);

	protected:
		VkQueue GetQueue(void) const;
		uint32_t GetQueueFamilyIndex(void) const;

	public:
		int Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) const;
		int Submit(const CGfxCommandBufferPtr &ptrCommandBuffer, CGfxSemaphore waitSemaphore, VkPipelineStageFlags waitStageFlags, CGfxSemaphore signalSemaphore) const;
		int WaitIdle(void) const;


	protected:
		VkQueue m_vkQueue;
		uint32_t m_queueFamilyIndex;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
