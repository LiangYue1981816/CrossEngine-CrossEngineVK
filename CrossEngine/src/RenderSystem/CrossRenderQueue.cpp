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

	CRenderQueue::CRenderQueue(void)
	{
		CreateThread();
	}

	CRenderQueue::~CRenderQueue(void)
	{
		DestroyThread();
	}

	void CRenderQueue::CreateThread(void)
	{
		event_init(&m_threadCluster.eventExit, 0);
		event_init(&m_threadCluster.eventReady, 1);
		event_init(&m_threadCluster.eventFinish, 1);
		event_init(&m_threadCluster.eventDispatch, 0);

		for (int index = 0; index < THREAD_COUNT; index++) {
			m_threadCluster.params[index].indexThread = index;
			m_threadCluster.params[index].pRenderQueue = this;
			pthread_create(&m_threadCluster.threads[index], NULL, WorkThread, &m_threadCluster.params[index]);
		}
	}

	void CRenderQueue::DestroyThread(void)
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

	void CRenderQueue::DispatchThread(BOOL bWait)
	{
		for (int index = 0; index < THREAD_COUNT; index++) {
			event_unsignal(&m_threadCluster.eventReady);
			event_unsignal(&m_threadCluster.eventFinish);
		}

		event_signal(&m_threadCluster.eventDispatch);

		if (bWait) {
			WaitThread();
		}
	}

	void CRenderQueue::WaitThread(void)
	{
		event_wait(&m_threadCluster.eventFinish);
	}

	void CRenderQueue::Clear(void)
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

	void CRenderQueue::AddDrawable(const CDrawable *pDrawable)
	{
		for (const auto &itMatPass : pDrawable->GetMaterial()->GetPasses()) {
			const uint32_t indexSubPass = itMatPass.second->GetIndexSubPass();
			const CGfxRenderPassPtr &ptrRenderPass = itMatPass.second->GetRenderPass();
			const CGfxPipelineGraphicsPtr &ptrMaterialPipeline = itMatPass.second->GetPipeline();
			const CGfxDescriptorSetPtr &ptrMaterialDescriptorSet = itMatPass.second->GetDescriptorSet();
			const CGfxVertexBufferPtr &ptrDrawableVertexBuffer = pDrawable->GetVertexBuffer();
			const CGfxIndexBufferPtr &ptrDrawableIndexBuffer = pDrawable->GetIndexBuffer();
			const CGfxDescriptorSetPtr &ptrDrawableDescriptorSet = pDrawable->GetDescriptorSet(itMatPass.first);

			if (m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] == NULL) {
				switch (pDrawable->GetType()) {
				case DRAWABLE_TYPE_PARTICAL:
					m_pBatchParticals.push_back(SAFE_NEW CBatchPartical);
					m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] = m_pBatchParticals[m_pBatchParticals.size() - 1];
					break;

				case DRAWABLE_TYPE_SKIN_MESH:
					m_pBatchSkinMeshs.push_back(SAFE_NEW CBatchSkinMesh);
					m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] = m_pBatchSkinMeshs[m_pBatchSkinMeshs.size() - 1];
					break;

				case DRAWABLE_TYPE_STATIC_MESH:
					m_pBatchStaticMeshs.push_back(SAFE_NEW CBatchStaticMesh);
					m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] = m_pBatchStaticMeshs[m_pBatchStaticMeshs.size() - 1];
					break;

				default:
					LOGE("Not support drawable type = %d", pDrawable->GetType());
					return;
				}
			}

			m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet]->AddDrawable(itMatPass.first, pDrawable, ptrMaterialPipeline);
		}
	}

	void CRenderQueue::UpdateInstanceBuffer(void)
	{
		for (int index = 0; index < m_pBatchParticals.size(); index++) {
			m_pBatchParticals[index]->UpdateInstanceBuffer();
		}

		for (int index = 0; index < m_pBatchSkinMeshs.size(); index++) {
			m_pBatchSkinMeshs[index]->UpdateInstanceBuffer();
		}

		for (int index = 0; index < m_pBatchStaticMeshs.size(); index++) {
			m_pBatchStaticMeshs[index]->UpdateInstanceBuffer();
		}
	}

	void CRenderQueue::PreBuildCommandBuffer(BOOL bWait)
	{
		DispatchThread(bWait);
	}

	void CRenderQueue::BuildCommandBuffer(const CGfxRenderPassPtr &ptrRenderPass, const CGfxFrameBufferPtr &ptrFrameBuffer)
	{
		const auto &itPassQueue = m_queue.find(ptrRenderPass);
		if (itPassQueue == m_queue.end()) return;

		WaitThread();

		uint32_t frame = GfxSwapChain()->GetImageIndex();
		if (m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer].find(frame) == m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer].end()) {
			m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer][frame] = GfxDevice()->AllocCommandBuffer(0, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}

		CGfxCommandBufferPtr &ptrMainCommandBuffer = m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer][frame];
		ptrMainCommandBuffer->Reset();
		ptrMainCommandBuffer->BeginPrimary(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		{
			ptrMainCommandBuffer->CmdBeginRenderPass(ptrFrameBuffer, ptrRenderPass, VK_SUBPASS_CONTENTS_INLINE);
			{
				ptrMainCommandBuffer->CmdSetScissor(0, 0, ptrFrameBuffer->GetWidth(), ptrFrameBuffer->GetHeight());
				ptrMainCommandBuffer->CmdSetViewport(0, 0, ptrFrameBuffer->GetWidth(), ptrFrameBuffer->GetHeight());

				for (uint32_t indexPass = 0; indexPass < ptrRenderPass->GetSubpassCount(); indexPass++) {
					const auto &itMaterialPipelineQueue = itPassQueue->second.find(indexPass);
					if (itMaterialPipelineQueue != itPassQueue->second.end()) {
						for (const auto &itMaterialDescriptorSetQueue : itMaterialPipelineQueue->second) {
							ptrMainCommandBuffer->CmdBindPipelineGraphics(itMaterialDescriptorSetQueue.first);
							{
								for (const auto itBatchQueue : itMaterialDescriptorSetQueue.second) {
									ptrMainCommandBuffer->CmdBindDescriptorSetGraphics(itBatchQueue.first, itMaterialDescriptorSetQueue.first);
									{
										for (const auto itVertexQueue : itBatchQueue.second) {
											ptrMainCommandBuffer->CmdBindVertexBuffer(itVertexQueue.first, 0, 0);
											{
												for (const auto itVertexIndexQueue : itVertexQueue.second) {
													ptrMainCommandBuffer->CmdBindIndexBuffer(itVertexIndexQueue.first, 0, VK_INDEX_TYPE_UINT16);
													{
														for (const auto itVertexIndexDescriptorSet : itVertexIndexQueue.second) {
															ptrMainCommandBuffer->CmdExecuteCommandBuffer(itVertexIndexDescriptorSet.second->GetCommandBuffer());
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					ptrMainCommandBuffer->CmdNextSubpass(VK_SUBPASS_CONTENTS_INLINE);
				}
			}
			ptrMainCommandBuffer->CmdEndRenderPass();
		}
		ptrMainCommandBuffer->End();
	}

	void CRenderQueue::Render(const CGfxRenderPassPtr &ptrRenderPass, const CGfxFrameBufferPtr &ptrFrameBuffer)
	{
		if (m_ptrMainCommandBuffers.find(ptrRenderPass) == m_ptrMainCommandBuffers.end()) return;
		if (m_ptrMainCommandBuffers[ptrRenderPass].find(ptrFrameBuffer) == m_ptrMainCommandBuffers[ptrRenderPass].end()) return;
		if (m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer].find(GfxSwapChain()->GetImageIndex()) == m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer].end()) return;

		GfxDevice()->GetGraphicsQueue()->Submit(m_ptrMainCommandBuffers[ptrRenderPass][ptrFrameBuffer][GfxSwapChain()->GetImageIndex()], GfxSwapChain()->GetAcquireSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, GfxSwapChain()->GetRenderDoneSemaphore());
	}

	void* CRenderQueue::WorkThread(void *pParams)
	{
		CRenderQueue::ThreadParam *pThreadParam = (CRenderQueue::ThreadParam *)pParams;
		CRenderQueue::ThreadCluster *pThreadCluster = &pThreadParam->pRenderQueue->m_threadCluster;
		CRenderQueue *pRenderQueue = pThreadParam->pRenderQueue;
		int indexThread = pThreadParam->indexThread;

		for (uint32_t frame = 0; frame < GfxSwapChain()->GetImageCount(); frame++) {
			uint32_t thread = thread_id();
			uint32_t pool = ((thread^frame) >> 1) | 0x80000000;
			GfxDevice()->AllocCommandBufferPool(pool);
		}

		while (TRUE) {
			event_wait(&pThreadCluster->eventDispatch);
			{
				if (event_wait_timeout(&pThreadCluster->eventExit, 0) == NO_ERROR) {
					break;
				}

				event_signal(&pThreadCluster->eventReady);
				event_wait(&pThreadCluster->eventReady);

				{
					uint32_t thread = thread_id();
					uint32_t frame = GfxSwapChain()->GetImageIndex();
					uint32_t pool = ((thread^frame) >> 1) | 0x80000000;

					GfxDevice()->ResetCommandBufferPool(pool);

					int numCommandBuffers = 0;
					CommandBufferSet &ptrSecondaryCommandBuffers = pRenderQueue->m_ptrSecondaryCommandBuffers[thread][frame];

					for (int index = indexThread; index < pRenderQueue->m_pBatchStaticMeshs.size(); index += CRenderQueue::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderQueue->m_pBatchStaticMeshs[index]->BuildCommandBuffer(ptrSecondaryCommandBuffers[numCommandBuffers]);
					}

					for (int index = indexThread; index < pRenderQueue->m_pBatchSkinMeshs.size(); index += CRenderQueue::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderQueue->m_pBatchSkinMeshs[index]->BuildCommandBuffer(ptrSecondaryCommandBuffers[numCommandBuffers]);
					}

					for (int index = indexThread; index < pRenderQueue->m_pBatchParticals.size(); index += CRenderQueue::THREAD_COUNT, numCommandBuffers++) {
						if (ptrSecondaryCommandBuffers.size() <= numCommandBuffers) {
							ptrSecondaryCommandBuffers.push_back(GfxDevice()->AllocCommandBuffer(pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY));
						}

						ptrSecondaryCommandBuffers[numCommandBuffers]->Clearup();
						ptrSecondaryCommandBuffers[numCommandBuffers]->ClearCommands();
						pRenderQueue->m_pBatchParticals[index]->BuildCommandBuffer(ptrSecondaryCommandBuffers[numCommandBuffers]);
					}
				}
			}
			event_reset(&pThreadCluster->eventDispatch);
			event_signal(&pThreadCluster->eventFinish);
		}

		return NULL;
	}

}
