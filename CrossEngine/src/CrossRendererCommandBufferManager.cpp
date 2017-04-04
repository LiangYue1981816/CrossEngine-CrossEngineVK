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

	CRendererCommandBufferManager::CRendererCommandBufferManager(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CRendererCommandBufferManager::~CRendererCommandBufferManager(void)
	{
		ASSERT(m_pCommandPools.empty());
	}

	BOOL CRendererCommandBufferManager::Create(void)
	{
		return m_pCommandPools.empty() ? TRUE : FALSE;
	}

	void CRendererCommandBufferManager::Destroy(void)
	{
		for (std::map<uint32_t, CRendererCommandPool*>::const_iterator itCommandPool = m_pCommandPools.begin(); itCommandPool != m_pCommandPools.end(); ++itCommandPool) {
			if (CRendererCommandPool *pCommandPool = itCommandPool->second) {
				SAFE_DELETE(pCommandPool);
			}
		}

		m_pCommandPools.clear();
	}

	CRendererCommandBuffer* CRendererCommandBufferManager::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return GetCommandPool(pool)->AllocCommandBuffer(level);
	}

	void CRendererCommandBufferManager::FreeCommandBuffer(uint32_t pool, CRendererCommandBuffer *pCommandBuffer)
	{
		GetCommandPool(pool)->FreeCommandBuffer(pCommandBuffer);
	}

	void CRendererCommandBufferManager::ResetCommandPool(uint32_t pool, BOOL bReleaseResources)
	{
		GetCommandPool(pool)->ResetCommandPool(bReleaseResources);
	}

	CRendererCommandPool* CRendererCommandBufferManager::GetCommandPool(uint32_t pool)
	{
		std::map<uint32_t, CRendererCommandPool*>::const_iterator itCommandPool = m_pCommandPools.find(pool);
		if (itCommandPool != m_pCommandPools.end()) return itCommandPool->second;

		CRendererCommandPool *pCommandPool = SAFE_NEW CRendererCommandPool(m_pDevice);
		m_pCommandPools[pool] = pCommandPool;

		return pCommandPool;
	}

}
