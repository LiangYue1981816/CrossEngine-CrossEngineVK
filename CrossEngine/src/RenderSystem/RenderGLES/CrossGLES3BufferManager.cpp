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

	CGLES3BufferManager::CGLES3BufferManager(void)
	{

	}

	CGLES3BufferManager::~CGLES3BufferManager(void)
	{

	}

	CGfxIndexBufferPtr CGLES3BufferManager::AllocIndexBuffer(void)
	{
		CGLES3IndexBuffer *pIndexBuffer = SAFE_NEW CGLES3IndexBuffer(this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pIndexBuffer] = pIndexBuffer;
		}
		return CGfxIndexBufferPtr(pIndexBuffer);
	}

	CGfxVertexBufferPtr CGLES3BufferManager::AllocVertexBuffer(void)
	{
		CGLES3VertexBuffer *pVertexBuffer = SAFE_NEW CGLES3VertexBuffer(this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pVertexBuffer] = pVertexBuffer;
		}
		return CGfxVertexBufferPtr(pVertexBuffer);
	}

	CGfxUniformBufferPtr CGLES3BufferManager::AllocUniformBuffer(void)
	{
		CGLES3UniformBuffer *pUniformBuffer = SAFE_NEW CGLES3UniformBuffer(this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pUniformBuffer] = pUniformBuffer;
		}
		return CGfxUniformBufferPtr(pUniformBuffer);
	}

	void CGLES3BufferManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;
		VkDeviceSize bufferSize = 0;
		VkDeviceSize memorySize = 0;

		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				if (const CGfxBuffer *pResource = (CGfxBuffer *)itResource.second) {
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
