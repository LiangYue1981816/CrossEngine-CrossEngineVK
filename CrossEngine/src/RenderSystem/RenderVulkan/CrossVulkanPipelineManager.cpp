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

	CVulkanPipelineManager::CVulkanPipelineManager(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_vkPipelineCache(VK_NULL_HANDLE)
	{

	}

	CVulkanPipelineManager::~CVulkanPipelineManager(void)
	{

	}

	BOOL CVulkanPipelineManager::Create(void)
	{
		try {
			VkPipelineCacheCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.initialDataSize = 0;
			createInfo.pInitialData = NULL;
			CALL_VK_FUNCTION_THROW(vkCreatePipelineCache(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineCache));

			return CGfxResourceManager::Create();
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanPipelineManager::Destroy(void)
	{
		CGfxResourceManager::Destroy();

		if (m_vkPipelineCache) {
			vkDestroyPipelineCache(m_pDevice->GetDevice(), m_vkPipelineCache, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkPipelineCache = VK_NULL_HANDLE;
	}

	CGfxPipelineComputePtr CVulkanPipelineManager::AllocPipelineCompute(void)
	{
		CVulkanPipelineCompute *pPipelineCompute = SAFE_NEW CVulkanPipelineCompute(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pPipelineCompute] = pPipelineCompute;
		}
		return CGfxPipelineComputePtr(pPipelineCompute);
	}

	CGfxPipelineGraphicsPtr CVulkanPipelineManager::AllocPipelineGraphics(void)
	{
		CVulkanPipelineGraphics *pPipelineGraphics = SAFE_NEW CVulkanPipelineGraphics(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pPipelineGraphics] = pPipelineGraphics;
		}
		return CGfxPipelineGraphicsPtr(pPipelineGraphics);
	}

	VkPipelineCache CVulkanPipelineManager::GetPipelineCache(void) const
	{
		return m_vkPipelineCache;
	}

}
