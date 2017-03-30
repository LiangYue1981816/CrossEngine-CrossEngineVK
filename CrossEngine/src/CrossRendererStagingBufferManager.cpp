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
		, m_pFence(NULL)
	{

	}

	CRendererStagingBufferManager::~CRendererStagingBufferManager(void)
	{
		ASSERT(m_pFence == NULL);
		ASSERT(m_pBuffers.empty());
		ASSERT(m_pPendings.empty());
	}

	BOOL CRendererStagingBufferManager::Create(void)
	{
		if (m_pFence == NULL) {
			m_pFence = m_pDevice->GetFenceManager()->AllocFence();
			m_pFence->Create();
		}

		return m_pBuffers.empty() ? TRUE : FALSE;
	}

	void CRendererStagingBufferManager::Destroy(void)
	{
		if (m_pFence) {
			m_pDevice->GetFenceManager()->Free(m_pFence);
		}

		for (std::vector<CRendererStagingBuffer*>::const_iterator itPending = m_pPendings.begin(); itPending != m_pPendings.end(); ++itPending) {
			if (CRendererStagingBuffer *pBuffer = *itPending) {
				SAFE_DELETE(pBuffer);
			}
		}

		for (std::map<CRendererStagingBuffer*, CRendererStagingBuffer*>::const_iterator itBuffer = m_pBuffers.begin(); itBuffer != m_pBuffers.end(); ++itBuffer) {
			if (CRendererStagingBuffer *pBuffer = itBuffer->second) {
				SAFE_DELETE(pBuffer);
			}
		}

		m_pFence = NULL;
		m_pBuffers.clear();
		m_pPendings.clear();
	}

	CRendererStagingBuffer* CRendererStagingBufferManager::AllocBuffer(VkDeviceSize size)
	{
		CRendererStagingBuffer *pBuffer = SAFE_NEW CRendererStagingBuffer(m_pDevice, size);
		m_pBuffers[pBuffer] = pBuffer;
		return pBuffer;
	}

	void CRendererStagingBufferManager::FreeBuffer(CRendererStagingBuffer *pBuffer)
	{
		std::map<CRendererStagingBuffer*, CRendererStagingBuffer*>::const_iterator itBuffer = m_pBuffers.find(pBuffer);
		if (itBuffer != m_pBuffers.end()) m_pBuffers.erase(itBuffer);

		SAFE_DELETE(pBuffer);
	}

	void CRendererStagingBufferManager::PendFreeBuffer(CRendererStagingBuffer *pBuffer)
	{
		std::map<CRendererStagingBuffer*, CRendererStagingBuffer*>::const_iterator itBuffer = m_pBuffers.find(pBuffer);
		if (itBuffer != m_pBuffers.end()) m_pBuffers.erase(itBuffer);

		m_pPendings.push_back(pBuffer);
	}

	BOOL CRendererStagingBufferManager::Process(void)
	{
		try {
			if (m_pPendings.size()) {
				std::vector<VkCommandBuffer> commandBuffers;
				for (std::vector<CRendererStagingBuffer*>::const_iterator itPending = m_pPendings.begin(); itPending != m_pPendings.end(); ++itPending) {
					if (CRendererStagingBuffer *pBuffer = *itPending) {
						commandBuffers.push_back(pBuffer->GetCommandBuffer()->GetCommandBuffer());
					}
				}

				CALL_VK_FUNCTION_THROW(m_pFence->Reset());
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->Submit(commandBuffers.size(), commandBuffers.data(), NULL, NULL, NULL, m_pFence->GetFence()));
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->WaitIdle());

				for (std::vector<CRendererStagingBuffer*>::const_iterator itPending = m_pPendings.begin(); itPending != m_pPendings.end(); ++itPending) {
					if (CRendererStagingBuffer *pBuffer = *itPending) {
						SAFE_DELETE(pBuffer);
					}
				}

				m_pPendings.clear();
			}

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

}
