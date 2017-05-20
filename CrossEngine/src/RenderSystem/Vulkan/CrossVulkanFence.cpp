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

	CVulkanFence::CVulkanFence(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager)
		: CVulkanResource(pDevice, pResourceManager)
		, m_vkFence(VK_NULL_HANDLE)
	{

	}

	CVulkanFence::~CVulkanFence(void)
	{

	}

	BOOL CVulkanFence::Create(void)
	{
		try {
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			CALL_VK_FUNCTION_THROW(vkCreateFence(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanFence::Destroy(void)
	{
		if (m_vkFence) {
			vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkFence = VK_NULL_HANDLE;
	}

	VkFence CVulkanFence::GetFence(void) const
	{
		return m_vkFence;
	}

	BOOL CVulkanFence::IsSignaled(void) const
	{
		return vkGetFenceStatus(m_pDevice->GetDevice(), m_vkFence) == VK_SUCCESS ? TRUE : FALSE;
	}

	BOOL CVulkanFence::Wait(uint64_t timeout) const
	{
		return vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_FALSE, timeout) == VK_SUCCESS ? TRUE : FALSE;
	}

	VkResult CVulkanFence::Reset(void) const
	{
		return vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence);
	}

	void CVulkanFence::DumpLog(void) const
	{
		if (m_vkFence) {
			LOGI("\t\tFence 0x%x\n", m_vkFence);
		}
	}

}
