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

	CVulkanStagingBufferManager::CVulkanStagingBufferManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_mutex(NULL)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CVulkanStagingBufferManager::~CVulkanStagingBufferManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	BOOL CVulkanStagingBufferManager::Create(void)
	{
		return TRUE;
	}

	void CVulkanStagingBufferManager::Destroy(void)
	{
		for (const auto &itBuffer : m_pBuffers) {
			if (CVulkanStagingBuffer *pBuffer = itBuffer.second) {
				SAFE_DELETE(pBuffer);
			}
		}

		m_pBuffers.clear();
	}

	CVulkanStagingBuffer* CVulkanStagingBufferManager::AllocBuffer(VkDeviceSize size)
	{
		CVulkanStagingBuffer *pBuffer = SAFE_NEW CVulkanStagingBuffer(m_pDevice, size);
		{
			mutex_autolock mutex(m_mutex);
			m_pBuffers[pBuffer] = pBuffer;
		}
		return pBuffer;
	}

	void CVulkanStagingBufferManager::FreeBuffer(CVulkanStagingBuffer *pBuffer)
	{
		{
			mutex_autolock mutex(m_mutex);
			const auto &itBuffer = m_pBuffers.find(pBuffer);
			if (itBuffer != m_pBuffers.end()) m_pBuffers.erase(itBuffer);
		}

		SAFE_DELETE(pBuffer);
	}

}
