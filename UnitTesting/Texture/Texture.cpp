#include "stdafx.h"


CrossEngine::CGfxShaderPtr ptrShaderVertex;
CrossEngine::CGfxShaderPtr ptrShaderFragment;
CrossEngine::CGfxPipelineGraphicsPtr ptrPipeline;

CrossEngine::CGfxTexturePtr ptrTexture;
CrossEngine::CGfxIndexBufferPtr ptrIndexBuffer;
CrossEngine::CGfxVertexBufferPtr ptrVertexBuffer;
CrossEngine::CGfxUniformBufferPtr ptrUniformBuffer;
CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSet;

CrossEngine::CGfxRenderTexturePtr ptrDepthTexture;
CrossEngine::CGfxRenderPassPtr ptrRenderPass;
CrossEngine::CGfxFrameBufferPtr ptrFrameBuffers[3];

CrossEngine::CGfxCommandBufferPtr ptrCommandBuffers[3];


void CreateRenderPass(void)
{
	ptrRenderPass = GfxDevice()->NewRenderPass(2, 1);
	ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, {0.0f, 0.0f, 0.0f, 1.0f}, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetDepthStencilAttachment(1, VK_FORMAT_D24_UNORM_S8_UINT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, {1.0f, 0}, VK_SAMPLE_COUNT_1_BIT);
	ptrRenderPass->SetSubpassOutputColorReference(0, 0);
	ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	ptrRenderPass->Create();
}

void DestroyRenderPass(void)
{
	ptrRenderPass.Release();
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

void DestroyFrameBuffer(void)
{
	ptrDepthTexture.Release();

	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrFrameBuffers[indexView].Release();
	}
}

void CreatePipeline(void)
{
	static char szSourceCode[1024 * 1024];

	LoadShader("../Data/Engine/Shader/texture.vert", szSourceCode, sizeof(szSourceCode));
	ptrShaderVertex = GfxDevice()->NewShader();
	ptrShaderVertex->Create(szSourceCode, strlen(szSourceCode), VK_SHADER_STAGE_VERTEX_BIT);

	LoadShader("../Data/Engine/Shader/texture.frag", szSourceCode, sizeof(szSourceCode));
	ptrShaderFragment = GfxDevice()->NewShader();
	ptrShaderFragment->Create(szSourceCode, strlen(szSourceCode), VK_SHADER_STAGE_FRAGMENT_BIT);

	ptrPipeline = GfxDevice()->NewPipelineGraphics(1);
	ptrPipeline->SetVertexShader(ptrShaderVertex);
	ptrPipeline->SetFragmentShader(ptrShaderFragment);
	ptrPipeline->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	ptrPipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
	ptrPipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	ptrPipeline->SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	ptrPipeline->Create(ptrRenderPass->GetHandle(), 0);
}

void DestroyPipeline(void)
{
	ptrPipeline.Release();
	ptrShaderVertex.Release();
	ptrShaderFragment.Release();
}

void CreateTexture(void)
{
	ptrTexture = GfxDevice()->NewTexture();
	ptrTexture->CreateTexture2D((gli::texture2d)gli::load("../Data/Texture/het_kanonschot_rgba8.ktx"), VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
}

void DestroyTexture(void)
{
	ptrTexture.Release();
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
	ptrVertexBuffer = GfxDevice()->NewVertexBuffer();
	ptrVertexBuffer->Create(vertexBufferSize, vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_POSITION | CrossEngine::VERTEX_ATTRIBUTE_UV0);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
	ptrIndexBuffer->Create(indexBufferSize, indexBuffer.data(), FALSE);

	ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
	ptrUniformBuffer->Create(sizeof(glm::mat4), NULL, TRUE);
	ptrUniformBuffer->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));
}

void DestroyBuffer(void)
{
	ptrIndexBuffer.Release();
	ptrVertexBuffer.Release();
	ptrUniformBuffer.Release();
}

void CreateDescriptorSet(void)
{
	GfxDevice()->AllocDescriptorSetPool(thread_id());

	ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(thread_id(), 0, ptrPipeline);
	ptrDescriptorSet->SetUniformBuffer(0, ptrUniformBuffer);
	ptrDescriptorSet->SetTexture(1, ptrTexture);
	ptrDescriptorSet->UpdateDescriptorSets();
}

void DestroyDescriptorSet(void)
{
	ptrDescriptorSet.Release();
}

void CreateCommandBuffer(void)
{
	GfxDevice()->AllocCommandBufferPool(thread_id());

	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView] = GfxDevice()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
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

					ptrCommandBuffers[indexView]->CmdBindDescriptorSetGraphics(ptrDescriptorSet, ptrPipeline, 0, NULL);
					ptrCommandBuffers[indexView]->CmdDrawIndexed(6, 1, 0, 0, 0);
				}
			}
			ptrCommandBuffers[indexView]->CmdEndRenderPass();
		}
		ptrCommandBuffers[indexView]->End();
	}
}

void DestroyCommandBuffer(void)
{
	for (int indexView = 0; indexView < (int)GfxSwapChain()->GetImageCount(); indexView++) {
		ptrCommandBuffers[indexView].Release();
	}
}

void Create(void)
{
	char szCurPath[_MAX_PATH];
	GetCurrentDirectory(sizeof(szCurPath), szCurPath);

	char szCachePath[_MAX_PATH];
	sprintf(szCachePath, "%s/Cache", szCurPath);
	mkdir(szCachePath);

	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipeline();
	CreateTexture();
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
	DestroyTexture();
	DestroyDescriptorSet();

	GfxDevice()->DumpLog();
}

void Render(void)
{
	if (GfxSwapChain() == NULL) {
		return;
	}

	static float angle = 0.0f; angle += 0.01f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
	glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mtxModel = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
	ptrUniformBuffer->UpdateData(0, sizeof(glm::mat4), &mtxViewModelProjection);

	GfxSwapChain()->AcquireNextImage(VK_NULL_HANDLE);
	{
		GfxDevice()->GetGraphicsQueue()->Submit(ptrCommandBuffers[GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}
	GfxSwapChain()->Present();
}
