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

	CGfxVertexBufferPtr CBatchStaticMesh::m_ptrInstanceBuffer;
	std::vector<CBatchStaticMesh::InstanceData> CBatchStaticMesh::m_datas;

	CBatchStaticMesh::CBatchStaticMesh(void)
	{

	}

	CBatchStaticMesh::~CBatchStaticMesh(void)
	{

	}

	CGfxVertexBufferPtr& CBatchStaticMesh::GetInstanceBuffer(void)
	{
		return m_ptrInstanceBuffer;
	}

	void CBatchStaticMesh::ClearInstanceBuffer(void)
	{
		m_datas.clear();
	}

	void CBatchStaticMesh::CreateInstanceBuffer(void)
	{
		if (m_ptrInstanceBuffer.IsNull()) {
			m_ptrInstanceBuffer = GfxDevice()->NewVertexBuffer();
		}

		size_t instanceBufferSize = m_datas.size() * sizeof(InstanceData);

		if (m_ptrInstanceBuffer->GetBufferSize() < instanceBufferSize) {
			m_ptrInstanceBuffer->Destroy();
			m_ptrInstanceBuffer->Create(FitBufferSize(instanceBufferSize), NULL, TRUE, INSTANCE_ATTRIBUTE_TRANSFORM, INSTANCE_BUFFER_BINDING);
		}

		m_ptrInstanceBuffer->SetData(0, instanceBufferSize, m_datas.data());
	}

	void CBatchStaticMesh::DestroyInstanceBuffer(void)
	{
		m_datas.clear();
		m_ptrInstanceBuffer.Release();
	}

	void CBatchStaticMesh::UpdateInstanceBuffer(void)
	{
		m_firstInstance = m_datas.size();

		for (const auto &itDrawable : m_pDrawables) {
			InstanceData data;
			data.mtxTransform = itDrawable.second->GetTransform();
			m_datas.push_back(data);
		}
	}

}
