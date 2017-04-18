#include "stdafx.h"


CrossEngine::CRenderer *pRenderer = NULL;
CrossEngine::CRendererDeviceGraphics *pDevice = NULL;
CrossEngine::CRendererSwapchain *pSwapchain = NULL;

CrossEngine::CRendererShader *pShaderVertex = NULL;
CrossEngine::CRendererShader *pShaderFragment = NULL;
CrossEngine::CRendererPipelineGraphics *pPipeline = NULL;

CrossEngine::CRendererIndexBuffer *pIndexBuffer = NULL;
CrossEngine::CRendererVertexBuffer *pVertexBuffer = NULL;
CrossEngine::CRendererUniformBuffer *pUniformBuffer = NULL;

CrossEngine::CRendererRenderTexture *pDepthTexture = NULL;
CrossEngine::CRendererRenderPass *pRenderPass = NULL;
CrossEngine::CRendererFrameBuffer *pFrameBuffers[3] = { NULL };

CrossEngine::CRendererFence *pFences[3] = { NULL };
CrossEngine::CRendererSemaphore *pRenderDoneSemaphores[3] = { NULL };
CrossEngine::CRendererCommandBuffer *pCommandBuffers[3] = { NULL };


void CreateRenderPass(void)
{
	pRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	pRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE);
	pRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE);
	pRenderPass->SetSubpassOutputColorReference(0, 0);
	pRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	pRenderPass->SetClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
	pRenderPass->SetClearDepthStencilValue(1.0f, 0);
	pRenderPass->Create();
}

void DestroyRenderPass(void)
{
	pRenderPass->Release();
}

void CreateFrameBuffer(void)
{
	pDepthTexture = pDevice->GetTextureManager()->AllocRenderTexture();
	pDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight());

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFrameBuffers[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		pFrameBuffers[indexView]->SetAttachment(0, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageView(indexView));
		pFrameBuffers[indexView]->SetAttachment(1, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pDepthTexture->GetImageView());
		pFrameBuffers[indexView]->Create(pRenderPass->GetRenderPass());
	}
}

void DestroyFrameBuffer(void)
{
	pDepthTexture->Release();

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFrameBuffers[indexView]->Release();
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

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
	pPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	pPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	pPipeline->Create(pRenderPass->GetRenderPass());
}

void DestroyPipeline(void)
{
	pPipeline->Release();
	pShaderVertex->Release();
	pShaderFragment->Release();
}

void CreateSynchronization(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFences[indexView] = pDevice->GetFenceManager()->AllocFence();
		pFences[indexView]->Create();

		pRenderDoneSemaphores[indexView] = pDevice->GetSemaphoreManager()->AllocSemaphore();
		pRenderDoneSemaphores[indexView]->Create();
	}
}

void DestroySynchronization(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFences[indexView]->Release();
		pRenderDoneSemaphores[indexView]->Release();
	}
}

void CreateBuffer(void)
{
	struct Vertex {
		float position[3];
		float color[3];
	};

	std::vector<Vertex> vertexBuffer = {
		{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
	};
	uint32_t vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);
	pVertexBuffer = pDevice->GetBufferManager()->AllocVertexBuffer();
	pVertexBuffer->Create(vertexBufferSize, 0, vertexBuffer.data());

	std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	pIndexBuffer = pDevice->GetBufferManager()->AllocIndexBuffer();
	pIndexBuffer->Create(indexBufferSize, 0, indexBuffer.data());

	pUniformBuffer = pDevice->GetBufferManager()->AllocUniformBuffer();
	pUniformBuffer->Create(sizeof(glm::mat4), 0, NULL);
	pPipeline->SetUniformBuffer("UBO", pUniformBuffer->GetBuffer(), 0, pUniformBuffer->GetBufferSize());
}

void DestroyBuffer()
{
	pIndexBuffer->Release();
	pVertexBuffer->Release();
	pUniformBuffer->Release();
}

void CreateCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pCommandBuffers[indexView] = pDevice->GetCommandBufferManager()->AllocCommandBuffer(0, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		pCommandBuffers[indexView]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.framebuffer = pFrameBuffers[indexView]->GetFrameBuffer();
			renderPassBeginInfo.renderPass = pRenderPass->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = pSwapchain->GetWidth();
			renderPassBeginInfo.renderArea.extent.height = pSwapchain->GetHeight();
			renderPassBeginInfo.clearValueCount = pRenderPass->GetClearValueCount();
			renderPassBeginInfo.pClearValues = pRenderPass->GetClearValues();
			pCommandBuffers[indexView]->CmdBeginRenderPass(&renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				VkViewport viewport = {};
				viewport.x = 0;
				viewport.y = 0;
				viewport.width = pSwapchain->GetWidth();
				viewport.height = pSwapchain->GetHeight();
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				pCommandBuffers[indexView]->CmdSetViewport(0, 1, &viewport);

				VkRect2D scissor = {};
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				scissor.extent.width = pSwapchain->GetWidth();
				scissor.extent.height = pSwapchain->GetHeight();
				pCommandBuffers[indexView]->CmdSetScissor(0, 1, &scissor);

				pCommandBuffers[indexView]->CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipelineLayout(), 0, 1, pPipeline->GetDescriptorSets().data(), 0, NULL);
				pCommandBuffers[indexView]->CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipeline());
				{
					VkDeviceSize offsets = 0;
					VkBuffer vkIndexBuffer = pIndexBuffer->GetBuffer();
					VkBuffer vkVertexBuffer = pVertexBuffer->GetBuffer();
					pCommandBuffers[indexView]->CmdBindIndexBuffer(vkIndexBuffer, offsets, VK_INDEX_TYPE_UINT32);
					pCommandBuffers[indexView]->CmdBindVertexBuffers(0, 1, &vkVertexBuffer, &offsets);
					pCommandBuffers[indexView]->CmdDrawIndexed(3, 1, 0, 0, 1);
				}
			}
			pCommandBuffers[indexView]->CmdEndRenderPass();
		}
		pCommandBuffers[indexView]->End();
	}
}

void DestroyCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pDevice->GetCommandBufferManager()->FreeCommandBuffer(0, pCommandBuffers[indexView]);
	}
}


void Create(HINSTANCE hInstance, HWND hWnd)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	pRenderer = SAFE_NEW CrossEngine::CRenderer;
	pRenderer->Create(hInstance, hWnd);
	pRenderer->CreateSwapchain(rcView.right - rcView.left, rcView.bottom - rcView.top, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	pDevice = pRenderer->GetGraphicsDevice();
	pSwapchain = pRenderer->GetSwapchain();

	CreateSynchronization();
	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateBuffer();
	CreateCommandBuffer();

	pDevice->DumpLog();
}

void Destroy(void)
{
	if (pRenderer) {
		pDevice->GetQueue()->WaitIdle();

		DestroyBuffer();
		DestroyPipeline();
		DestroyFrameBuffer();
		DestroyRenderPass();
		DestroyCommandBuffer();
		DestroySynchronization();

		pDevice->DumpLog();
		pRenderer->DestroySwapchain();
		pRenderer->Destroy();

		SAFE_DELETE(pRenderer);
	}
}

void Render(void)
{
	if (pSwapchain == NULL) {
		return;
	}

	static float angle = 0.0f; angle += 0.05f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));
	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * pSwapchain->GetWidth() / pSwapchain->GetHeight(), 0.1f, 100.0f);
	glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mtxModel = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
	pUniformBuffer->UpdateData(sizeof(glm::mat4), 0, &mtxViewModelProjection);

	pSwapchain->AcquireNextImage(VK_NULL_HANDLE);
	{
		pFences[pSwapchain->GetImageIndex()]->Wait(UINT64_MAX);
		pFences[pSwapchain->GetImageIndex()]->Reset();

		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkCommandBuffer commandBuffer = pCommandBuffers[pSwapchain->GetImageIndex()]->GetCommandBuffer();
		VkSemaphore acquireSemaphore = pSwapchain->GetAcquireSemaphore();
		VkSemaphore renderSemaphore = pRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore();
		pDevice->GetQueue()->Submit(1, &commandBuffer, &acquireSemaphore, &waitStageMask, &renderSemaphore, pFences[pSwapchain->GetImageIndex()]->GetFence());
	}
	pSwapchain->Present(pRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore());
}
