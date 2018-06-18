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
			PipelineParam(uint32_t _indexPass, const CGfxPipelineGraphicsPtr &_ptrMaterialPipeline)
			{
				indexPass = _indexPass;
				ptrMaterialPipeline = _ptrMaterialPipeline;
			}

			uint32_t indexPass;
			CGfxPipelineGraphicsPtr ptrMaterialPipeline;
		} PipelineParam;

		typedef struct ThreadParam {
			CCamera *pCamera;
			CRenderer *pRenderer;

			CGfxRenderPassPtr ptrRenderPass;
			CGfxFrameBufferPtr ptrFrameBuffer;

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
		// [FrameBuffer][RenderPass][Frame] = MainCommandBuffer
		typedef
			std::map<CGfxFrameBufferPtr,
			std::map<CGfxRenderPassPtr,
			std::map<uint32_t,
			CGfxCommandBufferPtr>>> MainCommandBufferMap;

		// [FrameBuffer][RenderPass][Pipeline][IndexPass][Frame][Thread] = SecondaryCommandBuffer
		typedef
			std::map<CGfxFrameBufferPtr,
			std::map<CGfxRenderPassPtr,
			std::map<CGfxPipelineGraphicsPtr,
			std::map<uint32_t,
			std::map<uint32_t,
			std::map<uint32_t,
			CGfxCommandBufferPtr>>>>>> SecondaryCommandBufferMap;

		
	protected:
		CRenderer(void);
		virtual ~CRenderer(void);


	protected:
		void CreateThread(void);
		void DestroyThread(void);

	protected:
		void BuildCommandBuffer(CCamera *pCamera, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
		void Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);

	protected:
		void ResetMainCommandBuffer(CCamera *pCamera, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
		void BuildMainCommandBuffer(CCamera *pCamera, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
		void BuildSecondaryCommandBuffer(CCamera *pCamera, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, const std::vector<PipelineParam> &pipelines);

	protected:
		void DispatchThread(CCamera *pCamera, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, BOOL bWait);
		void WaitThread(void);

	protected:
		static void* WorkThread(void *pParams);


	protected:
		MainCommandBufferMap m_ptrMainCommandBuffers;
		SecondaryCommandBufferMap m_ptrSecondaryCommandBuffers;

	protected:
		ThreadCluster m_threadCluster;
	};

}
