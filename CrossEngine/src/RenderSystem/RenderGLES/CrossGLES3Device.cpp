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
	{

	}

	CGLES3Device::~CGLES3Device(void)
	{

	}

	int CGLES3Device::Create(void)
	{
		m_features.Init();
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
		return NO_ERROR;
	}

	int CGLES3Device::CreateDescriptorSetManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateBufferManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateTextureManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateShaderManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreatePipelineManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateRenderPassManager(void)
	{
		return NO_ERROR;
	}

	int CGLES3Device::CreateFrameBufferManager(void)
	{
		return NO_ERROR;
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

	}

	void CGLES3Device::DestroyDescriptorSetManager(void)
	{

	}

	void CGLES3Device::DestroyBufferManager(void)
	{

	}

	void CGLES3Device::DestroyTextureManager(void)
	{

	}

	void CGLES3Device::DestroyShaderManager(void)
	{

	}

	void CGLES3Device::DestroyPipelineManager(void)
	{

	}

	void CGLES3Device::DestroyRenderPassManager(void)
	{

	}

	void CGLES3Device::DestroyFrameBufferManager(void)
	{

	}

	CGLES3Queue* CGLES3Device::GetQueue(void) const
	{
		return m_pQueue;
	}

	CGLES3Instance* CGLES3Device::GetInstance(void) const
	{
		return m_pInstance;
	}

	const VkPhysicalDeviceFeatures& CGLES3Device::GetPhysicalDeviceFeatures(void) const
	{
		return m_features.GetPhysicalDeviceFeatures();
	}

	const VkPhysicalDeviceProperties& CGLES3Device::GetPhysicalDeviceProperties(void) const
	{
		return m_properties.GetPhysicalDeviceProperties();
	}

	CGfxCommandBufferPtr CGLES3Device::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level)
	{
		return CGfxCommandBufferPtr(NULL);
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxDescriptorSetPtr CGLES3Device::AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline)
	{
		return CGfxDescriptorSetPtr(NULL);
	}

	CGfxIndexBufferPtr CGLES3Device::NewIndexBuffer(void)
	{
		return CGfxIndexBufferPtr(NULL);
	}

	CGfxVertexBufferPtr CGLES3Device::NewVertexBuffer(void)
	{
		return CGfxVertexBufferPtr(NULL);
	}

	CGfxUniformBufferPtr CGLES3Device::NewUniformBuffer(void)
	{
		return CGfxUniformBufferPtr(NULL);
	}

	CGfxTexturePtr CGLES3Device::NewTexture(void)
	{
		return CGfxTexturePtr(NULL);
	}

	CGfxRenderTexturePtr CGLES3Device::NewRenderTexture(void)
	{
		return CGfxRenderTexturePtr(NULL);
	}

	CGfxShaderPtr CGLES3Device::NewShader(void)
	{
		return CGfxShaderPtr(NULL);
	}

	CGfxPipelineComputePtr CGLES3Device::NewPipelineCompute(void)
	{
		return CGfxPipelineComputePtr(NULL);
	}

	CGfxPipelineGraphicsPtr CGLES3Device::NewPipelineGraphics(void)
	{
		return CGfxPipelineGraphicsPtr(NULL);
	}

	CGfxRenderPassPtr CGLES3Device::NewRenderPass(void)
	{
		return CGfxRenderPassPtr(NULL);
	}

	CGfxFrameBufferPtr CGLES3Device::NewFrameBuffer(void)
	{
		return CGfxFrameBufferPtr(NULL);
	}

	void CGLES3Device::DumpLog(void) const
	{

	}

}
