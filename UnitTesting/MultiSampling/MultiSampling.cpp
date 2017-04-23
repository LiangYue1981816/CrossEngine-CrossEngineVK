#include "stdafx.h"


#define SAMPLE_COUNT VK_SAMPLE_COUNT_4_BIT

CrossEngine::CVulkan *pVulkan = NULL;
CrossEngine::CVulkanDeviceGraphics *pDevice = NULL;
CrossEngine::CVulkanSwapchain *pSwapchain = NULL;

CrossEngine::CVulkanShader *pShaderVertex = NULL;
CrossEngine::CVulkanShader *pShaderFragment = NULL;
CrossEngine::CVulkanPipelineGraphics *pPipeline = NULL;

CrossEngine::CVulkanTexture *pTexture = NULL;
CrossEngine::CVulkanIndexBuffer *pIndexBuffer = NULL;
CrossEngine::CVulkanVertexBuffer *pVertexBuffer = NULL;
CrossEngine::CVulkanUniformBuffer *pUniformBuffer = NULL;
CrossEngine::CVulkanDescriptorSet *pDescriptorSet = NULL;

int indexPresentAttachment = 0;
int indexDepthAttachment = 1;
int indexColorAttachmentMSAA = 2;
int indexDepthAttachmentMSAA = 3;
CrossEngine::CVulkanRenderTexture *pColorTextureMSAA = NULL;
CrossEngine::CVulkanRenderTexture *pDepthTextureMSAA = NULL;
CrossEngine::CVulkanRenderTexture *pDepthTexture = NULL;
CrossEngine::CVulkanRenderPass *pRenderPass = NULL;
CrossEngine::CVulkanFrameBuffer *pFrameBuffers[3] = { NULL };

CrossEngine::CVulkanFence *pFences[3] = { NULL };
CrossEngine::CVulkanSemaphore *pRenderDoneSemaphores[3] = { NULL };
CrossEngine::CVulkanCommandBuffer *pCommandBuffers[3] = { NULL };


void CreateRenderPass(void)
{
	pRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	pRenderPass->SetPresentAttachment(indexPresentAttachment, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, {0.0f, 0.0f, 0.0f, 1.0f});
	pRenderPass->SetDepthStencilAttachment(indexDepthAttachment, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 });
	pRenderPass->SetColorAttachment(indexColorAttachmentMSAA, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, SAMPLE_COUNT);
	pRenderPass->SetDepthStencilAttachment(indexDepthAttachmentMSAA, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 }, SAMPLE_COUNT);
	pRenderPass->SetSubpassResolveAttachment(0, indexPresentAttachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	pRenderPass->SetSubpassResolveAttachment(0, indexDepthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	pRenderPass->SetSubpassOutputColorReference(0, indexColorAttachmentMSAA);
	pRenderPass->SetSubpassOutputDepthStencilReference(0, indexDepthAttachmentMSAA);
	pRenderPass->Create();
}

void DestroyRenderPass(void)
{
	pRenderPass->Release();
}

void CreateFrameBuffer(void)
{
	pColorTextureMSAA = pDevice->GetTextureManager()->AllocRenderTexture();
	pColorTextureMSAA->CreateColorTarget(VK_FORMAT_B8G8R8A8_UNORM, pSwapchain->GetWidth(), pSwapchain->GetHeight(), SAMPLE_COUNT);

	pDepthTextureMSAA = pDevice->GetTextureManager()->AllocRenderTexture();
	pDepthTextureMSAA->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight(), SAMPLE_COUNT);

	pDepthTexture = pDevice->GetTextureManager()->AllocRenderTexture();
	pDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight());

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFrameBuffers[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		pFrameBuffers[indexView]->SetAttachment(indexPresentAttachment, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageView(indexView));
		pFrameBuffers[indexView]->SetAttachment(indexDepthAttachment, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pDepthTexture->GetImageView());
		pFrameBuffers[indexView]->SetAttachment(indexColorAttachmentMSAA, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pColorTextureMSAA->GetImageView());
		pFrameBuffers[indexView]->SetAttachment(indexDepthAttachmentMSAA, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pDepthTextureMSAA->GetImageView());
		pFrameBuffers[indexView]->Create(pRenderPass->GetRenderPass());
	}
}

void DestroyFrameBuffer(void)
{
	pColorTextureMSAA->Release();
	pDepthTextureMSAA->Release();
	pDepthTexture->Release();

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pFrameBuffers[indexView]->Release();
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

	LoadShader("../Data/Shader/texture.vert", szSourceCode, sizeof(szSourceCode));
	pShaderVertex = pDevice->GetShaderManager()->AllocShader();
	pShaderVertex->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_vertex_shader);

	LoadShader("../Data/Shader/texture.frag", szSourceCode, sizeof(szSourceCode));
	pShaderFragment = pDevice->GetShaderManager()->AllocShader();
	pShaderFragment->Create(szSourceCode, strlen(szSourceCode), shaderc_glsl_fragment_shader);

	pPipeline = pDevice->GetPipelineManager()->AllocPipelineGraphics();
	pPipeline->SetVertexShader(pShaderVertex->GetShaderModule(), pShaderVertex->GetModule());
	pPipeline->SetFragmentShader(pShaderFragment->GetShaderModule(), pShaderFragment->GetModule());
	pPipeline->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	pPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	pPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	pPipeline->SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	pPipeline->SetSampleCounts(SAMPLE_COUNT);
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

void CreateTexture(void)
{
	pTexture = pDevice->GetTextureManager()->AllocTexture();
	pTexture->CreateTexture2D((gli::texture2d)gli::load("../Data/Texture/het_kanonschot_rgba8.ktx"), VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
}

void DestroyTexture(void)
{
	pTexture->Release();
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
	pVertexBuffer = pDevice->GetBufferManager()->AllocVertexBuffer();
	pVertexBuffer->Create(vertexBufferSize, 0, vertexBuffer.data());

	std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	pIndexBuffer = pDevice->GetBufferManager()->AllocIndexBuffer();
	pIndexBuffer->Create(indexBufferSize, 0, indexBuffer.data());

	pUniformBuffer = pDevice->GetBufferManager()->AllocUniformBuffer();
	pUniformBuffer->Create(sizeof(glm::mat4), 0, NULL);
}

void DestroyBuffer(void)
{
	pIndexBuffer->Release();
	pVertexBuffer->Release();
	pUniformBuffer->Release();
}

void CreateDescriptorSet(void)
{
	const CrossEngine::CVulkanDescriptorSetLayout *pDescriptorSetLayout = pPipeline->GetDescriptorSetLayout(0);
	pDescriptorSet = pDevice->GetDescriptorSetManager()->AllocDescriptorSet(0, pDescriptorSetLayout->GetLayout(), pDescriptorSetLayout->GetTypesUsedCount());
	pDescriptorSet->WriteDescriptorSet(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, pUniformBuffer->GetDescriptorBufferInfo());
	pDescriptorSet->WriteDescriptorSet(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, pTexture->GetDescriptorImageInfo());
	pDescriptorSet->UpdateDescriptorSets();
}

void DestroyDescriptorSet(void)
{
	pDevice->GetDescriptorSetManager()->FreeDescriptorSet(0, pDescriptorSet);
}

void CreateCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		pCommandBuffers[indexView] = pDevice->GetCommandBufferManager()->AllocCommandBuffer(0, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		pCommandBuffers[indexView]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			std::vector<VkClearValue> clearValues = pRenderPass->GetClearValues();
			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.framebuffer = pFrameBuffers[indexView]->GetFrameBuffer();
			renderPassBeginInfo.renderPass = pRenderPass->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = pSwapchain->GetWidth();
			renderPassBeginInfo.renderArea.extent.height = pSwapchain->GetHeight();
			renderPassBeginInfo.clearValueCount = clearValues.size();
			renderPassBeginInfo.pClearValues = clearValues.data();
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

				pCommandBuffers[indexView]->CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipeline());
				{
					VkDescriptorSet set = pDescriptorSet->GetDescriptorSet();
					pCommandBuffers[indexView]->CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipelineLayout(), 0, 1, &set, 0, NULL);

					VkDeviceSize offsets = 0;
					VkBuffer vkIndexBuffer = pIndexBuffer->GetBuffer();
					VkBuffer vkVertexBuffer = pVertexBuffer->GetBuffer();
					pCommandBuffers[indexView]->CmdBindIndexBuffer(vkIndexBuffer, offsets, VK_INDEX_TYPE_UINT32);
					pCommandBuffers[indexView]->CmdBindVertexBuffers(0, 1, &vkVertexBuffer, &offsets);

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
		pDevice->GetCommandBufferManager()->FreeCommandBuffer(0, pCommandBuffers[indexView]);
	}
}

void Create(HINSTANCE hInstance, HWND hWnd)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	pVulkan = SAFE_NEW CrossEngine::CVulkan;
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

		DestroyBuffer();
		DestroyTexture();
		DestroyDescriptorSet();
		DestroyPipeline();
		DestroyFrameBuffer();
		DestroyRenderPass();
		DestroyCommandBuffer();
		DestroySynchronization();

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

	static float angle = 30.0f; angle += 0.05f;
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

		pDevice->GetQueue()->Submit(
			pCommandBuffers[pSwapchain->GetImageIndex()]->GetCommandBuffer(), 
			pSwapchain->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			pRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore(),
			pFences[pSwapchain->GetImageIndex()]->GetFence());
	}
	pSwapchain->Present(pRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore());
}
