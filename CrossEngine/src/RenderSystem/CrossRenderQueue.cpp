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
		for (int index = 0; index < m_batchParticals.size(); index++) {
			SAFE_DELETE(m_batchParticals[index]);
		}

		for (int index = 0; index < m_batchSkinMeshs.size(); index++) {
			SAFE_DELETE(m_batchSkinMeshs[index]);
		}

		for (int index = 0; index < m_batchStaticMeshs.size(); index++) {
			SAFE_DELETE(m_batchStaticMeshs[index]);
		}

		m_queue.clear();

		m_batchParticals.clear();
		m_batchSkinMeshs.clear();
		m_batchStaticMeshs.clear();

		m_ptrRenderPasses.clear();
		m_ptrPipelines.clear();
		m_ptrDescriptorSets.clear();
		m_ptrVertexBuffers.clear();
		m_ptrIndexBuffers.clear();
	}

	void CRenderQueue::AddDrawable(const CDrawable *pDrawable)
	{
		for (const auto &itMatPass : pDrawable->GetMaterial()->GetPasses()) {
			const uint32_t indexSubPass = itMatPass.second->GetIndexSubPass();
			const CGfxRenderPassPtr ptrRenderPass = itMatPass.second->GetRenderPass();
			const CGfxPipelineGraphicsPtr ptrPipeline = itMatPass.second->GetPipeline();
			const CGfxDescriptorSetPtr ptrDescriptorSet = itMatPass.second->GetDescriptorSet();
			const CGfxVertexBufferPtr ptrVertexBuffer = pDrawable->GetVertexBuffer();
			const CGfxIndexBufferPtr ptrIndexBuffer = pDrawable->GetIndexBuffer();

			m_ptrRenderPasses[ptrRenderPass] = ptrRenderPass;
			m_ptrPipelines[ptrPipeline] = ptrPipeline;
			m_ptrDescriptorSets[ptrDescriptorSet] = ptrDescriptorSet;
			m_ptrVertexBuffers[ptrVertexBuffer] = ptrVertexBuffer;
			m_ptrIndexBuffers[ptrIndexBuffer] = ptrIndexBuffer;

			if (m_queue[ptrRenderPass][indexSubPass][ptrPipeline][ptrDescriptorSet][ptrVertexBuffer][ptrIndexBuffer] == NULL) {
				m_queue[ptrRenderPass][indexSubPass][ptrPipeline][ptrDescriptorSet][ptrVertexBuffer][ptrIndexBuffer] = CreateBatch(pDrawable->GetType());
			}

			m_queue[ptrRenderPass][indexSubPass][ptrPipeline][ptrDescriptorSet][ptrVertexBuffer][ptrIndexBuffer]->AddDrawable(pDrawable);
		}
	}

	CBatch* CRenderQueue::CreateBatch(DRAWABLE_TYPE type)
	{
		if (type == DRAWABLE_TYPE_PARTICAL) {
			m_batchParticals.push_back(SAFE_NEW CBatchPartical);
			return m_batchParticals[m_batchParticals.size() - 1];
		}

		if (type == DRAWABLE_TYPE_SKIN_MESH) {
			m_batchSkinMeshs.push_back(SAFE_NEW CBatchSkinMesh);
			return m_batchSkinMeshs[m_batchSkinMeshs.size() - 1];
		}

		if (type == DRAWABLE_TYPE_STATIC_MESH) {
			m_batchStaticMeshs.push_back(SAFE_NEW CBatchStaticMesh);
			return m_batchStaticMeshs[m_batchStaticMeshs.size() - 1];
		}

		return NULL;
	}

	void CRenderQueue::UpdateBuffer(void)
	{
		for (int index = 0; index < m_batchParticals.size(); index++) {
			m_batchParticals[index]->UpdateBuffer();
		}

		for (int index = 0; index < m_batchSkinMeshs.size(); index++) {
			m_batchSkinMeshs[index]->UpdateBuffer();
		}

		for (int index = 0; index < m_batchStaticMeshs.size(); index++) {
			m_batchStaticMeshs[index]->UpdateBuffer();
		}
	}

	void CRenderQueue::Render(const CGfxRenderPass *pRenderPass, const CGfxFrameBuffer *pFrameBuffer)
	{

	}

}
