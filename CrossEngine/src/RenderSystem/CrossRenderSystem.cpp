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

		, m_pCameraManager(NULL)
		, m_pDrawableManager(NULL)

		, m_pAmbientLight(NULL)
		, m_pDirectLight(NULL)
		, m_pPointLight(NULL)
		, m_pFog(NULL)

		, m_pRenderer(NULL)
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

	const CGfxDescriptorSetPtr& CRenderSystem::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
	}

	BOOL CRenderSystem::Create(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		CALL_BOOL_FUNCTION_RETURN(CreateGfx(api, hInstance, hWnd, hDC, width, height, transform));
		CALL_BOOL_FUNCTION_RETURN(CreateDescriptorSet());
		CALL_BOOL_FUNCTION_RETURN(CreateRenderer());
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
		DestroyRenderer();
		DestroyDescriptorSet();
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

	BOOL CRenderSystem::CreateDescriptorSet(void)
	{
		m_pAmbientLight = SAFE_NEW CAmbientLight;
		m_pDirectLight = SAFE_NEW CDirectLight;
		m_pPointLight = SAFE_NEW CPointLight;
		m_pFog = SAFE_NEW CFog;

		m_ptrDescriptorSetLayout = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_FRAME);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_AMBIENT_LIGHT], DESCRIPTOR_BIND_AMBIENT_LIGHT, VK_SHADER_STAGE_ALL);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_POINT_LIGHT], DESCRIPTOR_BIND_POINT_LIGHT, VK_SHADER_STAGE_ALL);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_DIRECT_LIGHT], DESCRIPTOR_BIND_DIRECT_LIGHT, VK_SHADER_STAGE_ALL);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_FOG], DESCRIPTOR_BIND_FOG, VK_SHADER_STAGE_ALL);
		m_ptrDescriptorSetLayout->Create();

		m_ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(thread_id(), m_ptrDescriptorSetLayout);
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_AMBIENT_LIGHT, m_pAmbientLight->GetUniformBuffer());
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_DIRECT_LIGHT, m_pDirectLight->GetUniformBuffer());
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_POINT_LIGHT, m_pPointLight->GetUniformBuffer());
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_FOG, m_pFog->GetUniformBuffer());
		m_ptrDescriptorSet->UpdateDescriptorSets();

		return TRUE;
	}

	BOOL CRenderSystem::CreateRenderer(void)
	{
		m_pRenderer = SAFE_NEW CRenderer;
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

	void CRenderSystem::DestroyDescriptorSet(void)
	{
		m_ptrDescriptorSet.Release();
		m_ptrDescriptorSetLayout.Release();

		SAFE_DELETE(m_pAmbientLight);
		SAFE_DELETE(m_pDirectLight);
		SAFE_DELETE(m_pPointLight);
		SAFE_DELETE(m_pFog);
	}

	void CRenderSystem::DestroyRenderer(void)
	{
		SAFE_DELETE(m_pRenderer);
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
		m_pAmbientLight->SetAmbient(shRed, shGreen, shBlue);
	}

	void CRenderSystem::SetAmbientRotation(const glm::mat4 &mtxRotation)
	{
		m_pAmbientLight->SetRotation(mtxRotation);
	}

	void CRenderSystem::SetDirectLightColor(float red, float green, float blue)
	{
		m_pDirectLight->SetColor(red, green, blue);
	}

	void CRenderSystem::SetDirectLightDirection(float x, float y, float z)
	{
		m_pDirectLight->SetDirection(x, y, z);
	}

	void CRenderSystem::SetPointLightColor(float red, float green, float blue)
	{
		m_pPointLight->SetColor(red, green, blue);
	}

	void CRenderSystem::SetPointLightPosition(float x, float y, float z, float radius)
	{
		m_pPointLight->SetPosition(x, y, z, radius);
	}

	void CRenderSystem::SetPointLightAttenuation(float linear, float square, float constant)
	{
		m_pPointLight->SetAttenuation(linear, square, constant);
	}

	void CRenderSystem::SetFogColor(float red, float green, float blue)
	{
		m_pFog->SetColor(red, green, blue);
	}

	void CRenderSystem::SetFogHeightDensity(float startHeight, float endHeight, float density)
	{
		m_pFog->SetHeightDensity(startHeight, endHeight, density);
	}

	void CRenderSystem::SetFogDistanceDensity(float startDistance, float endDistance, float density)
	{
		m_pFog->SetDistanceDensity(startDistance, endDistance, density);
	}

	void CRenderSystem::Update(void)
	{
		m_pAmbientLight->Apply();
		m_pDirectLight->Apply();
		m_pPointLight->Apply();
		m_pFog->Apply();

		m_pCameraManager->Update();
	}

	void CRenderSystem::Render(uint32_t dwCameraName, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		if (CCamera *pCamera = m_pCameraManager->GetCamera(dwCameraName)) {
			if (pCamera->IsEnable()) {
				m_pRenderer->BuildCommandBuffer(pCamera, ptrFrameBuffer, ptrRenderPass);
				m_pRenderer->Render(pCamera, ptrFrameBuffer, ptrRenderPass);
			}
		}
	}

}
