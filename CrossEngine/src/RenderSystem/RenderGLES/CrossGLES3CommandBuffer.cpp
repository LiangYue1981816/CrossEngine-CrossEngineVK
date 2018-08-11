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
#include "CrossGLES3CommandBeginCommandBufferPrimary.h"
#include "CrossGLES3CommandBeginCommandBufferSecondary.h"
#include "CrossGLES3CommandBeginRenderPass.h"
#include "CrossGLES3CommandEndRenderPass.h"
#include "CrossGLES3CommandEndCommandBuffer.h"
#include "CrossGLES3CommandBindFrameBuffer.h"
#include "CrossGLES3CommandBindPipelineCompute.h"
#include "CrossGLES3CommandBindPipelineGraphics.h"
#include "CrossGLES3CommandBindIndexBuffer.h"
#include "CrossGLES3CommandBindVertexBuffer.h"
#include "CrossGLES3CommandBindDescriptorSetCompute.h"
#include "CrossGLES3CommandBindDescriptorSetGraphics.h"
#include "CrossGLES3CommandSetViewport.h"
#include "CrossGLES3CommandSetScissor.h"
#include "CrossGLES3CommandSetLineWidth.h"
#include "CrossGLES3CommandSetDepthBias.h"
#include "CrossGLES3CommandSetDepthRange.h"
#include "CrossGLES3CommandSetDepthBounds.h"
#include "CrossGLES3CommandSetBlendConstants.h"
#include "CrossGLES3CommandSetStencilReference.h"
#include "CrossGLES3CommandSetStencilWriteMask.h"
#include "CrossGLES3CommandInvalidateFrameBuffer.h"
#include "CrossGLES3CommandResolve.h"
#include "CrossGLES3CommandDraw.h"
#include "CrossGLES3CommandDrawIndexed.h"
#include "CrossGLES3CommandDispatch.h"
#include "CrossGLES3CommandExecuteCommandBuffer.h"


namespace CrossEngine {

	CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3Device *pDevice, CGLES3CommandBufferManager *pCommandBufferManager)
		: m_pDevice(pDevice)
		, m_pCommandBufferManager(pCommandBufferManager)

		, m_indexPass(0)
	{

	}

	CGLES3CommandBuffer::~CGLES3CommandBuffer(void)
	{
		Reset();
	}

	void CGLES3CommandBuffer::Release(void)
	{
		m_pCommandBufferManager->FreeCommandBuffer(this);
	}

	CGLES3Device* CGLES3CommandBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	CGfxFence CGLES3CommandBuffer::GetFence(void) const
	{
		return NULL;
	}

	HANDLE CGLES3CommandBuffer::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	void CGLES3CommandBuffer::Reset(void)
	{
		Clearup();
		ClearCommands();
	}

	void CGLES3CommandBuffer::Clearup(void)
	{
		m_indexPass = 0;
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

	void CGLES3CommandBuffer::Execute(void) const
	{
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}
	}

	void CGLES3CommandBuffer::BeginPrimary(VkCommandBufferUsageFlags flags)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBeginCommandBufferPrimary(flags));
	}

	void CGLES3CommandBuffer::BeginSecondary(VkCommandBufferUsageFlags flags, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
	{
		m_indexPass = indexSubPass;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBeginCommandBufferSecondary(m_ptrFrameBuffer, m_ptrRenderPass, flags, indexSubPass, occlusionQueryEnable, queryFlags, pipelineStatistics));
	}

	void CGLES3CommandBuffer::End(void)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandEndCommandBuffer());
	}

	void CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, VkSubpassContents contents)
	{
		m_indexPass = 0;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));
	}

	void CGLES3CommandBuffer::CmdNextSubpass(VkSubpassContents contents)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandInvalidateFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));

		m_indexPass++;
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));
	}

	void CGLES3CommandBuffer::CmdEndRenderPass(void)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandInvalidateFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexPass));
	}

	void CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineComputePtr &ptrPipelineCompute)
	{
		m_ptrPipelineCompute = ptrPipelineCompute;
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindPipelineCompute(ptrPipelineCompute));
	}

	void CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
	{
		m_ptrPipelineGraphics = ptrPipelineGraphics;
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindPipelineGraphics(ptrPipelineGraphics));
	}

	void CGLES3CommandBuffer::CmdBindDescriptorSetCompute(const CGfxDescriptorSetPtr &ptrDescriptorSet)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindDescriptorSetCompute(ptrDescriptorSet, m_ptrPipelineCompute));
	}

	void CGLES3CommandBuffer::CmdBindDescriptorSetGraphics(const CGfxDescriptorSetPtr &ptrDescriptorSet)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindDescriptorSetGraphics(ptrDescriptorSet, m_ptrPipelineGraphics));
	}

	void CGLES3CommandBuffer::CmdBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindVertexBuffer(ptrVertexBuffer));
	}

	void CGLES3CommandBuffer::CmdBindIndexBuffer(const CGfxIndexBufferPtr &ptrIndexBuffer)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandBindIndexBuffer(ptrIndexBuffer));
	}

	void CGLES3CommandBuffer::CmdSetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetViewport(x, y, width, height));
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetDepthRange(minDepth, maxDepth));
	}

	void CGLES3CommandBuffer::CmdSetScissor(int x, int y, uint32_t width, uint32_t height)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetScissor(x, y, width, height));
	}

	void CGLES3CommandBuffer::CmdSetLineWidth(float lineWidth)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetLineWidth(lineWidth));
	}

	void CGLES3CommandBuffer::CmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetDepthBias(depthBiasConstantFactor, depthBiasSlopeFactor));
	}

	void CGLES3CommandBuffer::CmdSetBlendConstants(float red, float green, float blue, float alpha)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetBlendConstants(red, green, blue, alpha));
	}

	void CGLES3CommandBuffer::CmdSetDepthBounds(float minDepthBounds, float maxDepthBounds)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetDepthBounds(minDepthBounds, maxDepthBounds));
	}

	void CGLES3CommandBuffer::CmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetStencilWriteMask(faceMask, writeMask));
	}

	void CGLES3CommandBuffer::CmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference, uint32_t compareMask)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandSetStencilReference(faceMask, ((CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics))->GetDepthStencilState().front.compareOp, ((CGLES3PipelineGraphics *)&m_ptrPipelineGraphics)->GetDepthStencilState().back.compareOp, reference, compareMask));
	}

	void CGLES3CommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandDraw(((CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics))->GetInputAssemblyState().topology, vertexCount, instanceCount, firstVertex, firstInstance));
	}

	void CGLES3CommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstVertex, uint32_t firstInstance)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandDrawIndexed(((CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics))->GetInputAssemblyState().topology, indexCount, instanceCount, firstIndex, firstVertex, firstInstance));
	}

	void CGLES3CommandBuffer::CmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandDispatch(groupCountX, groupCountY, groupCountZ));
	}

	void CGLES3CommandBuffer::CmdExecuteCommandBuffer(const CGfxCommandBufferPtr &ptrCommandBuffer)
	{
		m_pCommands.emplace_back(SAFE_NEW CGLES3CommandExecuteCommandBuffer(ptrCommandBuffer));
	}

}
