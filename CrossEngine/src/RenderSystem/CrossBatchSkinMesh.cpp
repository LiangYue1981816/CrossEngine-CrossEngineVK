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

	CBatchSkinMesh::CBatchSkinMesh(void)
	{

	}

	CBatchSkinMesh::~CBatchSkinMesh(void)
	{

	}

	void CBatchSkinMesh::ClearBuffer(void)
	{
		datas.clear();
	}

	void* CBatchSkinMesh::GetBuffer(void)
	{
		return datas.data();
	}

	size_t CBatchSkinMesh::GetBufferSize(void)
	{
		return datas.size() * sizeof(InstanceData);
	}

	void CBatchSkinMesh::UpdateInstanceBuffer(void)
	{
		m_offset = datas.size() * sizeof(InstanceData);

		for (const auto &itDrawable : m_pDrawables) {
			InstanceData data;
			data.mtxModelToWorld = itDrawable.second->GetModelToWorldMatrix();

			datas.push_back(data);
		}
	}

	CGfxCommandBufferPtr CBatchSkinMesh::BuildCommandBuffer(void)
	{
		CGfxCommandBufferPtr ptrCommandBuffer = GfxDevice()->AllocCommandBuffer(thread_id(), VK_COMMAND_BUFFER_LEVEL_SECONDARY);
		{

		}
		return ptrCommandBuffer;
	}

}
