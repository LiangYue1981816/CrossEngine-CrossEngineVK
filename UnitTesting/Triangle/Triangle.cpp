#include "stdafx.h"


CrossEngine::CRenderer *pRenderer = NULL;
CrossEngine::CRendererDeviceGraphics *pDevice = NULL;
CrossEngine::CRendererRenderPass *pRenderPass = NULL;
CrossEngine::CRendererFrameBuffer *pFrameBuffer[3] = { NULL };
CrossEngine::CRendererShader *pShaderVertex = NULL;
CrossEngine::CRendererShader *pShaderFragment = NULL;
CrossEngine::CRendererPipelineGraphics *pPipeline = NULL;


void Create(HINSTANCE hInstance, HWND hWnd)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	CrossEngine::EnableLog(TRUE, "CrossEngineVK.log");
	pRenderer = SAFE_NEW CrossEngine::CRenderer;
	pRenderer->Create(hInstance, hWnd);
	pRenderer->CreateSwapchain(rcView.right - rcView.left, rcView.bottom - rcView.top, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	pDevice = pRenderer->GetGraphicsDevice();

	pRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	pRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE);
	pRenderPass->SetSubpassOutputColorReference(0, 0);
	pRenderPass->Create();

	for (int indexView = 0; indexView < (int)pRenderer->GetSwapchain()->GetImageCount(); indexView++) {
		pFrameBuffer[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		pFrameBuffer[indexView]->SetAttachment(0, pRenderer->GetSwapchain()->GetWidth(), pRenderer->GetSwapchain()->GetHeight(), pRenderer->GetSwapchain()->GetImageView(indexView));
		pFrameBuffer[indexView]->Create(pRenderPass->GetRenderPass());
	}

	pShaderVertex = pDevice->GetShaderManager()->AllocShader();
	pShaderFragment = pDevice->GetShaderManager()->AllocShader();
	pPipeline = pDevice->GetPipelineManager()->AllocPipelineGraphics(0);
}

void Destroy(void)
{
	if (pRenderer) {
		pDevice->GetPipelineManager()->Free(pPipeline);
		pDevice->GetShaderManager()->Free(pShaderVertex);
		pDevice->GetShaderManager()->Free(pShaderFragment);

		pDevice->GetRenderPassManager()->Free(pRenderPass);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffer[0]);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffer[1]);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffer[2]);

		pRenderer->DestroySwapchain();
		pRenderer->Destroy();

		SAFE_DELETE(pRenderer);
	}
}

void Render(void)
{

}
