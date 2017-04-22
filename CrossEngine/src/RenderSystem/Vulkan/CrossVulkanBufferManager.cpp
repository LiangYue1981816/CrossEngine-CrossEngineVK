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

	CVulkanBufferManager::CVulkanBufferManager(CVulkanDevice *pDevice)
		: CVulkanResourceManager(pDevice)
	{

	}

	CVulkanBufferManager::~CVulkanBufferManager(void)
	{

	}

	CVulkanBuffer* CVulkanBufferManager::AllocBuffer(void)
	{
		CVulkanBuffer *pBuffer = SAFE_NEW CVulkanBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CVulkanIndexBuffer* CVulkanBufferManager::AllocIndexBuffer(void)
	{
		CVulkanIndexBuffer *pBuffer = SAFE_NEW CVulkanIndexBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CVulkanVertexBuffer* CVulkanBufferManager::AllocVertexBuffer(void)
	{
		CVulkanVertexBuffer *pBuffer = SAFE_NEW CVulkanVertexBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	CVulkanUniformBuffer* CVulkanBufferManager::AllocUniformBuffer(void)
	{
		CVulkanUniformBuffer *pBuffer = SAFE_NEW CVulkanUniformBuffer(m_pDevice, this);
		m_pResources[pBuffer] = pBuffer;

		return pBuffer;
	}

	void CVulkanBufferManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize bufferSize = 0;
		VkDeviceSize memorySize = 0;

		LOGI("\n");
		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				if (const CVulkanBuffer *pResource = (CVulkanBuffer *)itResource.second) {
					pResource->DumpLog();
					bufferSize += pResource->GetBufferSize();
					memorySize += pResource->GetMemorySize();
					count++;
				}
			}
		}
		LOGI("*** %d objects found, total buffer size %d total memory size %d\n", count, bufferSize, memorySize);
		LOGI("\n");
	}

}
