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

	CRenderer::CRenderer(void)
		: pCamera(NULL)
		, pLightManager(NULL)
	{
		CreateThread();
	}

	CRenderer::~CRenderer(void)
	{
		DestroyThread();
	}

	void CRenderer::CreateThread(void)
	{
		event_init(&m_threadCluster.eventExit, 0);
		event_init(&m_threadCluster.eventReady, 1);
		event_init(&m_threadCluster.eventFinish, 1);
		event_init(&m_threadCluster.eventDispatch, 0);

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			m_threadCluster.params[indexThread].pRenderer = this;
			pthread_create(&m_threadCluster.threads[indexThread], NULL, WorkThread, &m_threadCluster.params[indexThread]);
		}
	}

	void CRenderer::DestroyThread(void)
	{
		event_signal(&m_threadCluster.eventExit);
		event_signal(&m_threadCluster.eventDispatch);

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			pthread_join(m_threadCluster.threads[indexThread], NULL);
		}

		event_destroy(&m_threadCluster.eventExit);
		event_destroy(&m_threadCluster.eventReady);
		event_destroy(&m_threadCluster.eventFinish);
		event_destroy(&m_threadCluster.eventDispatch);
	}

	void CRenderer::SetCamera(CCamera *pCamera)
	{
		this->pCamera = pCamera;
		this->pCamera->Apply();
	}

	void CRenderer::SetLightManager(CLightManager *pLightManager)
	{
		this->pLightManager = pLightManager;
		this->pLightManager->Apply();
	}

	void CRenderer::BuildCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		ResetMainCommandBuffer(ptrFrameBuffer, ptrRenderPass);
		DispatchThread(ptrFrameBuffer, ptrRenderPass, TRUE);
		BuildMainCommandBuffer(ptrFrameBuffer, ptrRenderPass);
	}

	void CRenderer::Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		if (pCamera == NULL) {
			return;
		}

		GfxDevice()->GetGraphicsQueue()->Submit(m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}

	void CRenderer::ResetMainCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		if (pCamera == NULL) {
			return;
		}

		const auto &itRenderPassQueue = pCamera->GetRenderQueue().find(ptrRenderPass);
		if (itRenderPassQueue == pCamera->GetRenderQueue().end()) return;

		uint32_t thread = thread_id() + (uint32_t)pCamera + (uint32_t)this;
		uint32_t frame = GfxSwapChain()->GetImageIndex();
		uint32_t pool = thread + frame;

		GfxDevice()->AllocCommandBufferPool(pool);

		if (m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass].find(frame) == m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass].end()) {
			m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][frame] = GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}
		else {
			m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][frame]->Reset();
		}
	}

	void CRenderer::BuildMainCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		if (pCamera == NULL) {
			return;
		}

		const auto &itRenderPassQueue = pCamera->GetRenderQueue().find(ptrRenderPass);
		if (itRenderPassQueue == pCamera->GetRenderQueue().end()) return;

		uint32_t frame = GfxSwapChain()->GetImageIndex();
		CGfxCommandBufferPtr &ptrMainCommandBuffer = m_ptrMainCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][frame];

		ptrMainCommandBuffer->BeginPrimary(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		{
			ptrMainCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
			{
				for (uint32_t indexPass = 0; indexPass < itRenderPassQueue->first->GetSubpassCount(); indexPass++) {
					const auto &itMaterialPipelineQueue = itRenderPassQueue->second.find(indexPass);
					if (itMaterialPipelineQueue != itRenderPassQueue->second.end()) {
						for (const auto &itMaterialPipeline : itMaterialPipelineQueue->second) {
							for (const auto &itSecondaryCommandBuffer : m_ptrSecondaryCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][itMaterialPipeline.first][indexPass][frame]) {
								ptrMainCommandBuffer->CmdExecuteCommandBuffer(itSecondaryCommandBuffer.second);
							}
						}
					}

					if (indexPass + 1 < ptrRenderPass->GetSubpassCount()) {
						ptrMainCommandBuffer->CmdNextSubpass(VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
					}
				}
			}
			ptrMainCommandBuffer->CmdEndRenderPass();
		}
		ptrMainCommandBuffer->End();
	}

	void CRenderer::BuildSecondaryCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, const std::vector<PipelineParam> &pipelines)
	{
		if (pCamera == NULL) {
			return;
		}

		const auto &itRenderPassQueue = pCamera->GetRenderQueue().find(ptrRenderPass);
		if (itRenderPassQueue == pCamera->GetRenderQueue().end()) return;

		uint32_t thread = thread_id() + (uint32_t)pCamera + (uint32_t)this;
		uint32_t frame = GfxSwapChain()->GetImageIndex();
		uint32_t pool = thread + frame;

		GfxDevice()->AllocCommandBufferPool(pool);
		GfxDevice()->ResetCommandBufferPool(pool);

		for (int indexPipeline = 0; indexPipeline < pipelines.size(); indexPipeline++) {
			const auto &itMaterialPipelineQueue = itRenderPassQueue->second.find(pipelines[indexPipeline].indexPass);
			if (itMaterialPipelineQueue == itRenderPassQueue->second.end()) continue;

			const auto &itMaterialDescriptorSetQueue = itMaterialPipelineQueue->second.find(pipelines[indexPipeline].ptrMaterialPipeline);
			if (itMaterialDescriptorSetQueue == itMaterialPipelineQueue->second.end()) continue;

			CGfxCommandBufferPtr &ptrCommandBuffer = m_ptrSecondaryCommandBuffers[pCamera][ptrFrameBuffer][ptrRenderPass][pipelines[indexPipeline].ptrMaterialPipeline][pipelines[indexPipeline].indexPass][frame][thread];

			if (ptrCommandBuffer.IsNull()) {
				ptrCommandBuffer = GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY);
			}

			ptrCommandBuffer->Clearup();
			ptrCommandBuffer->ClearCommands();
			ptrCommandBuffer->BeginSecondary(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, ptrFrameBuffer, ptrRenderPass, pipelines[indexPipeline].indexPass);
			{
				ptrCommandBuffer->CmdSetScissor(pCamera->GetViewportX(), pCamera->GetViewportY(), pCamera->GetViewportWidth(), pCamera->GetViewportHeight());
				ptrCommandBuffer->CmdSetViewport(pCamera->GetViewportX(), pCamera->GetViewportY(), pCamera->GetViewportWidth(), pCamera->GetViewportHeight());

				ptrCommandBuffer->CmdBindPipelineGraphics(pipelines[indexPipeline].ptrMaterialPipeline);
				ptrCommandBuffer->CmdBindDescriptorSetGraphics(pCamera->GetDescriptorSet());

				if (pLightManager) {
					ptrCommandBuffer->CmdBindDescriptorSetGraphics(pLightManager->GetDescriptorSet());
				}

				for (const auto &itMeshQueue : itMaterialDescriptorSetQueue->second) {
					ptrCommandBuffer->CmdBindDescriptorSetGraphics(itMeshQueue.first);
					for (const auto &itVertexBufferQueue : itMeshQueue.second) {
						ptrCommandBuffer->CmdBindVertexBuffer(itVertexBufferQueue.first);
						for (const auto &itIndexBufferQueue : itVertexBufferQueue.second) {
							ptrCommandBuffer->CmdBindIndexBuffer(itIndexBufferQueue.first);
							for (const auto &itMeshIndexCountQueue : itIndexBufferQueue.second) {
								for (const auto &itMeshIndexOffsetQueue : itMeshIndexCountQueue.second) {
									for (const auto &itMeshVertexOffsetQueue : itMeshIndexOffsetQueue.second) {
										for (const auto &itDrawDescriptorSet : itMeshVertexOffsetQueue.second) {
											itDrawDescriptorSet.second->BuildCommandBuffer(ptrCommandBuffer);
										}
									}
								}
							}
						}
					}
				}
			}
			ptrCommandBuffer->End();
		}
	}

	void CRenderer::DispatchThread(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, BOOL bWait)
	{
		if (pCamera == NULL) {
			return;
		}

		const auto &itRenderPassQueue = pCamera->GetRenderQueue().find(ptrRenderPass);
		if (itRenderPassQueue == pCamera->GetRenderQueue().end()) return;

		std::vector<PipelineParam> pipelines;
		for (const auto &itPass : itRenderPassQueue->second) {
			for (const auto &itMaterialPipeline : itPass.second) {
				PipelineParam param;
				param.indexPass = itPass.first;
				param.ptrMaterialPipeline = itMaterialPipeline.first;
				pipelines.push_back(param);
			}
		}

		for (int indexPipeline = 0; indexPipeline < pipelines.size(); indexPipeline++) {
			m_threadCluster.params[indexPipeline % THREAD_COUNT].pipelines.push_back(pipelines[indexPipeline]);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			m_threadCluster.params[indexThread].ptrRenderPass = ptrRenderPass;
			m_threadCluster.params[indexThread].ptrFrameBuffer = ptrFrameBuffer;

			event_unsignal(&m_threadCluster.eventReady);
			event_unsignal(&m_threadCluster.eventFinish);
		}

		event_signal(&m_threadCluster.eventDispatch);

		if (bWait) {
			WaitThread();
		}
	}

	void CRenderer::WaitThread(void)
	{
		event_wait(&m_threadCluster.eventFinish);

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			m_threadCluster.params[indexThread].ptrRenderPass.Release();
			m_threadCluster.params[indexThread].ptrFrameBuffer.Release();
			m_threadCluster.params[indexThread].pipelines.clear();
		}
	}

	void* CRenderer::WorkThread(void *pParams)
	{
		CRenderer::ThreadParam *pThreadParam = (CRenderer::ThreadParam *)pParams;
		CRenderer::ThreadCluster *pThreadCluster = &pThreadParam->pRenderer->m_threadCluster;

		while (TRUE) {
			event_wait(&pThreadCluster->eventDispatch);
			{
				if (event_wait_timeout(&pThreadCluster->eventExit, 0) == NO_ERROR) {
					break;
				}

				event_signal(&pThreadCluster->eventReady);
				event_wait(&pThreadCluster->eventReady);

				pThreadParam->pRenderer->BuildSecondaryCommandBuffer(pThreadParam->ptrFrameBuffer, pThreadParam->ptrRenderPass, pThreadParam->pipelines);
			}
			event_reset(&pThreadCluster->eventDispatch);
			event_signal(&pThreadCluster->eventFinish);
		}

		return NULL;
	}

}
