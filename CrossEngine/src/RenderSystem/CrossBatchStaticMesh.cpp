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

	std::vector<CBatchStaticMesh::InstanceData> CBatchStaticMesh::datas;

	CBatchStaticMesh::CBatchStaticMesh(void)
	{

	}

	CBatchStaticMesh::~CBatchStaticMesh(void)
	{

	}

	void CBatchStaticMesh::ClearBuffer(void)
	{
		datas.clear();
	}

	void* CBatchStaticMesh::GetBuffer(void)
	{
		return datas.data();
	}

	size_t CBatchStaticMesh::GetBufferSize(void)
	{
		return datas.size() * sizeof(InstanceData);
	}

	void CBatchStaticMesh::UpdateInstanceBuffer(void)
	{
		m_firstInstance = datas.size();

		for (const auto &itDrawable : m_pDrawables) {
			InstanceData data;
			data.mtxModelToWorld = itDrawable.second->GetModelToWorldMatrix();

			datas.push_back(data);
		}
	}

	CGfxCommandBufferPtr CBatchStaticMesh::BuildCommandBuffer(void)
	{
		CGfxCommandBufferPtr ptrCommandBuffer = GfxDevice()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_SECONDARY);
		{
			ptrCommandBuffer->CmdBindVertexBuffer(m_ptrVertexBuffer, 0, 0);
			ptrCommandBuffer->CmdBindIndexBuffer(m_ptrIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
			ptrCommandBuffer->CmdBindDescriptorSetGraphics(m_ptrDescriptorSet, m_ptrPipelineGraphics);
			ptrCommandBuffer->CmdDrawIndexed(m_indexCount, m_pDrawables.size(), m_firstIndex, m_vertexOffset, m_firstInstance);
		}
		return ptrCommandBuffer;
	}

}
