#include "stdafx.h"


#define SAMPLE_COUNT VK_SAMPLE_COUNT_4_BIT

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

int indexPresentAttachment = 0;
int indexDepthAttachment = 1;
int indexColorAttachmentMSAA = 2;
int indexDepthAttachmentMSAA = 3;
CrossEngine::CVulkanRenderTexturePtr ptrColorTextureMSAA;
CrossEngine::CVulkanRenderTexturePtr ptrDepthTextureMSAA;
CrossEngine::CVulkanRenderTexturePtr ptrDepthTexture;
CrossEngine::CVulkanRenderPassPtr ptrRenderPass;
CrossEngine::CVulkanFrameBufferPtr ptrFrameBuffers[3];

CrossEngine::CVulkanFencePtr ptrFences[3];
CrossEngine::CVulkanSemaphorePtr ptrRenderDoneSemaphores[3];
CrossEngine::CVulkanCommandBuffer *pCommandBuffers[3] = { NULL };


void CreateRenderPass(void)
{
	ptrRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();
	ptrRenderPass->SetPresentAttachment(indexPresentAttachment, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, {0.0f, 0.0f, 0.0f, 1.0f});
	ptrRenderPass->SetDepthStencilAttachment(indexDepthAttachment, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 });
	ptrRenderPass->SetColorAttachment(indexColorAttachmentMSAA, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, SAMPLE_COUNT);
	ptrRenderPass->SetDepthStencilAttachment(indexDepthAttachmentMSAA, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 }, SAMPLE_COUNT);
	ptrRenderPass->SetSubpassResolveAttachment(0, indexPresentAttachment, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	ptrRenderPass->SetSubpassResolveAttachment(0, indexDepthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	ptrRenderPass->SetSubpassOutputColorReference(0, indexColorAttachmentMSAA);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, indexDepthAttachmentMSAA);
	ptrRenderPass->Create();
}

void DestroyRenderPass(void)
{
	ptrRenderPass.SetNull();
}

void CreateFrameBuffer(void)
{
	ptrColorTextureMSAA = pDevice->GetTextureManager()->AllocRenderTexture();
	ptrColorTextureMSAA->CreateColorTarget(VK_FORMAT_B8G8R8A8_UNORM, pSwapchain->GetWidth(), pSwapchain->GetHeight(), SAMPLE_COUNT);

	ptrDepthTextureMSAA = pDevice->GetTextureManager()->AllocRenderTexture();
	ptrDepthTextureMSAA->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight(), SAMPLE_COUNT);

	ptrDepthTexture = pDevice->GetTextureManager()->AllocRenderTexture();
	ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, pSwapchain->GetWidth(), pSwapchain->GetHeight());

	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		ptrFrameBuffers[indexView]->SetPresentAttachment(indexPresentAttachment, pSwapchain->GetWidth(), pSwapchain->GetHeight(), pSwapchain->GetImageView(indexView));
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(indexDepthAttachment, ptrDepthTexture);
		ptrFrameBuffers[indexView]->SetColorAttachment(indexColorAttachmentMSAA, ptrColorTextureMSAA);
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(indexDepthAttachmentMSAA, ptrDepthTextureMSAA);
		ptrFrameBuffers[indexView]->Create(ptrRenderPass->GetRenderPass());
	}
}

void DestroyFrameBuffer(void)
{
	ptrColorTextureMSAA.SetNull();
	ptrDepthTextureMSAA.SetNull();
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
	ptrPipeline->SetSampleCounts(SAMPLE_COUNT);
//	ptrPipeline->SetSampleShading(VK_TRUE, 0.25f);
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
		ptrFences[indexView] = pDevice->GetFenceManager()->AllocFence();
		ptrFences[indexView]->Create();

		ptrRenderDoneSemaphores[indexView] = pDevice->GetSemaphoreManager()->AllocSemaphore();
		ptrRenderDoneSemaphores[indexView]->Create();
	}
}

void DestroySynchronization(void)
{
	for (int indexView = 0; indexView < (int)pSwapchain->GetImageCount(); indexView++) {
		ptrFences[indexView].SetNull();
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
	pDescriptorSet->WriteDescriptorSet(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ptrUniformBuffer->GetDescriptorBufferInfo());
	pDescriptorSet->WriteDescriptorSet(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ptrTexture->GetDescriptorImageInfo());
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
			std::vector<VkClearValue> clearValues = ptrRenderPass->GetClearValues();
			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.framebuffer = ptrFrameBuffers[indexView]->GetFrameBuffer();
			renderPassBeginInfo.renderPass = ptrRenderPass->GetRenderPass();
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

				pCommandBuffers[indexView]->CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, ptrPipeline->GetPipeline());
				{
					VkDescriptorSet set = pDescriptorSet->GetDescriptorSet();
					pCommandBuffers[indexView]->CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, ptrPipeline->GetPipelineLayout(), 0, 1, &set, 0, NULL);

					VkDeviceSize offsets = 0;
					VkBuffer vkIndexBuffer = ptrIndexBuffer->GetBuffer();
					VkBuffer vkVertexBuffer = ptrVertexBuffer->GetBuffer();
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

	static float angle = 30.0f; //angle += 0.05f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * pSwapchain->GetWidth() / pSwapchain->GetHeight(), 0.1f, 100.0f);
	glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mtxModel = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
	ptrUniformBuffer->UpdateData(sizeof(glm::mat4), 0, &mtxViewModelProjection);

	pSwapchain->AcquireNextImage(VK_NULL_HANDLE);
	{
		ptrFences[pSwapchain->GetImageIndex()]->Wait(UINT64_MAX);
		ptrFences[pSwapchain->GetImageIndex()]->Reset();

		pDevice->GetQueue()->Submit(
			pCommandBuffers[pSwapchain->GetImageIndex()]->GetCommandBuffer(), 
			pSwapchain->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			ptrRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore(),
			ptrFences[pSwapchain->GetImageIndex()]->GetFence());
	}
	pSwapchain->Present(ptrRenderDoneSemaphores[pSwapchain->GetImageIndex()]->GetSemaphore());
}
