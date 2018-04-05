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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CRenderQueue
	{
		friend class CCamera;


	protected:
		static const int THREAD_COUNT = 4;

		typedef struct ThreadParam {
			uint32_t indexThread;
			CRenderQueue *pRenderQueue;
		} ThreadParam;

		typedef struct ThreadCluster {
			event_t eventExit;
			event_t eventReady;
			event_t eventFinish;
			event_t eventDispatch;
			pthread_t threads[THREAD_COUNT];
			ThreadParam params[THREAD_COUNT];
		} ThreadCluster;

	protected:
		typedef std::map<CGfxVertexBufferPtr, std::map<CGfxIndexBufferPtr, std::map<CGfxDescriptorSetPtr, CBatch*>>> BatchQueue;
		typedef std::map<CGfxDescriptorSetPtr, BatchQueue> MaterialDescriptorSetQueue;
		typedef std::map<CGfxPipelineGraphicsPtr, MaterialDescriptorSetQueue> MaterialPipelineQueue;
		typedef std::map<uint32_t, MaterialPipelineQueue> SubPassQueue;
		typedef std::map<CGfxRenderPassPtr, SubPassQueue> RenderPassQueue;

	protected:
		typedef std::vector<CGfxCommandBufferPtr> CommandBufferSet;
		typedef std::map<uint32_t, CGfxCommandBufferPtr> CommandBufferMap;
		typedef std::map<CGfxRenderPassPtr, std::map<CGfxFrameBufferPtr, CommandBufferMap>> MainCommandBufferMap;
		typedef std::map<uint32_t, std::map<uint32_t, CommandBufferSet>> SecondaryCommandBufferMap;

		
	protected:
		CRenderQueue(void);
		virtual ~CRenderQueue(void);


	protected:
		void CreateThread(void);
		void DestroyThread(void);
		void DispatchThread(BOOL bWait);
		void WaitThread(void);

	public:
		void Clear(void);
		void AddDrawable(const CDrawable *pDrawable);
		void UpdateInstanceBuffer(void);

	public:
		void BuildSecondaryCommandBuffer(BOOL bWait);
		void BuildMainCommandBuffer(const CGfxRenderPassPtr &ptrRenderPass, const CGfxFrameBufferPtr &ptrFrameBuffer);
		void Render(const CGfxRenderPassPtr &ptrRenderPass, const CGfxFrameBufferPtr &ptrFrameBuffer);

	protected:
		static void* WorkThread(void *pParams);


	protected:
		RenderPassQueue m_queue;

		std::vector<CBatchPartical*> m_pBatchParticals;
		std::vector<CBatchSkinMesh*> m_pBatchSkinMeshs;
		std::vector<CBatchStaticMesh*> m_pBatchStaticMeshs;

	protected:
		MainCommandBufferMap m_ptrMainCommandBuffers;
		SecondaryCommandBufferMap m_ptrSecondaryCommandBuffers;

	protected:
		ThreadCluster m_threadCluster;
	};

}
