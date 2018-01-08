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
			const CGfxDescriptorSetPtr &ptrDrawableDescriptorSet = pDrawable->GetDescriptorSet();

			if (m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] == NULL) {
				m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet] = CreateBatch(pDrawable->GetType());
			}

			m_queue[ptrRenderPass][indexSubPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][ptrDrawableDescriptorSet]->AddDrawable(pDrawable);
		}
	}

	CBatch* CRenderQueue::CreateBatch(DRAWABLE_TYPE type)
	{
		if (type == DRAWABLE_TYPE_PARTICAL) {
			m_pBatchParticals.push_back(SAFE_NEW CBatchPartical);
			return m_pBatchParticals[m_pBatchParticals.size() - 1];
		}

		if (type == DRAWABLE_TYPE_SKIN_MESH) {
			m_pBatchSkinMeshs.push_back(SAFE_NEW CBatchSkinMesh);
			return m_pBatchSkinMeshs[m_pBatchSkinMeshs.size() - 1];
		}

		if (type == DRAWABLE_TYPE_STATIC_MESH) {
			m_pBatchStaticMeshs.push_back(SAFE_NEW CBatchStaticMesh);
			return m_pBatchStaticMeshs[m_pBatchStaticMeshs.size() - 1];
		}

		return NULL;
	}

	void CRenderQueue::UpdateBatchBuffer(void)
	{
		for (int index = 0; index < m_pBatchParticals.size(); index++) {
			m_pBatchParticals[index]->UpdateBuffer();
		}

		for (int index = 0; index < m_pBatchSkinMeshs.size(); index++) {
			m_pBatchSkinMeshs[index]->UpdateBuffer();
		}

		for (int index = 0; index < m_pBatchStaticMeshs.size(); index++) {
			m_pBatchStaticMeshs[index]->UpdateBuffer();
		}
	}

	void CRenderQueue::Render(const CGfxRenderPass *pRenderPass, const CGfxFrameBuffer *pFrameBuffer, CGfxCommandBuffer *pCommandBuffer)
	{

	}

}
