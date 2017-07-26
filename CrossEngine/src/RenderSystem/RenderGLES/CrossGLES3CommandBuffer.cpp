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
#include "CrossGLES3CommandBindPass.h"
#include "CrossGLES3CommandBindPipelineCompute.h"
#include "CrossGLES3CommandBindPipelineGraphics.h"
#include "CrossGLES3CommandBindIndexBuffer.h"
#include "CrossGLES3CommandBindVertexBuffer.h"
#include "CrossGLES3CommandBindDescriptorSet.h"
#include "CrossGLES3CommandSetViewport.h"
#include "CrossGLES3CommandSetScissor.h"
#include "CrossGLES3CommandSetLineWidth.h"
#include "CrossGLES3CommandSetDepthBias.h"
#include "CrossGLES3CommandSetDepthBounds.h"
#include "CrossGLES3CommandSetBlendConstants.h"
#include "CrossGLES3CommandSetStencilReference.h"
#include "CrossGLES3CommandSetStencilWriteMask.h"
#include "CrossGLES3CommandDraw.h"
#include "CrossGLES3CommandDrawIndexed.h"
#include "CrossGLES3CommandDispatch.h"
#include "CrossGLES3CommandExecuteCommandBuffer.h"


namespace CrossEngine {

	CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CGLES3CommandBuffer::~CGLES3CommandBuffer(void)
	{
		Reset();
	}

	void CGLES3CommandBuffer::Release(void)
	{

	}

	void CGLES3CommandBuffer::Clearup(void)
	{
		m_ptrRenderPass.Release();
		m_ptrFrameBuffer.Release();
		m_ptrPipelineCompute.Release();
		m_ptrPipelineGraphics.Release();
	}

	void CGLES3CommandBuffer::ClearCommands(void)
	{
		for (auto &itCommand : m_pCommands) {
			SAFE_DELETE(itCommand);
		}

		m_pCommands.clear();
	}

	HANDLE CGLES3CommandBuffer::GetHandle(void) const
	{
		ASSERT(FALSE);
		return INVALID_HANDLE_VALUE;
	}

	void CGLES3CommandBuffer::Reset(void)
	{
		Clearup();
		ClearCommands();
	}

	void CGLES3CommandBuffer::Execute(void) const
	{
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}
	}

	int CGLES3CommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags)
	{
		return NO_ERROR;
	}

	int CGLES3CommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
	{
		return NO_ERROR;
	}

	int CGLES3CommandBuffer::End(void)
	{
		return NO_ERROR;
	}

	void CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
	{

	}

	void CGLES3CommandBuffer::CmdNextSubpass(VkSubpassContents contents)
	{

	}

	void CGLES3CommandBuffer::CmdEndRenderPass(void)
	{

	}

	void CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipeline)
	{

	}

	void CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipeline)
	{

	}

	void CGLES3CommandBuffer::CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE hLayout)
	{

	}

	void CGLES3CommandBuffer::CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet, HANDLE hLayout)
	{

	}

	void CGLES3CommandBuffer::CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, size_t offset)
	{

	}

	void CGLES3CommandBuffer::CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer, size_t offset, VkIndexType indexType)
	{

	}

	void CGLES3CommandBuffer::CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
	{

	}

	void CGLES3CommandBuffer::CmdSetScissor(int x, int y, uint32_t width, uint32_t height)
	{

	}

	void CGLES3CommandBuffer::CmdSetLineWidth(float lineWidth)
	{

	}

	void CGLES3CommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{

	}

	void CGLES3CommandBuffer::CmdSetBlendConstants(const float blendConstants[4])
	{

	}

	void CGLES3CommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds)
	{

	}

	void CGLES3CommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask)
	{

	}

	void CGLES3CommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference, uint32_t compareMask)
	{

	}

	void CGLES3CommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{

	}

	void CGLES3CommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{

	}

	void CGLES3CommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{

	}

	void CGLES3CommandBuffer::CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer)
	{

	}

}
