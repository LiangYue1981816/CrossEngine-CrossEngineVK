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
		: m_api(GFX_API_NONE)

		, m_pGfxInstance(NULL)
		, m_pGfxDevice(NULL)
		, m_pGfxSwapchain(NULL)

		, m_pFxManager(NULL)
		, m_pLightManager(NULL)
		, m_pCameraManager(NULL)
		, m_pDrawableManager(NULL)
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

	BOOL CRenderSystem::Create(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		CALL_BOOL_FUNCTION_RETURN(CreateGfx(api, hInstance, hWnd, hDC, width, height, transform));
		CALL_BOOL_FUNCTION_RETURN(CreateFxManager());
		CALL_BOOL_FUNCTION_RETURN(CreateLightManager());
		CALL_BOOL_FUNCTION_RETURN(CreateCameraManager());
		CALL_BOOL_FUNCTION_RETURN(CreateDrawableManager());
		CALL_BOOL_FUNCTION_RETURN(CreateBatchBuffer());

		return TRUE;
	}

	void CRenderSystem::Destroy(void)
	{
		DestroyBatchBuffer();
		DestroyDrawableManager();
		DestroyCameraManager();
		DestroyLightManager();
		DestroyFxManager();
		DestroyGfx();
	}

	BOOL CRenderSystem::CreateGfx(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		switch (api) {
		case GFX_API_GLES31: m_pGfxInstance = SAFE_NEW CGLES3Instance; break;
		case GFX_API_VULKAN: m_pGfxInstance = SAFE_NEW CVulkanInstance; break;
		}

		if (m_pGfxInstance == NULL ||
			m_pGfxInstance->Create(hInstance, hWnd, hDC, width, height, transform) == FALSE) {
			return FALSE;
		}

		m_api = api;
		m_pGfxDevice = m_pGfxInstance->GetDevice();
		m_pGfxSwapchain = m_pGfxInstance->GetSwapchain();

		return TRUE;
	}

	BOOL CRenderSystem::CreateFxManager(void)
	{
		m_pFxManager = SAFE_NEW CFxManager;
		return TRUE;
	}

	BOOL CRenderSystem::CreateLightManager(void)
	{
		m_pLightManager = SAFE_NEW CLightManager;
		return TRUE;
	}

	BOOL CRenderSystem::CreateCameraManager(void)
	{
		m_pCameraManager = SAFE_NEW CCameraManager;
		return TRUE;
	}

	BOOL CRenderSystem::CreateDrawableManager(void)
	{
		m_pDrawableManager = SAFE_NEW CDrawableManager;
		return TRUE;
	}

	BOOL CRenderSystem::CreateBatchBuffer(void)
	{
		CBatchPartical::CreateBatchBuffer();
		CBatchSkinMesh::CreateBatchBuffer();
		CBatchStaticMesh::CreateBatchBuffer();
		return TRUE;
	}

	void CRenderSystem::DestroyGfx(void)
	{
		if (m_pGfxInstance) {
			m_pGfxInstance->Destroy();
		}

		SAFE_DELETE(m_pGfxInstance);

		m_api = GFX_API_NONE;
		m_pGfxDevice = NULL;
		m_pGfxSwapchain = NULL;
	}

	void CRenderSystem::DestroyFxManager(void)
	{
		SAFE_DELETE(m_pFxManager);
	}

	void CRenderSystem::DestroyLightManager(void)
	{
		SAFE_DELETE(m_pLightManager);
	}

	void CRenderSystem::DestroyCameraManager(void)
	{
		SAFE_DELETE(m_pCameraManager);
	}

	void CRenderSystem::DestroyDrawableManager(void)
	{
		SAFE_DELETE(m_pDrawableManager);
	}

	void CRenderSystem::DestroyBatchBuffer(void)
	{
		CBatchPartical::DestroyBatchBuffer();
		CBatchSkinMesh::DestroyBatchBuffer();
		CBatchStaticMesh::DestroyBatchBuffer();
	}

	CCamera* CRenderSystem::GetCamera(uint32_t dwName)
	{
		return m_pCameraManager->GetCamera(dwName);
	}

	void CRenderSystem::RemoveCamera(uint32_t dwName)
	{
		m_pCameraManager->RemoveCamera(dwName);
	}

	void CRenderSystem::RemoveCameraAll(void)
	{
		m_pCameraManager->RemoveCameraAll();
	}

	CDrawable* CRenderSystem::AllocDrawable(DRAWABLE_TYPE type)
	{
		return m_pDrawableManager->AllocDrawable(type);
	}

	void CRenderSystem::FreeDrawable(CDrawable *pDrawable)
	{
		m_pDrawableManager->FreeDrawable(pDrawable);
	}

	void CRenderSystem::FreeDrawableAll(void)
	{
		m_pDrawableManager->FreeDrawableAll();
	}

	void CRenderSystem::SetAmbientColor(float shRed[9], float shGreen[9], float shBlue[9])
	{
		m_pLightManager->SetAmbientColor(shRed, shGreen, shBlue);
	}

	void CRenderSystem::SetAmbientRotation(const glm::mat4 &mtxRotation)
	{
		m_pLightManager->SetAmbientRotation(mtxRotation);
	}

	void CRenderSystem::SetDirectionLightColor(float red, float green, float blue)
	{
		m_pLightManager->SetDirectionLightColor(red, green, blue);
	}

	void CRenderSystem::SetDirectionLightDirection(float x, float y, float z)
	{
		m_pLightManager->SetDirectionLightDirection(x, y, z);
	}

	void CRenderSystem::Update(void)
	{
		m_pCameraManager->Update();
	}

	void CRenderSystem::Render(uint32_t dwCameraName, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		m_pCameraManager->Render(dwCameraName, ptrFrameBuffer, ptrRenderPass);
	}

}
