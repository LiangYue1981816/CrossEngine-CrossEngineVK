#include "stdafx.h"

uint32_t dwMainCamera = CrossEngine::HashValue("MainCamera");

CrossEngine::CResMeshPtr ptrMesh;
CrossEngine::CResMaterialPtr ptrMaterial;
CrossEngine::CDrawableStaticMesh *pMesh = NULL;

void Create(void)
{
	ptrMaterial = MaterialManager()->LoadResource("PzVI-Tiger-P_Chassis.material", TRUE, TRUE);
	ptrMesh = MeshManager()->LoadResource("PzVI-Tiger-P_Body.mesh", TRUE, TRUE);

	pMesh = (CrossEngine::CDrawableStaticMesh *)RenderSystem()->AllocDrawable(CrossEngine::DRAWABLE_TYPE_STATIC_MESH);
	pMesh->SetMesh(ptrMesh);
	pMesh->SetMaterial(ptrMaterial);

	glm::mat4 mtxModel = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	pMesh->SetTransform(mtxModel);
}

void Destroy(void)
{
	RenderSystem()->FreeDrawable(pMesh);
	ptrMaterial.Release();
	ptrMesh.Release();
}

void Render(void)
{
	if (GfxSwapChain() == NULL) {
		return;
	}

	Camera(dwMainCamera)->SetViewport(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight());
	Camera(dwMainCamera)->SetPerspective(glm::radians(60.0f), 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
	Camera(dwMainCamera)->SetLookat(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera(dwMainCamera)->ClearRenderQueue();
	Camera(dwMainCamera)->AddRenderQueue(pMesh);

	RenderSystem()->Update();
}
