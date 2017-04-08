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

	CRendererBufferManager::CRendererBufferManager(CRendererDevice *pDevice)
		: CRendererResourceManager(pDevice)
	{

	}

	CRendererBufferManager::~CRendererBufferManager(void)
	{

	}

	CRendererBuffer* CRendererBufferManager::AllocBuffer(void)
	{
		CRendererBuffer *pBuffer = SAFE_NEW CRendererBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CRendererIndexBuffer* CRendererBufferManager::AllocIndexBuffer(void)
	{
		CRendererIndexBuffer *pBuffer = SAFE_NEW CRendererIndexBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CRendererVertexBuffer* CRendererBufferManager::AllocVertexBuffer(void)
	{
		CRendererVertexBuffer *pBuffer = SAFE_NEW CRendererVertexBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CRendererUniformBuffer* CRendererBufferManager::AllocUniformBuffer(void)
	{
		CRendererUniformBuffer *pBuffer = SAFE_NEW CRendererUniformBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	void CRendererBufferManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize size = 0;

		LOGI("\n");
		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				if (const CRendererBuffer *pResource = (CRendererBuffer *)itResource.second) {
					pResource->DumpLog();
					size += pResource->GetSize();
					count++;
				}
			}
		}
		LOGI("*** %d objects found, total size %d\n", count, size);
		LOGI("\n");
	}

}
