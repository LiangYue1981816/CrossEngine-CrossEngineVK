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
	CrossEngine::CVulkanRenderPass *pVulkanRenderPass = (CrossEngine::CVulkanRenderPass *)((CrossEngine::CGfxRenderPass *)ptrRenderPass);

	ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CreateFrameBuffer(void)
{
	ptrDepthTexture = pDevice->NewRenderTexture();
	ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight(), VK_SAMPLE_COUNT_1_BIT);

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView] = pDevice->NewFrameBuffer();
		ptrFrameBuffers[indexView]->SetPresentAttachment(0, pSwapchain->GetWidth(), pSwapchain->GetHeight(), (VkImageView)pSwapchain->GetImageHandle(indexView));
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(1, ptrDepthTexture);
		ptrFrameBuffers[indexView]->Create(ptrRenderPass->GetHandle());
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

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
	ptrVertexBuffer->Create(vertexBufferSize, vertexBuffer.data(), FALSE);

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
	CrossEngine::CVulkanSwapchain *pVkSwapchain = (CrossEngine::CVulkanSwapchain *)pSwapchain;
	CrossEngine::CVulkanPipelineGraphics *pVkPipeline = (CrossEngine::CVulkanPipelineGraphics *)((CrossEngine::CGfxPipelineGraphics *)ptrPipeline);

	for (int indexView = 0; indexView < (int)pVkSwapchain->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView] = pDevice->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		CrossEngine::CVulkanCommandBuffer *pVkCommandBuffer = (CrossEngine::CVulkanCommandBuffer *)((CrossEngine::CGfxCommandBuffer *)ptrCommandBuffers[indexView]);
		pVkCommandBuffer->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			pVkCommandBuffer->CmdBeginRenderPass(ptrFrameBuffers[indexView], ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
			{
				pVkCommandBuffer->CmdSetViewport(0, 0, pVkSwapchain->GetWidth(), pVkSwapchain->GetHeight(), 0.0f, 1.0f);
				pVkCommandBuffer->CmdSetScissor(0, 0, pVkSwapchain->GetWidth(), pVkSwapchain->GetHeight());

				pVkCommandBuffer->CmdBindPipelineGraphics(ptrPipeline);
				{
					pVkCommandBuffer->CmdBindIndexBuffer(ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
					pVkCommandBuffer->CmdBindVertexBuffer(ptrVertexBuffer, 0);

					pVkCommandBuffer->CmdBindDescriptorSetGraphics(ptrDescriptorSetA, pVkPipeline->GetPipelineLayout());
					pVkCommandBuffer->CmdDrawIndexed(3, 1, 0, 0, 1);

					pVkCommandBuffer->CmdBindDescriptorSetGraphics(ptrDescriptorSetB, pVkPipeline->GetPipelineLayout());
					pVkCommandBuffer->CmdDrawIndexed(3, 1, 0, 0, 1);
				}
			}
			pVkCommandBuffer->CmdEndRenderPass();
		}
		pVkCommandBuffer->End();
	}
}

void DestroyRenderPass(void)
{
	ptrRenderPass.Release();
}

void DestroyFrameBuffer(void)
{
	CrossEngine::CVulkanSwapchain *pVulkanSwapchain = (CrossEngine::CVulkanSwapchain *)((CrossEngine::CGfxSwapchain *)pSwapchain);

	ptrDepthTexture.Release();

	for (int indexView = 0; indexView < (int)pVulkanSwapchain->GetImageCount(); indexView++) {
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
	CrossEngine::CVulkanSwapchain *pVkSwapchain = (CrossEngine::CVulkanSwapchain *)pSwapchain;

	for (int indexView = 0; indexView < (int)pVkSwapchain->GetImageCount(); indexView++) {
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

	pGfxInstance = SAFE_NEW CrossEngine::CVulkanInstance(szCachePath);
	pGfxInstance->Create(hInstance, hWnd, hDC, rcView.right - rcView.left, rcView.bottom - rcView.top);
	pDevice = pGfxInstance->GetDevice();
	pSwapchain = pGfxInstance->GetSwapchain();

	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateBuffer();
	CreateDescriptorSet();
	CreateCommandBuffer();
}

void Destroy(void)
{
	if (pGfxInstance) {
		((CrossEngine::CVulkanDevice *)pDevice)->GetQueue()->WaitIdle();

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

	CrossEngine::CVulkanSwapchain *pVulkanSwapchain = (CrossEngine::CVulkanSwapchain *)((CrossEngine::CGfxSwapchain *)pSwapchain);

	static float angle = 0.0f; angle += 0.05f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * pVulkanSwapchain->GetWidth() / pVulkanSwapchain->GetHeight(), 0.1f, 100.0f);
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

	pVulkanSwapchain->AcquireNextImage(VK_NULL_HANDLE);
	{
		((CrossEngine::CVulkanDevice *)pDevice)->GetQueue()->Submit(ptrCommandBuffers[pVulkanSwapchain->GetImageIndex()], pVulkanSwapchain->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, pVulkanSwapchain->GetRenderDoneSemaphore());
	}
	pVulkanSwapchain->Present();
}
