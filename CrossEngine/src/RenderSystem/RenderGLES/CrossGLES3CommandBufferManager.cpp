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

	CGLES3CommandBufferManager::CGLES3CommandBufferManager(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CGLES3CommandBufferManager::~CGLES3CommandBufferManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	int CGLES3CommandBufferManager::Create(void)
	{
		return NO_ERROR;
	}

	void CGLES3CommandBufferManager::Destroy(void)
	{
		for (const auto &itCommandBuffer : m_pCommandBuffers) {
			if (CGLES3CommandBuffer *pCommandBuffer = itCommandBuffer.second) {
				SAFE_DELETE(pCommandBuffer);
			}
		}

		m_pCommandBuffers.clear();
	}

	CGfxCommandBufferPtr CGLES3CommandBufferManager::AllocCommandBuffer(void)
	{
		CGLES3CommandBuffer *pCommandBuffer = SAFE_NEW CGLES3CommandBuffer(m_pDevice);
		{
			mutex_autolock mutex(m_mutex);
			m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
		}
		return CGfxCommandBufferPtr(pCommandBuffer);
	}

	void CGLES3CommandBufferManager::FreeCommandBuffer(CGLES3CommandBuffer *pCommandBuffer)
	{
		mutex_autolock mutex(m_mutex);
		m_pCommandBuffers.erase(pCommandBuffer);
		SAFE_DELETE(pCommandBuffer);
	}

}
