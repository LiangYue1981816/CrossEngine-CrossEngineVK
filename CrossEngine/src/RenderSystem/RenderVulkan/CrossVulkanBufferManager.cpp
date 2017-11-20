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
		: m_pDevice(pDevice)
	{

	}

	CVulkanBufferManager::~CVulkanBufferManager(void)
	{

	}

	CGfxIndexBufferPtr CVulkanBufferManager::AllocIndexBuffer(void)
	{
		CVulkanIndexBuffer *pIndexBuffer = SAFE_NEW CVulkanIndexBuffer(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pIndexBuffer] = pIndexBuffer;
		}
		return CGfxIndexBufferPtr(pIndexBuffer);
	}

	CGfxVertexBufferPtr CVulkanBufferManager::AllocVertexBuffer(void)
	{
		CVulkanVertexBuffer *pVertexBuffer = SAFE_NEW CVulkanVertexBuffer(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pVertexBuffer] = pVertexBuffer;
		}
		return CGfxVertexBufferPtr(pVertexBuffer);
	}

	CGfxUniformBufferPtr CVulkanBufferManager::AllocUniformBuffer(void)
	{
		CVulkanUniformBuffer *pUniformBuffer = SAFE_NEW CVulkanUniformBuffer(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pUniformBuffer] = pUniformBuffer;
		}
		return CGfxUniformBufferPtr(pUniformBuffer);
	}

	void CVulkanBufferManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize bufferSize = 0;
		VkDeviceSize memorySize = 0;

		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				itResource.second->DumpLog();
				bufferSize += ((CGfxBuffer *)itResource.second)->GetBufferSize();
				memorySize += ((CGfxBuffer *)itResource.second)->GetMemorySize();
				count++;
			}
		}
		LOGI("*** %d objects found, total buffer size %d total memory size %d\n", count, bufferSize, memorySize);
		LOGI("\n");
	}

}
