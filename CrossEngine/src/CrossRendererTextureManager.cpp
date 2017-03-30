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

	CRendererTextureManager::CRendererTextureManager(CRendererDevice *pDevice)
		: CRendererResourceManager(pDevice)
	{

	}

	CRendererTextureManager::~CRendererTextureManager(void)
	{

	}

	CRendererTexture* CRendererTextureManager::AllocTexture(void)
	{
		CRendererTexture *pTexture = SAFE_NEW CRendererTexture(m_pDevice, this);
		m_pResources[pTexture] = pTexture;
		return pTexture;
	}

	CRendererRenderTexture* CRendererTextureManager::AllocRenderTexture(void)
	{
		CRendererRenderTexture *pTexture = SAFE_NEW CRendererRenderTexture(m_pDevice, this);
		m_pResources[pTexture] = pTexture;
		return pTexture;
	}

	void CRendererTextureManager::DumpLog(const char *szTitle) const
	{
		LOGI("\n");
		LOGI("%s\n", szTitle);
		uint32_t count = 0;
		VkDeviceSize size = 0;
		for (std::map<CRendererResource*, CRendererResource*>::const_iterator itResource = m_pResources.begin(); itResource != m_pResources.end(); ++itResource) {
			if (const CRendererImage *pResource = (CRendererImage *)itResource->second) {
				pResource->DumpLog();
				size += pResource->GetSize();
				count++;
			}
		}
		LOGI("*** %d objects found, total size %d\n", count, size);
		LOGI("\n");
	}

}
