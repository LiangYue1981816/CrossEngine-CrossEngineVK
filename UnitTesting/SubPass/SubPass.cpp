#include "stdafx.h"

struct {
	CrossEngine::CResShaderPtr ptrShaderVertex;
	CrossEngine::CResShaderPtr ptrShaderFragment;
	CrossEngine::CGfxPipelineGraphicsPtr ptrGraphics;
	CrossEngine::CGfxTexturePtr ptrTexture;
	CrossEngine::CGfxIndexBufferPtr ptrIndexBuffer;
	CrossEngine::CGfxVertexBufferPtr ptrVertexBuffer;
	CrossEngine::CGfxUniformBufferPtr ptrUniformBuffer;
	CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSet;
} Mesh;

struct {
	CrossEngine::CResShaderPtr ptrShaderVertex;
	CrossEngine::CResShaderPtr ptrShaderFragment;
	CrossEngine::CGfxPipelineGraphicsPtr ptrGraphics;
} Screen;

struct {
	CrossEngine::CGfxRenderTexturePtr ptrColorTexture;
	CrossEngine::CGfxRenderPassPtr ptrRenderPass;
	CrossEngine::CGfxFrameBufferPtr ptrFrameBuffers[3];
	CrossEngine::CGfxCommandBufferPtr ptrCommandBuffers[3];
} Renderer;

void CreateRenderer(void)
{
	Renderer.ptrRenderPass = GfxDevice()->NewRenderPass();
	Renderer.ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	Renderer.ptrRenderPass->SetColorAttachment(1, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 0.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	Renderer.ptrRenderPass->SetSubpassOutputColorReference(0, 1);
	Renderer.ptrRenderPass->SetSubpassInputReference(1, 1);
	Renderer.ptrRenderPass->SetSubpassOutputColorReference(1, 0);
	Renderer.ptrRenderPass->SetSubpassDependency(0, 0, 1);
	Renderer.ptrRenderPass->Create();

	Renderer.ptrColorTexture = GfxDevice()->NewRenderTexture();
	Renderer.ptrColorTexture->CreateColorTarget(VK_FORMAT_B8G8R8A8_UNORM, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);

	for (int index = 0; index < 3; index++) {
		Renderer.ptrFrameBuffers[index] = GfxDevice()->NewFrameBuffer();
		Renderer.ptrFrameBuffers[index]->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), GfxSwapChain()->GetImageHandle(index));
		Renderer.ptrFrameBuffers[index]->SetColorAttachment(1, Renderer.ptrColorTexture);
		Renderer.ptrFrameBuffers[index]->Create(Renderer.ptrRenderPass->GetHandle());
	}
}

void DestroyRenderer(void)
{
	for (int index = 0; index < 3; index++) {
		Renderer.ptrFrameBuffers[index].Release();
	}

	Renderer.ptrColorTexture.Release();
	Renderer.ptrRenderPass.Release();
}

void CreateMesh(void)
{
	Mesh.ptrShaderVertex = ShaderManager()->LoadResource("texture.vert", TRUE);
	Mesh.ptrShaderFragment = ShaderManager()->LoadResource("texture.frag", TRUE);

	Mesh.ptrGraphics = GfxDevice()->NewPipelineGraphics();
	Mesh.ptrGraphics->SetVertexShader(Mesh.ptrShaderVertex->GetGfxShader());
	Mesh.ptrGraphics->SetFragmentShader(Mesh.ptrShaderFragment->GetGfxShader());
	Mesh.ptrGraphics->SetDepthTest(FALSE, FALSE, VK_COMPARE_OP_ALWAYS);
	Mesh.ptrGraphics->SetCullMode(VK_CULL_MODE_BACK_BIT);
	Mesh.ptrGraphics->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	Mesh.ptrGraphics->SetColorBlendAttachment(0, FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	Mesh.ptrGraphics->Create(Renderer.ptrRenderPass->GetHandle(), 0);

	Mesh.ptrTexture = GfxDevice()->NewTexture();
	Mesh.ptrTexture->CreateTexture2D((gli::texture2d)gli::load("../Data/Texture/het_kanonschot_rgba8.ktx"), VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

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
	Mesh.ptrVertexBuffer = GfxDevice()->NewVertexBuffer();
	Mesh.ptrVertexBuffer->Create(vertexBuffer.size() * sizeof(Vertex), vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_POSITION | CrossEngine::VERTEX_ATTRIBUTE_TEXCOORD0);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
	Mesh.ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
	Mesh.ptrIndexBuffer->Create(indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), FALSE);

	Mesh.ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
	Mesh.ptrUniformBuffer->Create(sizeof(glm::mat4), NULL, TRUE);
	Mesh.ptrUniformBuffer->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));

	Mesh.ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(0, 0, Mesh.ptrGraphics);
	Mesh.ptrDescriptorSet->SetUniformBuffer(0, Mesh.ptrUniformBuffer);
	Mesh.ptrDescriptorSet->SetTexture(1, Mesh.ptrTexture);
}

void DestroyMesh(void)
{
	Mesh.ptrShaderVertex.Release();
	Mesh.ptrShaderFragment.Release();
	Mesh.ptrGraphics.Release();

	Mesh.ptrTexture.Release();

	Mesh.ptrIndexBuffer.Release();
	Mesh.ptrVertexBuffer.Release();
	Mesh.ptrUniformBuffer.Release();

	Mesh.ptrDescriptorSet.Release();
}

void CreateScreen(void)
{
	Screen.ptrShaderVertex = ShaderManager()->LoadResource("composition.vert", TRUE);
	Screen.ptrShaderFragment = ShaderManager()->LoadResource("composition.frag", TRUE);

	Screen.ptrGraphics = GfxDevice()->NewPipelineGraphics();
	Screen.ptrGraphics->SetVertexShader(Screen.ptrShaderVertex->GetGfxShader());
	Screen.ptrGraphics->SetFragmentShader(Screen.ptrShaderFragment->GetGfxShader());
	Screen.ptrGraphics->SetDepthTest(FALSE, FALSE, VK_COMPARE_OP_ALWAYS);
	Screen.ptrGraphics->SetCullMode(VK_CULL_MODE_BACK_BIT);
	Screen.ptrGraphics->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	Screen.ptrGraphics->SetColorBlendAttachment(0, FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	Screen.ptrGraphics->Create(Renderer.ptrRenderPass->GetHandle(), 1);
}

void DestroyScreen(void)
{
	Screen.ptrGraphics.Release();
	Screen.ptrShaderVertex.Release();
	Screen.ptrShaderFragment.Release();
}

void CreateCommandBuffer(void)
{

}

void DestroyCommandBuffer(void)
{

}

void Create(void)
{
	CreateRenderer();
	CreateMesh();
	CreateScreen();
	CreateCommandBuffer();
}

void Destroy(void)
{
	DestroyCommandBuffer();
	DestroyMesh();
	DestroyScreen();
	DestroyRenderer();
}

void Render(void)
{

}
/*
CrossEngine::CResShaderPtr ptrShaderVertex;
CrossEngine::CResShaderPtr ptrShaderFragment;
CrossEngine::CGfxPipelineGraphicsPtr ptrPipeline;

CrossEngine::CGfxIndexBufferPtr ptrIndexBuffer;
CrossEngine::CGfxVertexBufferPtr ptrVertexBuffer;
CrossEngine::CGfxUniformBufferPtr ptrUniformBufferA;
CrossEngine::CGfxUniformBufferPtr ptrUniformBufferB;
CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSetA;
CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSetB;

CrossEngine::CGfxIndexBufferPtr ptrRectIndexBuffer;
CrossEngine::CGfxVertexBufferPtr ptrRectVertexBuffer;
CrossEngine::CGfxUniformBufferPtr ptrRectUniformBuffer;

CrossEngine::CGfxRenderTexturePtr ptrDepthTexture;
CrossEngine::CGfxRenderPassPtr ptrRenderPass;
CrossEngine::CGfxFrameBufferPtr ptrFrameBuffers[3];

CrossEngine::CGfxCommandBufferPtr ptrCommandBuffers[3];


void CreateRenderPass(void)
{
	ptrRenderPass = GfxDevice()->NewRenderPass();
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
		ptrFrameBuffers[indexView] = GfxDevice()->NewFrameBuffer();
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

	ptrPipeline = GfxDevice()->NewPipelineGraphics();
	ptrPipeline->SetVertexShader(ptrShaderVertex->GetGfxShader());
	ptrPipeline->SetFragmentShader(ptrShaderFragment->GetGfxShader());
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
	ptrVertexBuffer->Create(vertexBufferSize, vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_FLAG_POSITION | CrossEngine::VERTEX_ATTRIBUTE_FLAG_COLOR);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
	uint32_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
	ptrIndexBuffer->Create(indexBufferSize, indexBuffer.data(), FALSE);

	ptrUniformBufferA = GfxDevice()->NewUniformBuffer();
	ptrUniformBufferA->Create(sizeof(glm::mat4), NULL, TRUE);
	ptrUniformBufferA->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));

	ptrUniformBufferB = GfxDevice()->NewUniformBuffer();
	ptrUniformBufferB->Create(sizeof(glm::mat4), NULL, TRUE);
	ptrUniformBufferB->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));
}

void CreateDescriptorSet(void)
{
	ptrDescriptorSetA = GfxDevice()->AllocDescriptorSet(0, 0, ptrPipeline);
	ptrDescriptorSetA->SetUniformBuffer(0, ptrUniformBufferA);
	ptrDescriptorSetA->UpdateDescriptorSets();

	ptrDescriptorSetB = GfxDevice()->AllocDescriptorSet(0, 0, ptrPipeline);
	ptrDescriptorSetB->SetUniformBuffer(0, ptrUniformBufferB);
	ptrDescriptorSetB->UpdateDescriptorSets();
}

void CreateCommandBuffer(void)
{
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

	static float angle = 0.0f; angle += 0.05f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
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

	GfxSwapChain()->AcquireNextImage(VK_NULL_HANDLE);
	{
		GfxDevice()->GetGraphicsQueue()->Submit(ptrCommandBuffers[GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}
	GfxSwapChain()->Present();
}
*/