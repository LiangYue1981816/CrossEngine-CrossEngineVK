#include "stdafx.h"

struct Vertex {
	float position[3];
	float texcoord[2];
};

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
	CrossEngine::CGfxDescriptorSetPtr ptrDescriptorSet;
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
	Renderer.ptrRenderPass->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, { 1.0f, 0.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	Renderer.ptrRenderPass->SetColorAttachment(1, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, { 0.0f, 1.0f, 0.0f, 1.0f }, VK_SAMPLE_COUNT_1_BIT);
	Renderer.ptrRenderPass->SetSubpassOutputColorReference(0, 1);
	Renderer.ptrRenderPass->SetSubpassInputColorReference(1, 1);
	Renderer.ptrRenderPass->SetSubpassOutputColorReference(1, 0);
	Renderer.ptrRenderPass->SetSubpassDependency(0, 0, 1);
	Renderer.ptrRenderPass->Create();

	Renderer.ptrColorTexture = GfxDevice()->NewRenderTexture();
	Renderer.ptrColorTexture->CreateColorTarget(VK_FORMAT_B8G8R8A8_UNORM, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);

	for (int index = 0; index < 3; index++) {
		Renderer.ptrFrameBuffers[index] = GfxDevice()->NewFrameBuffer(Renderer.ptrRenderPass->GetAttachmentCount());
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

	Mesh.ptrGraphics = GfxDevice()->NewPipelineGraphics(Renderer.ptrRenderPass->GetAttachmentCount());
	Mesh.ptrGraphics->SetVertexShader(Mesh.ptrShaderVertex->GetShader());
	Mesh.ptrGraphics->SetFragmentShader(Mesh.ptrShaderFragment->GetShader());
	Mesh.ptrGraphics->SetDepthTest(FALSE, FALSE, VK_COMPARE_OP_ALWAYS);
	Mesh.ptrGraphics->SetCullMode(VK_CULL_MODE_BACK_BIT);
	Mesh.ptrGraphics->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
	Mesh.ptrGraphics->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	Mesh.ptrGraphics->Create(Renderer.ptrRenderPass->GetHandle(), 0);

	Mesh.ptrTexture = GfxDevice()->NewTexture();
	Mesh.ptrTexture->CreateTexture2D((gli::texture2d)gli::load("../Data/Texture/het_kanonschot_rgba8.ktx"), VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

	std::vector<Vertex> vertexBuffer = {
		{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f } }
	};
	Mesh.ptrVertexBuffer = GfxDevice()->NewVertexBuffer();
	Mesh.ptrVertexBuffer->Create(vertexBuffer.size() * sizeof(Vertex), vertexBuffer.data(), FALSE, CrossEngine::VERTEX_ATTRIBUTE_FLAG_POSITION | CrossEngine::VERTEX_ATTRIBUTE_FLAG_TEXCOORD0);

	std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
	Mesh.ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
	Mesh.ptrIndexBuffer->Create(indexBuffer.size() * sizeof(uint32_t), indexBuffer.data(), FALSE);

	Mesh.ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
	Mesh.ptrUniformBuffer->Create(sizeof(glm::mat4), NULL, TRUE);
	Mesh.ptrUniformBuffer->SetDescriptorBufferInfo(0, 0, 0, sizeof(glm::mat4));

	Mesh.ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(0, 0, Mesh.ptrGraphics);
	Mesh.ptrDescriptorSet->SetUniformBuffer(0, Mesh.ptrUniformBuffer);
	Mesh.ptrDescriptorSet->SetTexture(1, Mesh.ptrTexture);
	Mesh.ptrDescriptorSet->UpdateDescriptorSets();
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

	Screen.ptrGraphics = GfxDevice()->NewPipelineGraphics(Renderer.ptrRenderPass->GetAttachmentCount());
	Screen.ptrGraphics->SetVertexShader(Screen.ptrShaderVertex->GetShader());
	Screen.ptrGraphics->SetFragmentShader(Screen.ptrShaderFragment->GetShader());
	Screen.ptrGraphics->SetDepthTest(FALSE, FALSE, VK_COMPARE_OP_ALWAYS);
	Screen.ptrGraphics->SetCullMode(VK_CULL_MODE_NONE);
	Screen.ptrGraphics->SetColorBlendAttachment(0, VK_FALSE, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, 0xf);
	Screen.ptrGraphics->Create(Renderer.ptrRenderPass->GetHandle(), 1);

	Screen.ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(0, 0, Screen.ptrGraphics);
	Screen.ptrDescriptorSet->SetRenderTexture(0, Renderer.ptrColorTexture);
	Screen.ptrDescriptorSet->UpdateDescriptorSets();
}

void DestroyScreen(void)
{
	Screen.ptrGraphics.Release();
	Screen.ptrShaderVertex.Release();
	Screen.ptrShaderFragment.Release();
	Screen.ptrDescriptorSet.Release();
}

void CreateCommandBuffer(void)
{
	for (int index = 0; index < (int)GfxSwapChain()->GetImageCount(); index++) {
		Renderer.ptrCommandBuffers[index] = GfxDevice()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		Renderer.ptrCommandBuffers[index]->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			Renderer.ptrCommandBuffers[index]->CmdBeginRenderPass(Renderer.ptrFrameBuffers[index], Renderer.ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
			Renderer.ptrCommandBuffers[index]->CmdSetViewport(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), 0.0f, 1.0f);
			Renderer.ptrCommandBuffers[index]->CmdSetScissor(0, 0, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight());
			{
				Renderer.ptrCommandBuffers[index]->CmdBindPipelineGraphics(Mesh.ptrGraphics);
				{
					Renderer.ptrCommandBuffers[index]->CmdBindDescriptorSetGraphics(Mesh.ptrDescriptorSet, Mesh.ptrGraphics);
					Renderer.ptrCommandBuffers[index]->CmdBindVertexBuffer(Mesh.ptrVertexBuffer, 0);
					Renderer.ptrCommandBuffers[index]->CmdBindIndexBuffer(Mesh.ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
					Renderer.ptrCommandBuffers[index]->CmdDrawIndexed(3, 1, 0, 0, 0);
				}
			}
			Renderer.ptrCommandBuffers[index]->CmdNextSubpass(VK_SUBPASS_CONTENTS_INLINE);
			{
				Renderer.ptrCommandBuffers[index]->CmdBindPipelineGraphics(Screen.ptrGraphics);
				{
					Renderer.ptrCommandBuffers[index]->CmdBindDescriptorSetGraphics(Screen.ptrDescriptorSet, Screen.ptrGraphics);
					Renderer.ptrCommandBuffers[index]->CmdDraw(3, 1, 0, 0);
				}
			}
			Renderer.ptrCommandBuffers[index]->CmdEndRenderPass();
		}
		Renderer.ptrCommandBuffers[index]->End();
	}
}

void DestroyCommandBuffer(void)
{
	for (int index = 0; index < (int)GfxSwapChain()->GetImageCount(); index++) {
		Renderer.ptrCommandBuffers[index].Release();
	}
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
	if (GfxSwapChain() == NULL) {
		return;
	}

	static float angle = 0.0f; angle += 0.01f;
	static glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 mtxProjection = mtxLH2RH * glm::perspective(glm::radians(60.0f), 1.0f * GfxSwapChain()->GetWidth() / GfxSwapChain()->GetHeight(), 0.1f, 100.0f);
	glm::mat4 mtxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mtxModel = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 mtxViewModelProjection = mtxProjection * mtxView * mtxModel;
	Mesh.ptrUniformBuffer->UpdateData(0, sizeof(glm::mat4), &mtxViewModelProjection);

	GfxSwapChain()->AcquireNextImage(VK_NULL_HANDLE);
	{
		GfxDevice()->GetGraphicsQueue()->Submit(Renderer.ptrCommandBuffers[GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}
	GfxSwapChain()->Present();
}
