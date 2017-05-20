#include "stdafx.h"


CrossEngine::CVulkan *pVulkan = NULL;
CrossEngine::CVulkanDeviceGraphics *pDevice = NULL;
CrossEngine::CVulkanSwapchain *pSwapchain = NULL;

CrossEngine::CVulkanShaderPtr ptrShaderVertex;
CrossEngine::CVulkanShaderPtr ptrShaderFragment;
CrossEngine::CVulkanPipelineGraphicsPtr ptrPipeline;

CrossEngine::CVulkanTexturePtr ptrTexture;
CrossEngine::CVulkanIndexBufferPtr ptrIndexBuffer;
CrossEngine::CVulkanVertexBufferPtr ptrVertexBuffer;
CrossEngine::CVulkanUniformBufferPtr ptrUniformBuffer;
CrossEngine::CVulkanDescriptorSet *pDescriptorSet = NULL;

CrossEngine::CVulkanRenderTexturePtr ptrDepthTexture;
CrossEngine::CVulkanRenderPassPtr ptrRenderPass;
CrossEngine::CVulkanFrameBufferPtr ptrFrameBuffers[3];

CrossEngine::CVulkanSemaphorePtr ptrRenderDoneSemaphores[3];
CrossEngine::CVulkanCommandBuffer *pCommandBuffers[3] = { NULL };


void CreateRenderPass(void)
{
	ptrRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, {0.0f, 0.0f, 0.0f, 1.0f}, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, {1.0f, 0}, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void DestroyRenderPass(void)
{
	ptrRenderPass.SetNull();
}

void CreateFrameBuffer(void)
{
	ptrDepthTexture = pDevice->GetTextureManager()->AllocRenderTexture();
	ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight());

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		ptrFrameBuffers[indexView]->SetPresentAttachment(0, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageView(indexView));
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(1, ptrDepthTexture);
		ptrFrameBuffers[indexView]->Create(ptrRenderPass->GetRenderPass());
	}
}

void DestroyFrameBuffer(void)
{
	ptrDepthTexture.SetNull();

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView].SetNull();
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

	LoadShader("../Data/Shader/texture.vert", szSourceCode, sizeof(szSourceCode));
	ptrShaderVertex = pDevice->GetShaderManager()->AllocShader();
	ptrShaderVertex->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_vertex_shader);

	LoadShader("../Data/Shader/texture.frag", szSourceCode, sizeof(szSourceCode));
	ptrShaderFragment = pDevice->GetShaderManager()->AllocShader();
	ptrShaderFragment->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_fragment_shader);

	ptrPipeline = pDevice->GetPipelineManager()->AllocPipelineGraphics();
	ptrPipeline->SetVertexShader(ptrShaderVertex);
	ptrPipeline->SetFragmentShader(ptrShaderFragment);
	ptrPipeline->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	ptrPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	ptrPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	ptrPipeline->SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	ptrPipeline->Create(ptrRenderPass->GetRenderPass());
}

void DestroyPipeline(void)
{
	ptrPipeline.SetNull();
	ptrShaderVertex.SetNull();
	ptrShaderFragment.SetNull();
}

void CreateSynchronization(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrRenderDoneSemaphores[indexView] = pDevice->GetSemaphoreManager()->AllocSemaphore();
		ptrRenderDoneSemaphores[indexView]->Create();
	}
}

void DestroySynchronization(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrRenderDoneSemaphores[indexView].SetNull();
	}
}

void CreateTexture(void)
{
	ptrTexture = pDevice->GetTextureManager()->AllocTexture();
	ptrTexture->CreateTexture2D((gli::texture2d)gli::load("../Data/Texture/het_kanonschot_rgba8.ktx"), VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
}

void DestroyTexture(void)
{
	ptrTexture.SetNull();
}

void CreateBuffer(void)
{
	struct Vertex {
		float position[3];
		float texcoord[2];
	};

	std::vector<Vertex> vertexBuffer = {
		{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f } }
	};
	uint32_t vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);
	ptrVertexBuffer = pDevice->GetBufferManager()->AllocVertexBuffer();
	ptrVertexBuffer->Create(vertexBufferSize, 0, vertexBuffer.data());

	std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	ptrIndexBuffer = pDevice->GetBufferManager()->AllocIndexBuffer();
	ptrIndexBuffer->Create(indexBufferSize, 0, indexBuffer.data());

	ptrUniformBuffer = pDevice->GetBufferManager()->AllocUniformBuffer();
	ptrUniformBuffer->Create(sizeof(glm::mat4), 0, NULL);
}

void DestroyBuffer(void)
{
	ptrIndexBuffer.SetNull();
	ptrVertexBuffer.SetNull();
	ptrUniformBuffer.SetNull();
}

void CreateDescriptorSet(void)
{
	const CrossEngine::CVulkanDescriptorSetLayout* pDescriptorSetLayout = ptrPipeline->GetDescriptorSetLayout(0);
	pDescriptorSet = pDevice->GetDescriptorSetManager()->AllocDescriptorSet(0, pDescriptorSetLayout->GetLayout(), pDescriptorSetLayout->GetTypesUsedCount());
	pDescriptorSet->SetUniformBuffer(0, ptrUniformBuffer);
	pDescriptorSet->SetTexture(1, ptrTexture);
	pDescriptorSet->UpdateDescriptorSets();
}

void DestroyDescriptorSet(void)
{
	pDevice->GetDescriptorSetManager()->FreeDescriptorSet(pDescriptorSet);
}

void CreateCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pCommandBuffers[indexView] = pDevice->GetCommandBufferManager()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		pCommandBuffers[indexView]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			pCommandBuffers[indexView]->CmdBeginRenderPass(ptrFrameBuffers[indexView], ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
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

				pCommandBuffers[indexView]->CmdBindPipelineGraphics(ptrPipeline);
				{
					pCommandBuffers[indexView]->CmdBindIndexBuffer(ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
					pCommandBuffers[indexView]->CmdBindVertexBuffer(ptrVertexBuffer, 0);

					VkDescriptorSet set = pDescriptorSet->GetDescriptorSet();
					pCommandBuffers[indexView]->CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, ptrPipeline->GetPipelineLayout(), 0, 1, &set, 0, NULL);
					pCommandBuffers[indexView]->CmdDrawIndexed(6, 1, 0, 0, 1);
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
		pDevice->GetCommandBufferManager()->FreeCommandBuffer(pCommandBuffers[indexView]);
	}
}

void Create(HINSTANCE hInstance, HWND hWnd)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	char szCurPath[_MAX_PATH];
	GetCurrentDirectory(sizeof(szCurPath), szCurPath);

	char szCachePath[_MAX_PATH];
	sprintf(szCachePath, "%s/Cache", szCurPath);
	mkdir(szCachePath);

	pVulkan = SAFE_NEW CrossEngine::CVulkan(szCachePath);
	pVulkan->Create(hInstance, hWnd);
	pVulkan->CreateSwapchain(rcView.right - rcView.left, rcView.bottom - rcView.top, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	pDevice = pVulkan->GetGraphicsDevice();
	pSwapchain = pVulkan->GetSwapchain();

	CreateSynchronization();
	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateTexture();
	CreateBuffer();
	CreateDescriptorSet();
	CreateCommandBuffer();

	pDevice->DumpLog();
}

void Destroy(void)
{
	if (pVulkan) {
		pDevice->GetQueue()->WaitIdle();

		DestroyRenderPass();
		DestroyFrameBuffer();
		DestroyCommandBuffer();
		DestroySynchronization();

		DestroyPipeline();
		DestroyBuffer();
		DestroyTexture();
		DestroyDescriptorSet();

		pDevice->DumpLog();
		pVulkan->DestroySwapchain();
		pVulkan->Destroy();

		SAFE_DELETE(pVulkan);
	}
}

void Render(void)
{
	if (pSwapchain == NULL) {
		return;
	}

	static float angle = 30.0f; //angle += 0.05f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * pSwapchain->GetWidth() / pSwapchain->GetHeight(), 0.1f, 100.0f);
	glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mtxModel = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
	ptrUniformBuffer->UpdateData(sizeof(glm::mat4), 0, &mtxViewModelProjection);

	pSwapchain->AcquireNextImage(VK_NULL_HANDLE);
	{
		pDevice->GetQueue()->Submit(
			pCommandBuffers[pSwapchain->GetImageIndex()],
			pSwapchain->GetAcquireSemaphore(),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			ptrRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore());
	}
	pSwapchain->Present(ptrRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore());
}
