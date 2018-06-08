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

	class CROSS_EXPORT CRenderer
	{
		friend class CCamera;


	protected:
		static const int THREAD_COUNT = 4;

		typedef struct PipelineParam {
			uint32_t indexPass;
			CGfxRenderPassPtr ptrRenderPass;
			CGfxPipelineGraphicsPtr ptrMaterialPipeline;
		} PipelineParam;

		typedef struct ThreadParam {
			CRenderer *pRenderer;
			std::vector<PipelineParam> pipelines;
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
		// [RenderPass][IndexPass][MaterialPipeline][MaterialDescriptorSet][VertexBuffer][IndexBuffer][indexCount][indexOffset][vertexOffset][DrawDescriptorSet] = Batch
		typedef 
			std::map<CGfxRenderPassPtr,
			std::map<uint32_t,
			std::map<CGfxPipelineGraphicsPtr,
			std::map<CGfxDescriptorSetPtr, 
			std::map<CGfxVertexBufferPtr, 
			std::map<CGfxIndexBufferPtr, 
			std::map<uint32_t, 
			std::map<uint32_t, 
			std::map<uint32_t, 
			std::map<CGfxDescriptorSetPtr, 
			CBatch*>>>>>>>>>> RenderQueue;

		// [FrameBuffer][RenderPass][Frame] = CommandBuffer
		typedef
			std::map<CGfxFrameBufferPtr,
			std::map<CGfxRenderPassPtr,
			std::map<uint32_t,
			CGfxCommandBufferPtr>>> MainCommandBufferMap;

		// [FrameBuffer][RenderPass][Pipeline][IndexPass][Frame][Thread] = CommandBuffer
		typedef
			std::map<CGfxFrameBufferPtr,
			std::map<CGfxRenderPassPtr,
			std::map<CGfxPipelineGraphicsPtr,
			std::map<uint32_t,
			std::map<uint32_t,
			std::map<uint32_t,
			CGfxCommandBufferPtr>>>>>> SecondaryCommandBufferMap;

		
	protected:
		CRenderer(CCamera *pCamera);
		virtual ~CRenderer(void);


	protected:
		void CreateThread(void);
		void DestroyThread(void);

		void DispatchThread(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, BOOL bWait);
		void WaitThread(void);

	protected:
		void Clear(void);
		void AddDrawable(const CDrawable *pDrawable);
		void UpdateBatchBuffer(void);

	protected:
		void BuildCommandBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
		void Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);

	protected:
		static void* WorkThread(void *pParams);


	protected:
		CCamera *m_pCamera;

	protected:
		RenderQueue m_queue;
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
