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

	CRendererSemaphore::CRendererSemaphore(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkSemaphore(VK_NULL_HANDLE)
	{

	}

	CRendererSemaphore::~CRendererSemaphore(void)
	{
		ASSERT(m_vkSemaphore == VK_NULL_HANDLE);
	}

	BOOL CRendererSemaphore::Create(void)
	{
		try {
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			CALL_VK_FUNCTION_THROW(vkCreateSemaphore(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkSemaphore));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererSemaphore::Destroy(void)
	{
		if (m_vkSemaphore) {
			vkDestroySemaphore(m_pDevice->GetDevice(), m_vkSemaphore, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSemaphore = VK_NULL_HANDLE;
	}

	void CRendererSemaphore::DumpLog(void) const
	{
		if (m_vkSemaphore) {
			LOGI("\t\tSemaphore 0x%x\n", m_vkSemaphore);
		}
	}

	VkSemaphore CRendererSemaphore::GetSemaphore(void) const
	{
		return m_vkSemaphore;
	}

}
