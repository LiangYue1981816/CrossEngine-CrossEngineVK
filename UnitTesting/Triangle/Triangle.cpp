#include "stdafx.h"


CrossEngine::CGfxInstance *pGfxInstance = NULL;
CrossEngine::CGfxDevice *pDevice = NULL;
CrossEngine::CGfxSwapchain *pSwapchain = NULL;

CrossEngine::CGfxShaderPtr ptrShaderVertex;
CrossEngine::CGfxShaderPtr ptrShaderFragment;
CrossEngine::CGfxPipelineGraphicsPtr ptrPipeline;

CrossEngine::CGfxIndexBufferPtr ptrIndexBuffer;
CrossEngine::CGfxVertexBufferPtr ptrVertexBuffer;
CrossEngine::CGfxUniformBufferPtr ptrUniformBufferA;
CrossEngine::CGfxUniformBufferPtr ptrUniformBufferB;
CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSetA;
CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSetB;

CrossEngine::CGfxRenderTexturePtr ptrDepthTexture;
CrossEngine::CGfxRenderPassPtr ptrRenderPass;
CrossEngine::CGfxFrameBufferPtr ptrFrameBuffers[3];

CrossEngine::CGfxCommandBufferPtr ptrCommandBuffers[3];


void CreateRenderPass(void)
{
	ptrRenderPass = pDevice->NewRenderPass();
	ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CreateFrameBuffer(void)
{
	ptrDepthTexture = pDevice->NewRenderTexture();
	ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView] = pDevice->NewFrameBuffer();
		ptrFrameBuffers[indexView]->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageHandle(indexView));
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(1, ptrDepthTexture);
		ptrFrameBuffers[indexView]->Create(ptrRenderPass->GetHandle());
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

	pDevice->SetShaderCachePath("../Data/ShaderCache/");
	pDevice->AddShaderIncludePath("../Data/Shader/");

	LoadShader("../Data/Shader/triangle.vert", szSourceCode, sizeof(szSourceCode));
	ptrShaderVertex = pDevice->NewShader();
	ptrShaderVertex->Create(szSourceCode, strlen(szSourceCode), VK_SHADER_STAGE_VERTEX_BIT);

	LoadShader("../Data/Shader/triangle.frag", szSourceCode, sizeof(szSourceCode));
	ptrShaderFragment = pDevice->NewShader();
	ptrShaderFragment->Create(szSourceCode, strlen(szSourceCode), VK_SHADER_STAGE_FRAGMENT_BIT);

	ptrPipeline = pDevice->NewPipelineGraphics();
	ptrPipeline->SetVertexShader(ptrShaderVertex);
	ptrPipeline->SetFragmentShader(ptrShaderFragment);
	ptrPipeline->SetColorBlendAttachment(0, FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	ptrPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	ptrPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	ptrPipeline->Create(ptrRenderPass->GetHandle());
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
	ptrVertexBuffer = pDevice->NewVertexBuffer();
	ptrVertexBuffer->Create(vertexBufferSize, vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_FLAG_POSITION | CrossEngine::VERTEX_ATTRIBUTE_FLAG_COLOR);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	ptrIndexBuffer = pDevice->NewIndexBuffer();
	ptrIndexBuffer->Create(indexBufferSize, indexBuffer.data(), FALSE);

	ptrUniformBufferA = pDevice->NewUniformBuffer();
	ptrUniformBufferA->Create(sizeof(glm::mat4), NULL, TRUE);
	ptrUniformBufferA->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));

	ptrUniformBufferB = pDevice->NewUniformBuffer();
	ptrUniformBufferB->Create(sizeof(glm::mat4), NULL, TRUE);
	ptrUniformBufferB->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));
}

void CreateDescriptorSet(void)
{
	ptrDescriptorSetA = pDevice->AllocDescriptorSet(0, 0, ptrPipeline);
	ptrDescriptorSetA->SetUniformBuffer(0, ptrUniformBufferA);
	ptrDescriptorSetA->UpdateDescriptorSets();

	ptrDescriptorSetB = pDevice->AllocDescriptorSet(0, 0, ptrPipeline);
	ptrDescriptorSetB->SetUniformBuffer(0, ptrUniformBufferB);
	ptrDescriptorSetB->UpdateDescriptorSets();
}

void CreateCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView] = pDevice->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		ptrCommandBuffers[indexView]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			ptrCommandBuffers[indexView]->CmdBeginRenderPass(ptrFrameBuffers[indexView], ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
			{
				ptrCommandBuffers[indexView]->CmdSetViewport(0, 0, pSwapchain->GetWidth(), pSwapchain->GetHeight(), 0.0f, 1.0f);
				ptrCommandBuffers[indexView]->CmdSetScissor(0, 0, pSwapchain->GetWidth(), pSwapchain->GetHeight());

				ptrCommandBuffers[indexView]->CmdBindPipelineGraphics(ptrPipeline);
				{
					ptrCommandBuffers[indexView]->CmdBindVertexBuffer(ptrVertexBuffer, 0);
					ptrCommandBuffers[indexView]->CmdBindIndexBuffer(ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

					ptrCommandBuffers[indexView]->CmdBindDescriptorSetGraphics(ptrDescriptorSetA, ptrPipeline);
					ptrCommandBuffers[indexView]->CmdDrawIndexed(3, 1, 0, 0, 1);

					ptrCommandBuffers[indexView]->CmdBindDescriptorSetGraphics(ptrDescriptorSetB, ptrPipeline);
					ptrCommandBuffers[indexView]->CmdDrawIndexed(3, 1, 0, 0, 1);
				}
			}
			ptrCommandBuffers[indexView]->CmdEndRenderPass();
		}
		ptrCommandBuffers[indexView]->End();
	}
}

void DestroyRenderPass(void)
{
	ptrRenderPass.Release();
}

void DestroyFrameBuffer(void)
{
	ptrDepthTexture.Release();

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView].Release();
	}
}

void DestroyPipeline(void)
{
	ptrShaderVertex.Release();
	ptrShaderFragment.Release();
	ptrPipeline.Release();
}

void DestroyBuffer(void)
{
	ptrIndexBuffer.Release();
	ptrVertexBuffer.Release();
	ptrUniformBufferA.Release();
	ptrUniformBufferB.Release();
}

void DestroyDescriptorSet(void)
{
	ptrDescriptorSetA.Release();
	ptrDescriptorSetB.Release();
}

void DestroyCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView].Release();
	}
}

void Create(HINSTANCE hInstance, HWND hWnd, HDC hDC)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	char szCurrentPath[_MAX_PATH];
	GetCurrentDirectory(sizeof(szCurrentPath), szCurrentPath);

	char szCachePath[_MAX_PATH];
	sprintf(szCachePath, "%s/Cache", szCurrentPath);
	mkdir(szCachePath);

//	pGfxInstance = SAFE_NEW CrossEngine::CGLES3Instance;
	pGfxInstance = SAFE_NEW CrossEngine::CVulkanInstance;
	pGfxInstance->Create(hInstance, hWnd, hDC, rcView.right - rcView.left, rcView.bottom - rcView.top);
	pDevice = pGfxInstance->GetDevice();
	pSwapchain = pGfxInstance->GetSwapchain();

	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateBuffer();
	CreateDescriptorSet();
	CreateCommandBuffer();

	pDevice->DumpLog();
}

void Destroy(void)
{
	if (pGfxInstance) {
		DestroyRenderPass();
		DestroyFrameBuffer();
		DestroyCommandBuffer();

		DestroyPipeline();
		DestroyBuffer();
		DestroyDescriptorSet();

		pDevice->DumpLog();
		pGfxInstance->Destroy();

		SAFE_DELETE(pGfxInstance);
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
	{
		glm::mat4 mtxModel = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
		ptrUniformBufferA->UpdateData(0, sizeof(glm::mat4), &mtxViewModelProjection);
	}
	{
		glm::mat4 mtxModel = glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
		ptrUniformBufferB->UpdateData(0, sizeof(glm::mat4), &mtxViewModelProjection);
	}

	pSwapchain->AcquireNextImage(VK_NULL_HANDLE);
	{
		pDevice->Submit(ptrCommandBuffers[pSwapchain->GetImageIndex()], pSwapchain->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, pSwapchain->GetRenderDoneSemaphore());
	}
	pSwapchain->Present();
}
