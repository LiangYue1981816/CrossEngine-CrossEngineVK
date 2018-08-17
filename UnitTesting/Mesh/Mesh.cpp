#include "stdafx.h"

uint32_t dwMainCamera = CrossEngine::HashValue("MainCamera");

CrossEngine::CResMeshPtr ptrMesh;
CrossEngine::CResMaterialPtr ptrMaterial;
CrossEngine::CDrawableStaticMesh *pMesh = NULL;

CrossEngine::CResRenderPassPtr ptrRenderPass;
CrossEngine::CResFrameBufferPtr ptrFrameBuffer[3];

void Create(void)
{
	ptrRenderPass = RenderPassManager()->LoadResource("ForwardPresent.renderpass", TRUE, TRUE);
	ptrFrameBuffer[0] = FrameBufferManager()->LoadResource("Present0.framebuffer", TRUE, TRUE);
	ptrFrameBuffer[1] = FrameBufferManager()->LoadResource("Present1.framebuffer", TRUE, TRUE);
	ptrFrameBuffer[2] = FrameBufferManager()->LoadResource("Present2.framebuffer", TRUE, TRUE);

	ptrMaterial = MaterialManager()->LoadResource("PzVI-Tiger-P_Chassis.material", TRUE, TRUE);
	ptrMesh = MeshManager()->LoadResource("PzVI-Tiger-P_Body.mesh", TRUE, TRUE);

	pMesh = (CrossEngine::CDrawableStaticMesh *)RenderSystem()->AllocDrawable(CrossEngine::DRAWABLE_TYPE_STATIC_MESH);
	pMesh->SetMesh(ptrMesh);
	pMesh->SetMaterial(ptrMaterial);

	float sh_red[9] = { 0.0 };
	float sh_green[9] = { 0.0 };
	float sh_blue[9] = { 0.0 };
	if (FILE *pFile = fopen("../Data/Env/Env.sh", "rb")) {
		fscanf(pFile, "%f %f %f %f %f %f %f %f %f", &sh_red[0], &sh_red[1], &sh_red[2], &sh_red[3], &sh_red[4], &sh_red[5], &sh_red[6], &sh_red[7], &sh_red[8]);
		fscanf(pFile, "%f %f %f %f %f %f %f %f %f", &sh_green[0], &sh_green[1], &sh_green[2], &sh_green[3], &sh_green[4], &sh_green[5], &sh_green[6], &sh_green[7], &sh_green[8]);
		fscanf(pFile, "%f %f %f %f %f %f %f %f %f", &sh_blue[0], &sh_blue[1], &sh_blue[2], &sh_blue[3], &sh_blue[4], &sh_blue[5], &sh_blue[6], &sh_blue[7], &sh_blue[8]);
		fclose(pFile);
	}

	RenderSystem()->SetAmbientLightColor(sh_red, sh_green, sh_blue);
	RenderSystem()->SetDirectLightColor(3.0f, 3.0f, 3.0f);
	RenderSystem()->SetDirectLightDirection(-1.0f, -1.0f, -1.0f);
	RenderSystem()->SetFogColor(1.0f, 1.0f, 1.0f);
	RenderSystem()->SetFogDistanceDensity(10.0f, 100.0f, 1.0f);
	RenderSystem()->SetFogHeightDensity(1.0f, 100.0f, 1.0f);
}

void Destroy(void)
{
	RenderSystem()->FreeDrawable(pMesh);
	ptrMaterial.Release();
	ptrMesh.Release();

	ptrFrameBuffer[0].Release();
	ptrFrameBuffer[1].Release();
	ptrFrameBuffer[2].Release();
	ptrRenderPass.Release();
}

void Render(void)
{
	if (GfxSwapChain() == NULL) {
		return;
	}

	Camera(dwMainCamera)->SetScissor(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight());
	Camera(dwMainCamera)->SetViewport(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight());
	Camera(dwMainCamera)->SetPerspective(60.0f, 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
	Camera(dwMainCamera)->SetLookat(0.0f, 3.5f, 10.0f, 0.0f, -0.25f, -1.0f, 0.0f, 1.0f, 0.0f);
	Camera(dwMainCamera)->ClearRenderQueue();
	Camera(dwMainCamera)->AddRenderQueue(pMesh);

	static float angle = 0.0f; angle += 1.0f;
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 rotatex = glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotatey = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	pMesh->SetTransform(rotatey);

	GfxSwapChain()->AcquireNextImage(VK_NULL_HANDLE);
	{
		RenderSystem()->Update();
		RenderSystem()->Render(dwMainCamera, ptrFrameBuffer[GfxSwapChain()->GetImageIndex()]->GetFrameBuffer(), ptrRenderPass->GetRenderPass());
	}
	GfxSwapChain()->Present();
}
