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

	std::vector<CBatchSkinMesh::InstanceData> CBatchSkinMesh::datas;
	CGfxVertexBufferPtr CBatchSkinMesh::ptrInstanceBuffer;

	CBatchSkinMesh::CBatchSkinMesh(void)
	{

	}

	CBatchSkinMesh::~CBatchSkinMesh(void)
	{

	}

	void CBatchSkinMesh::ClearInstanceBuffer(void)
	{
		datas.clear();
	}

	void CBatchSkinMesh::CreateInstanceBuffer(void)
	{
		if (ptrInstanceBuffer.IsNull()) {
			ptrInstanceBuffer = GfxDevice()->NewVertexBuffer();
		}

		size_t dataBufferSize = datas.size() * sizeof(InstanceData);
		if (ptrInstanceBuffer->GetBufferSize() < dataBufferSize) {
			size_t dataInstanceBufferSize = 64 * 1024;
			while (dataInstanceBufferSize < dataBufferSize) {
				dataInstanceBufferSize <<= 1;
			}

			ptrInstanceBuffer->Destroy();
			ptrInstanceBuffer->Create(dataInstanceBufferSize, NULL, FALSE, 0);
		}

		ptrInstanceBuffer->SetData(0, dataBufferSize, datas.data());
	}

	CGfxVertexBufferPtr& CBatchSkinMesh::GetInstanceBuffer(void)
	{
		return ptrInstanceBuffer;
	}

	void CBatchSkinMesh::UpdateInstanceBuffer(void)
	{
		m_firstInstance = datas.size();

		for (const auto &itDrawable : m_pDrawables) {
			InstanceData data;
			data.mtxModelToWorld = itDrawable.second->GetTransform();
			datas.push_back(data);
		}
	}

	void CBatchSkinMesh::BuildCommandBuffer(CGfxCommandBufferPtr &ptrCommandBuffer)
	{
		m_ptrCommandBuffer = ptrCommandBuffer;
	}

}
