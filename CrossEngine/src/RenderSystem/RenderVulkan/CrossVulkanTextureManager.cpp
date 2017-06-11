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

	CVulkanTextureManager::CVulkanTextureManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CVulkanTextureManager::~CVulkanTextureManager(void)
	{

	}

	CGfxTexturePtr CVulkanTextureManager::AllocTexture(void)
	{
		CVulkanTexture *pTexture = SAFE_NEW CVulkanTexture(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pTexture] = pTexture;
		}
		return CGfxTexturePtr(pTexture);
	}

	CGfxRenderTexturePtr CVulkanTextureManager::AllocRenderTexture(void)
	{
		CVulkanRenderTexture *pRenderTexture = SAFE_NEW CVulkanRenderTexture(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pRenderTexture] = pRenderTexture;
		}
		return CGfxRenderTexturePtr(pRenderTexture);
	}

	void CVulkanTextureManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize size = 0;

		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				if (const CGfxImage *pResource = (CGfxImage *)itResource.second) {
					pResource->DumpLog();
					size += pResource->GetMemorySize();
					count++;
				}
			}
		}
		LOGI("*** %d objects found, total size %d\n", count, size);
		LOGI("\n");
	}

}
