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
		typedef std::map<const CGfxVertexBuffer*, CBatch*> BatchQueue;
		typedef std::map<const CGfxDescriptorSet*, BatchQueue> DescriptorSetQueue;
		typedef std::map<const CGfxPipelineGraphics*, DescriptorSetQueue> PipelineQueue;
		typedef std::map<const uint32_t, PipelineQueue> SubPassQueue;
		typedef std::map<const CGfxRenderPass*, SubPassQueue> RenderPassQueue;

		
	protected:
		CRenderQueue(void);
		virtual ~CRenderQueue(void);


	public:
		void AddDrawable(CDrawable *pDrawable);
		void Clear(void);

		void UpdateInstanceBuffer(void);


	protected:
		RenderPassQueue m_queue;

	protected:
		std::vector<CBatchPartical> m_batchParticals;
		std::vector<CBatchSkinMesh> m_batchSkinMeshs;
		std::vector<CBatchStaticMesh> m_batchStaticMeshs;
	};

}
