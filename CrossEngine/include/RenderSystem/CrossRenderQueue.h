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


		typedef std::map<CGfxVertexBufferPtr, std::map<CGfxIndexBufferPtr, std::map<CGfxDescriptorSetPtr, CBatch*>>> BatchQueue;
		typedef std::map<CGfxDescriptorSetPtr, BatchQueue> MaterialDescriptorSetQueue;
		typedef std::map<CGfxPipelineGraphicsPtr, MaterialDescriptorSetQueue> MaterialPipelineQueue;
		typedef std::map<uint32_t, MaterialPipelineQueue> SubPassQueue;
		typedef std::map<CGfxRenderPassPtr, SubPassQueue> RenderPassQueue;

		
	protected:
		CRenderQueue(void);
		virtual ~CRenderQueue(void);


	public:
		void Clear(void);
		void AddDrawable(const CDrawable *pDrawable);

	public:
		void UpdateBuffer(void);
		void Render(const CGfxRenderPass *pRenderPass, const CGfxFrameBuffer *pFrameBuffer, CGfxCommandBuffer *pCommandBuffer);

	protected:
		CBatch* CreateBatch(DRAWABLE_TYPE type);


	protected:
		RenderPassQueue m_queue;

	protected:
		std::vector<CBatchPartical*> m_pBatchParticals;
		std::vector<CBatchSkinMesh*> m_pBatchSkinMeshs;
		std::vector<CBatchStaticMesh*> m_pBatchStaticMeshs;
	};

}
