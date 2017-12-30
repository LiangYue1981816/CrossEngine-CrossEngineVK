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

	CRenderSystem::CRenderSystem(void)
		: m_nFrameCount(0)

		, m_pGfxInstance(NULL)
		, m_pGfxDevice(NULL)
		, m_pGfxSwapchain(NULL)

		, m_pCameraManager(NULL)
		, m_pRenderPassManager(NULL)
		, m_pFrameBufferManager(NULL)
		, m_pBatchBufferManager(NULL)
	{

	}

	CRenderSystem::~CRenderSystem(void)
	{

	}

	uint32_t CRenderSystem::GetFrameCount(void) const
	{
		return m_nFrameCount;
	}

	CGfxDevice* CRenderSystem::GetDevice(void) const
	{
		return m_pGfxDevice;
	}

	CGfxSwapchain* CRenderSystem::GetSwapchain(void) const
	{
		return m_pGfxSwapchain;
	}

	CCameraManager* CRenderSystem::GetCameraManager(void) const
	{
		return m_pCameraManager;
	}

	CRenderPassManager* CRenderSystem::GetRenderPassManager(void) const
	{
		return m_pRenderPassManager;
	}

	CFrameBufferManager* CRenderSystem::GetFrameBufferManager(void) const
	{
		return m_pFrameBufferManager;
	}

	CBatchBufferManager* CRenderSystem::GetBatchBufferManager(void) const
	{
		return m_pBatchBufferManager;
	}

	BOOL CRenderSystem::Create(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		switch (api) {
		case GFX_API_GLES31: m_pGfxInstance = SAFE_NEW CGLES3Instance; break;
		case GFX_API_VULKAN: m_pGfxInstance = SAFE_NEW CVulkanInstance; break;
		}

		if (m_pGfxInstance == NULL || 
			m_pGfxInstance->Create(hInstance, hWnd, hDC, width, height, transform) == FALSE) {
			return FALSE;
		}

		m_pGfxDevice = m_pGfxInstance->GetDevice();
		m_pGfxSwapchain = m_pGfxInstance->GetSwapchain();

		m_pCameraManager = SAFE_NEW CCameraManager;
		m_pRenderPassManager = SAFE_NEW CRenderPassManager;
		m_pFrameBufferManager = SAFE_NEW CFrameBufferManager;
		m_pBatchBufferManager = SAFE_NEW CBatchBufferManager;

		return TRUE;
	}

	void CRenderSystem::Destroy(void)
	{
		SAFE_DELETE(m_pCameraManager);
		SAFE_DELETE(m_pBatchBufferManager);
		SAFE_DELETE(m_pFrameBufferManager);
		SAFE_DELETE(m_pRenderPassManager);

		m_pGfxInstance->Destroy();

		SAFE_DELETE(m_pGfxInstance);

		m_pGfxInstance = NULL;
		m_pGfxDevice = NULL;
		m_pGfxSwapchain = NULL;

		m_pRenderPassManager = NULL;
		m_pFrameBufferManager = NULL;
		m_pBatchBufferManager = NULL;
	}

	void CRenderSystem::Render(void)
	{
		m_nFrameCount++;
		m_pCameraManager->Render();
	}

}
