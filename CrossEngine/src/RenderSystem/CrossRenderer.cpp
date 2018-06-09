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

	CRenderer::CRenderer(CCamera *pCamera)
		: m_pCamera(pCamera)
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

		for (int index = 0; index < THREAD_COUNT; index++) {
			m_threadCluster.params[index].pRenderer = this;
			pthread_create(&m_threadCluster.threads[index], NULL, WorkThread, &m_threadCluster.params[index]);
		}
	}

	void CRenderer::DestroyThread(void)
	{
		event_signal(&m_threadCluster.eventExit);
		event_signal(&m_threadCluster.eventDispatch);

		for (int index = 0; index < THREAD_COUNT; index++) {
			pthread_join(m_threadCluster.threads[index], NULL);
		}

		event_destroy(&m_threadCluster.eventExit);
		event_destroy(&m_threadCluster.eventReady);
		event_destroy(&m_threadCluster.eventFinish);
		event_destroy(&m_threadCluster.eventDispatch);
	}

	void CRenderer::DispatchThread(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, BOOL bWait)
	{
		const auto &itRenderPassQueue = m_queue.find(ptrRenderPass);
		if (itRenderPassQueue == m_queue.end()) return;

		int indexThread = 0;
		for (const auto &itPass : itRenderPassQueue->second) {
			for (const auto &itMaterialPipeline : itPass.second) {
				PipelineParam param;
				param.indexPass = itPass.first;
				param.ptrMaterialPipeline = itMaterialPipeline.first;
				m_threadCluster.params[(indexThread++) % THREAD_COUNT].pipelines.push_back(param);
				m_threadCluster.params[(indexThread++) % THREAD_COUNT].ptrRenderPass = ptrRenderPass;
				m_threadCluster.params[(indexThread++) % THREAD_COUNT].ptrFrameBuffer = ptrFrameBuffer;
			}
		}

		m_ptrSecondaryCommandBuffers[ptrFrameBuffer][ptrRenderPass].clear();

		for (int index = 0; index < THREAD_COUNT; index++) {
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
			m_threadCluster.params[indexThread].pipelines.clear();
			m_threadCluster.params[indexThread].ptrRenderPass.Release();
			m_threadCluster.params[indexThread].ptrFrameBuffer.Release();
		}
	}

	void CRenderer::Clear(void)
	{
		for (int index = 0; index < m_pBatchParticals.size(); index++) {
			SAFE_DELETE(m_pBatchParticals[index]);
		}

		for (int index = 0; index < m_pBatchSkinMeshs.size(); index++) {
			SAFE_DELETE(m_pBatchSkinMeshs[index]);
		}

		for (int index = 0; index < m_pBatchStaticMeshs.size(); index++) {
			SAFE_DELETE(m_pBatchStaticMeshs[index]);
		}

		m_queue.clear();
		m_pBatchParticals.clear();
		m_pBatchSkinMeshs.clear();
		m_pBatchStaticMeshs.clear();
	}

	void CRenderer::AddDrawable(const CDrawable *pDrawable)
	{
		for (const auto &itMatPass : pDrawable->GetMaterial()->GetPasses()) {
			const uint32_t indexPass = itMatPass.second->GetIndexSubPass();
			const CGfxRenderPassPtr &ptrRenderPass = itMatPass.second->GetRenderPass();
			const CGfxPipelineGraphicsPtr &ptrMaterialPipeline = itMatPass.second->GetPipeline();
			const CGfxDescriptorSetPtr &ptrMaterialDescriptorSet = itMatPass.second->GetDescriptorSet();
			const CGfxVertexBufferPtr &ptrDrawableVertexBuffer = pDrawable->GetVertexBuffer();
			const CGfxIndexBufferPtr &ptrDrawableIndexBuffer = pDrawable->GetIndexBuffer();
			const CGfxDescriptorSetPtr &ptrDrawableDescriptorSet = pDrawable->GetDescriptorSet(itMatPass.first);
			const uint32_t indexCount = pDrawable->GetIndexCount();
			const uint32_t indexOffset = pDrawable->GetIndexOffset();
			const uint32_t vertexOffset = pDrawable->GetVertexOffset();

			if (m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] == NULL) {
				switch (pDrawable->GetType()) {
				case DRAWABLE_TYPE_PARTICAL:
					m_pBatchParticals.push_back(SAFE_NEW CBatchPartical);
					m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchParticals[m_pBatchParticals.size() - 1];
					break;

				case DRAWABLE_TYPE_SKIN_MESH:
					m_pBatchSkinMeshs.push_back(SAFE_NEW CBatchSkinMesh);
					m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchSkinMeshs[m_pBatchSkinMeshs.size() - 1];
					break;

				case DRAWABLE_TYPE_STATIC_MESH:
					m_pBatchStaticMeshs.push_back(SAFE_NEW CBatchStaticMesh);
					m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchStaticMeshs[m_pBatchStaticMeshs.size() - 1];
					break;

				default:
					LOGE("Not support drawable type = %d", pDrawable->GetType());
					return;
				}
			}

			m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet]->AddDrawable(pDrawable, itMatPass.first);
		}
	}

	void CRenderer::UpdateBatchBuffer(void)
	{
		for (int index = 0; index < m_pBatchParticals.size(); index++) {
			m_pBatchParticals[index]->UpdateBatchBuffer();
		}

		for (int index = 0; index < m_pBatchSkinMeshs.size(); index++) {
			m_pBatchSkinMeshs[index]->UpdateBatchBuffer();
		}

		for (int index = 0; index < m_pBatchStaticMeshs.size(); index++) {
			m_pBatchStaticMeshs[index]->UpdateBatchBuffer();
		}
	}

	void CRenderer::BuildCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		const auto &itRenderPassQueue = m_queue.find(ptrRenderPass);
		if (itRenderPassQueue == m_queue.end()) return;

		uint32_t thread = thread_id() + (uint32_t)this;
		uint32_t frame = GfxSwapChain()->GetImageIndex();
		uint32_t pool = thread + frame;

		GfxDevice()->AllocCommandBufferPool(pool);

		if (m_ptrMainCommandBuffers[ptrFrameBuffer][ptrRenderPass].find(frame) == m_ptrMainCommandBuffers[ptrFrameBuffer][ptrRenderPass].end()) {
			m_ptrMainCommandBuffers[ptrFrameBuffer][ptrRenderPass][frame] = GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}

		CGfxCommandBufferPtr &ptrMainCommandBuffer = m_ptrMainCommandBuffers[ptrFrameBuffer][ptrRenderPass][frame];
		ptrMainCommandBuffer->Reset();

		DispatchThread(ptrFrameBuffer, ptrRenderPass, TRUE);

		ptrMainCommandBuffer->BeginPrimary(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		{
			ptrMainCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
			{
				for (uint32_t indexPass = 0; indexPass < itRenderPassQueue->first->GetSubpassCount(); indexPass++) {
					const auto &itMaterialPipelineQueue = itRenderPassQueue->second.find(indexPass);
					if (itMaterialPipelineQueue != itRenderPassQueue->second.end()) {
						for (const auto &itMaterialPipeline : itMaterialPipelineQueue->second) {
							for (const auto &itSecondaryCommandBuffer : m_ptrSecondaryCommandBuffers[ptrFrameBuffer][ptrRenderPass][itMaterialPipeline.first][indexPass][frame]) {
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

	void CRenderer::Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		GfxDevice()->GetGraphicsQueue()->Submit(m_ptrMainCommandBuffers[ptrFrameBuffer][ptrRenderPass][GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}

	void* CRenderer::WorkThread(void *pParams)
	{
		CRenderer::ThreadParam *pThreadParam = (CRenderer::ThreadParam *)pParams;
		CRenderer::ThreadCluster *pThreadCluster = &pThreadParam->pRenderer->m_threadCluster;
		CRenderer *pRenderer = pThreadParam->pRenderer;

		while (TRUE) {
			event_wait(&pThreadCluster->eventDispatch);
			{
				if (event_wait_timeout(&pThreadCluster->eventExit, 0) == NO_ERROR) {
					break;
				}

				event_signal(&pThreadCluster->eventReady);
				event_wait(&pThreadCluster->eventReady);

				{
					uint32_t thread = thread_id() + (uint32_t)pRenderer;
					uint32_t frame = GfxSwapChain()->GetImageIndex();
					uint32_t pool = thread + frame;

					GfxDevice()->AllocCommandBufferPool(pool);
					GfxDevice()->ResetCommandBufferPool(pool);

					/*
					int numCommandBuffers = 0;
					CommandBufferSet &ptrSecondaryCommandBuffers = pRenderer->m_ptrSecondaryCommandBuffers[thread][frame];

					for (int index = pThreadParam->indexThread; index < pRenderer->m_pBatchStaticMeshs.size(); index += CRenderer::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderer->m_pBatchStaticMeshs[index]->BuildCommandBuffer(pRenderer->m_pCamera, ptrSecondaryCommandBuffers[numCommandBuffers], pRenderer->m_ptrFrameBuffer, pRenderer->m_ptrRenderPass);
					}

					for (int index = pThreadParam->indexThread; index < pRenderer->m_pBatchSkinMeshs.size(); index += CRenderer::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderer->m_pBatchSkinMeshs[index]->BuildCommandBuffer(pRenderer->m_pCamera, ptrSecondaryCommandBuffers[numCommandBuffers], pRenderer->m_ptrFrameBuffer, pRenderer->m_ptrRenderPass);
					}

					for (int index = pThreadParam->indexThread; index < pRenderer->m_pBatchParticals.size(); index += CRenderer::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderer->m_pBatchParticals[index]->BuildCommandBuffer(pRenderer->m_pCamera, ptrSecondaryCommandBuffers[numCommandBuffers], pRenderer->m_ptrFrameBuffer, pRenderer->m_ptrRenderPass);
					}
					*/
				}
			}
			event_reset(&pThreadCluster->eventDispatch);
			event_signal(&pThreadCluster->eventFinish);
		}

		return NULL;
	}

}
