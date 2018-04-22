#include "stdafx.h"

CrossEngine::CResMeshPtr ptrMesh;
CrossEngine::CResMaterialPtr ptrMaterial;
CrossEngine::CDrawableStaticMesh *pMesh = NULL;

void Create(void)
{

}

void Destroy(void)
{

}

void Render(void)
{
	if (GfxSwapChain() == NULL) {
		return;
	}
}
