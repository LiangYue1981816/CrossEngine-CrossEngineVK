#include "stdafx.h"


CrossEngine::CRenderer *pRenderer = NULL;
CrossEngine::CRendererDeviceGraphics *pDevice = NULL;
CrossEngine::CRendererSwapchain *pSwapchain = NULL;

CrossEngine::CRendererShader *pShaderVertex = NULL;
CrossEngine::CRendererShader *pShaderFragment = NULL;
CrossEngine::CRendererPipelineGraphics *pPipeline = NULL;

CrossEngine::CRendererRenderPass *pRenderPass = NULL;
CrossEngine::CRendererFrameBuffer *pFrameBuffers[3] = { NULL };

CrossEngine::CRendererFence *pFences[3] = { NULL };
CrossEngine::CRendererCommandBuffer *pCommandBuffers[3] = { NULL };


void Create(HINSTANCE hInstance, HWND hWnd)
{
	static char szSourceCode[1024 * 1024];

	RECT rcView;
	GetClientRect(hWnd, &rcView);

	pRenderer = SAFE_NEW CrossEngine::CRenderer;
	pRenderer->Create(hInstance, hWnd);
	pRenderer->CreateSwapchain(rcView.right - rcView.left, rcView.bottom - rcView.top, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	pDevice = pRenderer->GetGraphicsDevice();
	pSwapchain = pRenderer->GetSwapchain();

	pRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	pRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE);
	pRenderPass->SetSubpassOutputColorReference(0, 0);
	pRenderPass->Create();

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFrameBuffers[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		pFrameBuffers[indexView]->SetAttachment(0, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageView(indexView));
		pFrameBuffers[indexView]->Create(pRenderPass->GetRenderPass());
	}

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFences[indexView] = pDevice->GetFenceManager()->AllocFence();
		pFences[indexView]->Create();

		pCommandBuffers[indexView] = pDevice->GetCommandBufferManager()->AllocCommandBuffer(0, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}

	LoadShader("../Data/Shader/triangle.vert", szSourceCode, sizeof(szSourceCode));
	pShaderVertex = pDevice->GetShaderManager()->AllocShader();
	pShaderVertex->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_vertex_shader);
	
	LoadShader("../Data/Shader/triangle.frag", szSourceCode, sizeof(szSourceCode));
	pShaderFragment = pDevice->GetShaderManager()->AllocShader();
	pShaderFragment->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_fragment_shader);

	pPipeline = pDevice->GetPipelineManager()->AllocPipelineGraphics(0);
	pPipeline->SetVertexShader(pShaderVertex->GetShaderModule(), pShaderVertex->GetModule());
	pPipeline->SetFragmentShader(pShaderFragment->GetShaderModule(), pShaderFragment->GetModule());
	pPipeline->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	pPipeline->Create(pRenderPass->GetRenderPass());

	pDevice->DumpLog();
}

void Destroy(void)
{
	if (pRenderer) {
		for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
			pDevice->GetCommandBufferManager()->FreeCommandBuffer(0, pCommandBuffers[indexView]);
			pDevice->GetFenceManager()->Free(pFences[indexView]);
		}

		pDevice->GetPipelineManager()->Free(pPipeline);
		pDevice->GetShaderManager()->Free(pShaderVertex);
		pDevice->GetShaderManager()->Free(pShaderFragment);

		pDevice->GetRenderPassManager()->Free(pRenderPass);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffers[0]);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffers[1]);
		pDevice->GetFrameBufferManager()->Free(pFrameBuffers[2]);

		pRenderer->DestroySwapchain();
		pRenderer->Destroy();

		SAFE_DELETE(pRenderer);
	}
}

void Render(void)
{
	if (pSwapchain) {
		//pFences[pSwapchain->GetImageIndex()]->Wait(UINT64_MAX);
		//pFences[pSwapchain->GetImageIndex()]->Reset();
		//pSwapchain->AcquireNextImage();
		//pSwapchain->Present();
	}
}
