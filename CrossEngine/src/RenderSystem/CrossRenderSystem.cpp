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
		: m_pGfxInstance(NULL)
		, m_pGfxDevice(NULL)
		, m_pGfxSwapchain(NULL)
		, m_pCameraManager(NULL)
	{

	}

	CRenderSystem::~CRenderSystem(void)
	{

	}

	GFX_API CRenderSystem::GetAPI(void) const
	{
		return m_api;
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

	BOOL CRenderSystem::Create(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		CALL_BOOL_FUNCTION_RETURN(CreateGfx(api, hInstance, hWnd, hDC, width, height, transform));
		CALL_BOOL_FUNCTION_RETURN(CreateCameraManager());

		return TRUE;
	}

	BOOL CRenderSystem::CreateGfx(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		m_api = api;

		switch (m_api) {
		case GFX_API_GLES31: m_pGfxInstance = SAFE_NEW CGLES3Instance; break;
		case GFX_API_VULKAN: m_pGfxInstance = SAFE_NEW CVulkanInstance; break;
		}

		if (m_pGfxInstance == NULL ||
			m_pGfxInstance->Create(hInstance, hWnd, hDC, width, height, transform) == FALSE) {
			return FALSE;
		}

		m_pGfxDevice = m_pGfxInstance->GetDevice();
		m_pGfxSwapchain = m_pGfxInstance->GetSwapchain();

		return TRUE;
	}

	BOOL CRenderSystem::CreateCameraManager(void)
	{
		m_pCameraManager = SAFE_NEW CCameraManager;
		return TRUE;
	}

	void CRenderSystem::Destroy(void)
	{
		DestroyCameraManager();
		DestroyGfx();
	}

	void CRenderSystem::DestroyGfx(void)
	{
		if (m_pGfxInstance) {
			m_pGfxInstance->Destroy();
		}

		SAFE_DELETE(m_pGfxInstance);
		m_pGfxInstance = NULL;
		m_pGfxDevice = NULL;
		m_pGfxSwapchain = NULL;
	}

	void CRenderSystem::DestroyCameraManager(void)
	{
		SAFE_DELETE(m_pCameraManager);
		m_pCameraManager = NULL;
	}

	void CRenderSystem::Update(void)
	{
		m_pCameraManager->Update();
	}

}
