#include "stdafx.h"


CrossEngine::CResShaderPtr ptrShaderVertex;
CrossEngine::CResShaderPtr ptrShaderFragment;
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
	ptrRenderPass = GfxDevice()->NewRenderPass(2, 1);
	ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 1.0f, 0 }, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void CreateFrameBuffer(void)
{
	ptrDepthTexture = GfxDevice()->NewRenderTexture();
	ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);

	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView] = GfxDevice()->NewFrameBuffer(ptrRenderPass->GetAttachmentCount());
		ptrFrameBuffers[indexView]->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), GfxSwapChain()->GetImageHandle(indexView));
		ptrFrameBuffers[indexView]->SetDepthStencilAttachment(1, ptrDepthTexture);
		ptrFrameBuffers[indexView]->Create(ptrRenderPass->GetHandle());
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

	ptrShaderVertex = ShaderManager()->LoadResource(CrossEngine::HashValue("triangle.vert"));
	ptrShaderFragment = ShaderManager()->LoadResource(CrossEngine::HashValue("triangle.frag"));

	ptrPipeline = GfxDevice()->NewPipelineGraphics(1);
	ptrPipeline->SetVertexShader(ptrShaderVertex->GetShader());
	ptrPipeline->SetFragmentShader(ptrShaderFragment->GetShader());
	ptrPipeline->SetColorBlendAttachment(0, FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	ptrPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	ptrPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	ptrPipeline->Create(ptrRenderPass->GetHandle(), 0);
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
	ptrVertexBuffer = GfxDevice()->NewVertexBuffer();
	ptrVertexBuffer->Create(vertexBufferSize, vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_POSITION | CrossEngine::VERTEX_ATTRIBUTE_COLOR);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
	ptrIndexBuffer->Create(indexBufferSize, indexBuffer.data(), FALSE);

	ptrUniformBufferA = GfxDevice()->NewUniformBuffer();
	ptrUniformBufferA->Create(GfxSwapChain()->GetImageCount() * 0x100, NULL, TRUE);
	ptrUniformBufferA->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));

	ptrUniformBufferB = GfxDevice()->NewUniformBuffer();
	ptrUniformBufferB->Create(GfxSwapChain()->GetImageCount() * 0x100, NULL, TRUE);
	ptrUniformBufferB->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));
}

void CreateDescriptorSet(void)
{
	GfxDevice()->AllocDescriptorSetPool(thread_id());

	ptrDescriptorSetA = GfxDevice()->AllocDescriptorSet(thread_id(), 0, ptrPipeline);
	ptrDescriptorSetA->SetUniformBuffer(0, ptrUniformBufferA);
	ptrDescriptorSetA->UpdateDescriptorSets();

	ptrDescriptorSetB = GfxDevice()->AllocDescriptorSet(thread_id(), 0, ptrPipeline);
	ptrDescriptorSetB->SetUniformBuffer(0, ptrUniformBufferB);
	ptrDescriptorSetB->UpdateDescriptorSets();
}

void CreateCommandBuffer(void)
{
	GfxDevice()->AllocCommandBufferPool(thread_id());

	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView] = GfxDevice()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}
}

void CreateCommandBuffer(int indexView)
{
	ptrCommandBuffers[indexView]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
	{
		ptrCommandBuffers[indexView]->CmdBeginRenderPass(ptrFrameBuffers[indexView], ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
		{
			ptrCommandBuffers[indexView]->CmdSetViewport(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), 0.0f, 1.0f);
			ptrCommandBuffers[indexView]->CmdSetScissor(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight());

			ptrCommandBuffers[indexView]->CmdBindPipelineGraphics(ptrPipeline);
			{
				ptrCommandBuffers[indexView]->CmdBindVertexBuffer(ptrVertexBuffer, 0, 0);
				ptrCommandBuffers[indexView]->CmdBindIndexBuffer(ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

				ptrCommandBuffers[indexView]->CmdBindDescriptorSetGraphics(ptrDescriptorSetA, ptrPipeline);
				ptrCommandBuffers[indexView]->CmdDrawIndexed(3, 1, 0, 0, 0);

				ptrCommandBuffers[indexView]->CmdBindDescriptorSetGraphics(ptrDescriptorSetB, ptrPipeline);
				ptrCommandBuffers[indexView]->CmdDrawIndexed(3, 1, 0, 0, 0);
			}
		}
		ptrCommandBuffers[indexView]->CmdEndRenderPass();
	}
	ptrCommandBuffers[indexView]->End();
}

void DestroyRenderPass(void)
{
	ptrRenderPass.Release();
}

void DestroyFrameBuffer(void)
{
	ptrDepthTexture.Release();

	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
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
	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView].Release();
	}
}

void Create(void)
{
	char szCurrentPath[_MAX_PATH];
	GetCurrentDirectory(sizeof(szCurrentPath), szCurrentPath);

	char szCachePath[_MAX_PATH];
	sprintf(szCachePath, "%s/Cache", szCurrentPath);
	mkdir(szCachePath);

	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateBuffer();
	CreateDescriptorSet();
	CreateCommandBuffer();

	GfxDevice()->DumpLog();
}

void Destroy(void)
{
	DestroyRenderPass();
	DestroyFrameBuffer();
	DestroyCommandBuffer();

	DestroyPipeline();
	DestroyBuffer();
	DestroyDescriptorSet();

	GfxDevice()->DumpLog();
}

void Render(void)
{
	if (GfxSwapChain() == NULL) {
		return;
	}

	GfxSwapChain()->AcquireNextImage(ptrCommandBuffers[GfxSwapChain()->GetImageIndex()]->GetFence());
	{
		static float angle = 0.0f; angle += 0.05f;
		static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

		glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
		glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		{
			glm::mat4 mtxModel = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
			ptrDescriptorSetA->SetUniformBufferData(0, GfxSwapChain()->GetImageIndex() * 0x100, sizeof(glm::mat4), &mtxViewModelProjection);
		}
		{
			glm::mat4 mtxModel = glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
			ptrDescriptorSetB->SetUniformBufferData(0, GfxSwapChain()->GetImageIndex() * 0x100, sizeof(glm::mat4), &mtxViewModelProjection);
		}

		CreateCommandBuffer(GfxSwapChain()->GetImageIndex());
		GfxDevice()->GetGraphicsQueue()->Submit(ptrCommandBuffers[GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}
	GfxSwapChain()->Present();
}
