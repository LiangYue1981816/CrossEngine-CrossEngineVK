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

	CGfxVertexBufferPtr CBatchSkinMesh::m_ptrBatchBuffer;
	std::vector<CBatchSkinMesh::BatchData> CBatchSkinMesh::m_batchs;

	CBatchSkinMesh::CBatchSkinMesh(void)
	{

	}

	CBatchSkinMesh::~CBatchSkinMesh(void)
	{

	}

	CGfxVertexBufferPtr& CBatchSkinMesh::GetBatchBuffer(void)
	{
		return m_ptrBatchBuffer;
	}

	void CBatchSkinMesh::ClearBatchBuffer(void)
	{
		m_batchs.clear();
	}

	void CBatchSkinMesh::CreateBatchBuffer(void)
	{
		if (m_batchs.size()) {
			if (m_ptrBatchBuffer.IsNull()) {
				m_ptrBatchBuffer = GfxDevice()->NewVertexBuffer();
			}

			const int INSTANCE_BUFFER_SIZE = 4 * 1024;
			size_t batchBufferSize = m_batchs.size() * sizeof(BatchData);
			size_t instanceBufferSize = m_ptrBatchBuffer->GetBufferSize();

			if (instanceBufferSize == 0 || instanceBufferSize < batchBufferSize) {
				instanceBufferSize = INSTANCE_BUFFER_SIZE;
				while (instanceBufferSize < batchBufferSize) instanceBufferSize <<= 1;

				m_ptrBatchBuffer->Destroy();
				m_ptrBatchBuffer->Create(instanceBufferSize, NULL, TRUE, INSTANCE_BUFFER_BINDING);
			}

			m_ptrBatchBuffer->SetData(0, batchBufferSize, m_batchs.data());
		}
	}

	void CBatchSkinMesh::DestroyBatchBuffer(void)
	{
		m_batchs.clear();
		m_ptrBatchBuffer.Release();
	}

	void CBatchSkinMesh::UpdateBatchBuffer(void)
	{
		m_instanceOffset = m_batchs.size();

		for (const auto &itDrawable : m_pDrawables) {
			BatchData batchData;
			batchData.mtxTransform = itDrawable.second->GetTransform();
			m_batchs.push_back(batchData);
		}
	}

	void CBatchSkinMesh::BuildCommandBuffer(CGfxCommandBufferPtr &ptrCommandBuffer)
	{
		ptrCommandBuffer->CmdBindDescriptorSetGraphics(m_ptrDescriptorSet);
		ptrCommandBuffer->CmdBindVertexBuffer(m_ptrBatchBuffer);
		ptrCommandBuffer->CmdDrawIndexed(m_indexCount, m_pDrawables.size(), m_indexOffset, m_vertexOffset, m_instanceOffset);
	}

}
