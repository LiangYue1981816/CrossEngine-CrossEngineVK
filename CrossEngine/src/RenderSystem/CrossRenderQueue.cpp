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

	}

	CRenderQueue::~CRenderQueue(void)
	{

	}

	void CRenderQueue::Clear(void)
	{
		m_queue.clear();
		m_batchParticals.clear();
		m_batchSkinMeshs.clear();
		m_batchStaticMeshs.clear();
	}

	void CRenderQueue::AddDrawable(const CDrawable *pDrawable)
	{
		for (const auto &itMatPass : pDrawable->GetMaterial()->GetPasses()) {
			const uint32_t indexSubPass = itMatPass.second->GetIndexSubPass();
			const CGfxRenderPass *pRenderPass = itMatPass.second->GetRenderPass();
			const CGfxPipelineGraphics *pPipeline = itMatPass.second->GetPipeline();
			const CGfxDescriptorSet *pDescriptorSet = itMatPass.second->GetDescriptorSet();
			const CGfxVertexBuffer *pVertexBuffer = pDrawable->GetVertexBuffer();

			if (m_queue[pRenderPass][indexSubPass][pPipeline][pDescriptorSet][pVertexBuffer] == NULL) {
				m_queue[pRenderPass][indexSubPass][pPipeline][pDescriptorSet][pVertexBuffer] = CreateBatch(pDrawable->GetType());
			}

			m_queue[pRenderPass][indexSubPass][pPipeline][pDescriptorSet][pVertexBuffer]->AddDrawable(pDrawable);
		}
	}

	CBatch* CRenderQueue::CreateBatch(DRAWABLE_TYPE type)
	{
		CBatch *pBatch = NULL;

		switch (type) {
		case DRAWABLE_TYPE_PARTICAL:
			m_batchParticals.push_back(CBatchPartical());
			pBatch = &m_batchParticals[m_batchParticals.size() - 1];
			break;

		case DRAWABLE_TYPE_SKIN_MESH:
			m_batchSkinMeshs.push_back(CBatchSkinMesh());
			pBatch = &m_batchSkinMeshs[m_batchSkinMeshs.size() - 1];
			break;

		case DRAWABLE_TYPE_STATIC_MESH:
			m_batchStaticMeshs.push_back(CBatchStaticMesh());
			pBatch = &m_batchStaticMeshs[m_batchStaticMeshs.size() - 1];
			break;
		}

		return pBatch;
	}

	void CRenderQueue::UpdateInstanceBuffer(void)
	{
		for (int index = 0; index < m_batchParticals.size(); index++) {
			m_batchParticals[index].UpdateInstanceBuffer();
		}

		for (int index = 0; index < m_batchSkinMeshs.size(); index++) {
			m_batchSkinMeshs[index].UpdateInstanceBuffer();
		}

		for (int index = 0; index < m_batchStaticMeshs.size(); index++) {
			m_batchStaticMeshs[index].UpdateInstanceBuffer();
		}
	}

}
