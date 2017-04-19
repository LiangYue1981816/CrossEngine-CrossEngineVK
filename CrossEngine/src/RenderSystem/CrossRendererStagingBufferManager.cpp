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

	CRendererStagingBufferManager::CRendererStagingBufferManager(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CRendererStagingBufferManager::~CRendererStagingBufferManager(void)
	{
		ASSERT(m_pBuffers.empty());
	}

	BOOL CRendererStagingBufferManager::Create(void)
	{
		return m_pBuffers.empty() ? TRUE : FALSE;
	}

	void CRendererStagingBufferManager::Destroy(void)
	{
		for (const auto &itBuffer : m_pBuffers) {
			if (CRendererStagingBuffer *pBuffer = itBuffer.second) {
				SAFE_DELETE(pBuffer);
			}
		}

		m_pBuffers.clear();
	}

	CRendererStagingBuffer* CRendererStagingBufferManager::AllocBuffer(VkDeviceSize size)
	{
		CRendererStagingBuffer *pBuffer = SAFE_NEW CRendererStagingBuffer(m_pDevice, size);
		m_pBuffers[pBuffer] = pBuffer;

		return pBuffer;
	}

	void CRendererStagingBufferManager::FreeBuffer(CRendererStagingBuffer *pBuffer)
	{
		const auto &itBuffer = m_pBuffers.find(pBuffer);
		if (itBuffer != m_pBuffers.end()) m_pBuffers.erase(itBuffer);

		SAFE_DELETE(pBuffer);
	}

}
