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
		, m_pMaterialManager(NULL)
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
		m_pMaterialManager = SAFE_NEW CGLES3MaterialManager(this);
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
		SAFE_DELETE(m_pMaterialManager);
		SAFE_DELETE(m_pRenderPassManager);
		SAFE_DELETE(m_pFrameBufferManager);
	}

	int CGLES3Device::Create(void)
	{
		m_properties.Init();

		CALL_GL_FUNCTION_RETURN(CreateDevice());
		CALL_GL_FUNCTION_RETURN(CreateQueue());
		CALL_GL_FUNCTION_RETURN(CreateCommandBufferManager());
		CALL_GL_FUNCTION_RETURN(CreateDescriptorSetManager());

		CALL_GL_FUNCTION_RETURN(CreateBufferManager());
		CALL_GL_FUNCTION_RETURN(CreateTextureManager());
		CALL_GL_FUNCTION_RETURN(CreateShaderManager());
		CALL_GL_FUNCTION_RETURN(CreatePipelineManager());
		CALL_GL_FUNCTION_RETURN(CreateMaterialManager());
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

	int CGLES3Device::CreateCommandBufferManager(void)
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

	int CGLES3Device::CreateMaterialManager(void)
	{
		return m_pMaterialManager->Create();
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
		DestroyFrameBufferManager();
		DestroyRenderPassManager();
		DestroyMaterialManager();
		DestroyPipelineManager();
		DestroyShaderManager();
		DestroyTextureManager();
		DestroyBufferManager();

		DestroyDescriptorSetManager();
		DestroyCommandBufferManager();
		DestroyQueue();
		DestroyDevice();
	}

	void CGLES3Device::DestroyDevice(void)
	{

	}

	void CGLES3Device::DestroyQueue(void)
	{

	}

	void CGLES3Device::DestroyCommandBufferManager(void)
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

	void CGLES3Device::DestroyMaterialManager(void)
	{
		m_pMaterialManager->Destroy();
	}

	void CGLES3Device::DestroyRenderPassManager(void)
	{
		m_pRenderPassManager->Destroy();
	}

	void CGLES3Device::DestroyFrameBufferManager(void)
	{
		m_pFrameBufferManager->Destroy();
	}

	CGfxInstance* CGLES3Device::GetInstance(void) const
	{
		return m_pInstance;
	}

	CGfxQueue* CGLES3Device::GetComputeQueue(void) const
	{
		return m_pQueue;
	}

	CGfxQueue* CGLES3Device::GetGraphicsQueue(void) const
	{
		return m_pQueue;
	}

	CGfxQueue* CGLES3Device::GetTransferQueue(void) const
	{
		return m_pQueue;
	}

	const GLPhysicalDeviceLimits& CGLES3Device::GetPhysicalDeviceLimits(void) const
	{
		return m_properties.GetPhysicalDeviceLimits();
	}

	void CGLES3Device::ResetCommandBufferPool(uint32_t pool)
	{

	}

	void CGLES3Device::AllocCommandBufferPool(uint32_t pool)
	{

	}

	CGfxCommandBufferPtr CGLES3Device::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return m_pCommandBufferManager->AllocCommandBuffer();
	}

	void CGLES3Device::ResetDescriptorSetPool(uint32_t pool)
	{

	}

	void CGLES3Device::AllocDescriptorSetPool(uint32_t pool)
	{

	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout)
	{
		return m_pDescriptorSetManager->AllocDescriptorSet(ptrDescriptorSetLayout);
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline)
	{
		return AllocDescriptorSet(pool, ((CGLES3PipelineCompute *)((CGfxPipelineCompute *)ptrPipeline))->GetDescriptorSetLayout(set));
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		return AllocDescriptorSet(pool, ((CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)ptrPipeline))->GetDescriptorSetLayout(set));
	}

	CGfxDescriptorSetLayoutPtr CGLES3Device::AllocDescriptorSetLayout(uint32_t set)
	{
		return CGfxDescriptorSetLayoutPtr(SAFE_NEW CGLES3DescriptorSetLayout(this, set));
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

	CGfxMaterialPtr CGLES3Device::NewMaterial(void)
	{
		return m_pMaterialManager->AllocMaterial();
	}

	CGfxPipelineComputePtr CGLES3Device::NewPipelineCompute(void)
	{
		return m_pPipelineManager->AllocPipelineCompute();
	}

	CGfxPipelineGraphicsPtr CGLES3Device::NewPipelineGraphics(uint32_t numAttachments)
	{
		return m_pPipelineManager->AllocPipelineGraphics(numAttachments);
	}

	CGfxRenderPassPtr CGLES3Device::NewRenderPass(uint32_t numAttachments, uint32_t numSubpasses)
	{
		return m_pRenderPassManager->AllocRenderPass(numAttachments, numSubpasses);
	}

	CGfxFrameBufferPtr CGLES3Device::NewFrameBuffer(uint32_t numAttachments)
	{
		return m_pFrameBufferManager->AllocFrameBuffer(numAttachments);
	}

	void CGLES3Device::SetShaderCachePath(const char *szPath)
	{
		m_pShaderManager->SetCachePath(szPath);
	}

	void CGLES3Device::AddShaderIncludePath(const char *szPath)
	{
		m_pShaderManager->AddIncludePath(szPath);
	}

	void CGLES3Device::AddShaderMacroDefinition(const char *szName)
	{
		m_pShaderManager->AddMacroDefinition(szName);
	}

	void CGLES3Device::AddShaderMacroDefinition(const char *szName, const char *szValue)
	{
		m_pShaderManager->AddMacroDefinition(szName, szValue);
	}

	BOOL CGLES3Device::PrecompileShader(const char *szSource, size_t length, VkShaderStageFlagBits flags, std::vector<uint32_t> &words)
	{
		return m_pShaderManager->Precompile(szSource, length, flags, words);
	}

	int CGLES3Device::WaitIdle(void) const
	{
		glFinish();
		return NO_ERROR;
	}

	void CGLES3Device::DumpLog(void) const
	{
		LOGI("==================== GLES Resource Dump ====================\n");
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
