/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	CGLES3Device::CGLES3Device(CGLES3Instance *pInstance)
		: m_pInstance(pInstance)

		, m_pQueue(NULL)
		, m_pCommandBufferManager(NULL)
		, m_pDescriptorSetManager(NULL)

		, m_pBufferManager(NULL)
		, m_pTextureManager(NULL)
		, m_pShaderManager(NULL)
		, m_pPipelineManager(NULL)
		, m_pRenderPassManager(NULL)
		, m_pFrameBufferManager(NULL)
	{
		m_pQueue = SAFE_NEW CGLES3Queue(this);
		m_pCommandBufferManager = SAFE_NEW CGLES3CommandBufferManager(this);
		m_pDescriptorSetManager = SAFE_NEW CGLES3DescriptorSetManager(this);

		m_pBufferManager = SAFE_NEW CGLES3BufferManager(this);
		m_pTextureManager = SAFE_NEW CGLES3TextureManager(this);
		m_pShaderManager = SAFE_NEW CGLES3ShaderManager(this);
		m_pPipelineManager = SAFE_NEW CGLES3PipelineManager(this);
		m_pRenderPassManager = SAFE_NEW CGLES3RenderPassManager(this);
		m_pFrameBufferManager = SAFE_NEW CGLES3FrameBufferManager(this);
	}

	CGLES3Device::~CGLES3Device(void)
	{
		SAFE_DELETE(m_pQueue);
		SAFE_DELETE(m_pCommandBufferManager);
		SAFE_DELETE(m_pDescriptorSetManager);

		SAFE_DELETE(m_pBufferManager);
		SAFE_DELETE(m_pTextureManager);
		SAFE_DELETE(m_pShaderManager);
		SAFE_DELETE(m_pPipelineManager);
		SAFE_DELETE(m_pRenderPassManager);
		SAFE_DELETE(m_pFrameBufferManager);
	}

	int CGLES3Device::Create(void)
	{
		m_properties.Init();

		CALL_GL_FUNCTION_RETURN(CreateDevice());
		CALL_GL_FUNCTION_RETURN(CreateQueue());
		CALL_GL_FUNCTION_RETURN(CreateCommandManager());
		CALL_GL_FUNCTION_RETURN(CreateDescriptorSetManager());

		CALL_GL_FUNCTION_RETURN(CreateBufferManager());
		CALL_GL_FUNCTION_RETURN(CreateTextureManager());
		CALL_GL_FUNCTION_RETURN(CreateShaderManager());
		CALL_GL_FUNCTION_RETURN(CreatePipelineManager());
		CALL_GL_FUNCTION_RETURN(CreateRenderPassManager());
		CALL_GL_FUNCTION_RETURN(CreateFrameBufferManager());

		return NO_ERROR;
	}

	int CGLES3Device::CreateDevice(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateQueue(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateCommandManager(void)
	{
		return m_pCommandBufferManager->Create();
	}

	int CGLES3Device::CreateDescriptorSetManager(void)
	{
		return m_pDescriptorSetManager->Create();
	}

	int CGLES3Device::CreateBufferManager(void)
	{
		return m_pBufferManager->Create();
	}

	int CGLES3Device::CreateTextureManager(void)
	{
		return m_pTextureManager->Create();
	}

	int CGLES3Device::CreateShaderManager(void)
	{
		return m_pShaderManager->Create();
	}

	int CGLES3Device::CreatePipelineManager(void)
	{
		return m_pPipelineManager->Create();
	}

	int CGLES3Device::CreateRenderPassManager(void)
	{
		return m_pRenderPassManager->Create();
	}

	int CGLES3Device::CreateFrameBufferManager(void)
	{
		return m_pFrameBufferManager->Create();
	}

	void CGLES3Device::Destroy(void)
	{
		glFinish();

		DestroyFrameBufferManager();
		DestroyRenderPassManager();
		DestroyPipelineManager();
		DestroyShaderManager();
		DestroyTextureManager();
		DestroyBufferManager();

		DestroyDescriptorSetManager();
		DestroyCommandManager();
		DestroyQueue();
		DestroyDevice();
	}

	void CGLES3Device::DestroyDevice(void)
	{

	}

	void CGLES3Device::DestroyQueue(void)
	{

	}

	void CGLES3Device::DestroyCommandManager(void)
	{
		m_pCommandBufferManager->Destroy();
	}

	void CGLES3Device::DestroyDescriptorSetManager(void)
	{
		m_pDescriptorSetManager->Destroy();
	}

	void CGLES3Device::DestroyBufferManager(void)
	{
		m_pBufferManager->Destroy();
	}

	void CGLES3Device::DestroyTextureManager(void)
	{
		m_pTextureManager->Destroy();
	}

	void CGLES3Device::DestroyShaderManager(void)
	{
		m_pShaderManager->Destroy();
	}

	void CGLES3Device::DestroyPipelineManager(void)
	{
		m_pPipelineManager->Destroy();
	}

	void CGLES3Device::DestroyRenderPassManager(void)
	{
		m_pRenderPassManager->Destroy();
	}

	void CGLES3Device::DestroyFrameBufferManager(void)
	{
		m_pFrameBufferManager->Destroy();
	}

	CGfxQueue* CGLES3Device::GetQueue(void) const
	{
		return m_pQueue;
	}

	CGfxInstance* CGLES3Device::GetInstance(void) const
	{
		return m_pInstance;
	}

	const GLPhysicalDeviceLimits& CGLES3Device::GetPhysicalDeviceLimits(void) const
	{
		return m_properties.GetPhysicalDeviceLimits();
	}

	CGfxCommandBufferPtr CGLES3Device::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return m_pCommandBufferManager->AllocCommandBuffer();
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline)
	{
		return m_pDescriptorSetManager->AllocDescriptorSet(((CGLES3PipelineCompute *)((CGfxPipelineCompute *)ptrPipeline))->GetDescriptorSetLayout(set));
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		return m_pDescriptorSetManager->AllocDescriptorSet(((CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)ptrPipeline))->GetDescriptorSetLayout(set));
	}

	CGfxIndexBufferPtr CGLES3Device::NewIndexBuffer(void)
	{
		return m_pBufferManager->AllocIndexBuffer();
	}

	CGfxVertexBufferPtr CGLES3Device::NewVertexBuffer(void)
	{
		return m_pBufferManager->AllocVertexBuffer();
	}

	CGfxUniformBufferPtr CGLES3Device::NewUniformBuffer(void)
	{
		return m_pBufferManager->AllocUniformBuffer();
	}

	CGfxTexturePtr CGLES3Device::NewTexture(void)
	{
		return m_pTextureManager->AllocTexture();
	}

	CGfxRenderTexturePtr CGLES3Device::NewRenderTexture(void)
	{
		return m_pTextureManager->AllocRenderTexture();
	}

	CGfxShaderPtr CGLES3Device::NewShader(void)
	{
		return m_pShaderManager->AllocShader();
	}

	CGfxPipelineComputePtr CGLES3Device::NewPipelineCompute(void)
	{
		return m_pPipelineManager->AllocPipelineCompute();
	}

	CGfxPipelineGraphicsPtr CGLES3Device::NewPipelineGraphics(void)
	{
		return m_pPipelineManager->AllocPipelineGraphics();
	}

	CGfxRenderPassPtr CGLES3Device::NewRenderPass(void)
	{
		return m_pRenderPassManager->AllocRenderPass();
	}

	CGfxFrameBufferPtr CGLES3Device::NewFrameBuffer(void)
	{
		return m_pFrameBufferManager->AllocFrameBuffer();
	}

	void CGLES3Device::DumpLog(void) const
	{
		LOGI("=================== GLES Resource Dump ===================\n");
		{
			m_pDescriptorSetManager->DumpLog("DescriptorSet ...");
			m_pBufferManager->DumpLog("Buffer ...");
			m_pTextureManager->DumpLog("Texture ...");
			m_pShaderManager->DumpLog("Shader ...");
			m_pPipelineManager->DumpLog("Pipeline ...");
			m_pRenderPassManager->DumpLog("RenderPass ...");
			m_pFrameBufferManager->DumpLog("FrameBuffer ...");
		}
		LOGI("============================================================\n");
	}

}
