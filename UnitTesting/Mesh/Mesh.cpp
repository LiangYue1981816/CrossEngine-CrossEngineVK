#include "stdafx.h"

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
}
