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

	std::vector<CBatchPartical::InstanceData> CBatchPartical::datas;
	CGfxVertexBufferPtr CBatchPartical::ptrInstanceBuffer;

	CBatchPartical::CBatchPartical(void)
	{

	}

	CBatchPartical::~CBatchPartical(void)
	{

	}

	void CBatchPartical::ClearInstanceBuffer(void)
	{
		datas.clear();
	}

	void CBatchPartical::CreateInstanceBuffer(void)
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

			uint32_t format =
				VERTEX_INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL0 |
				VERTEX_INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL1 |
				VERTEX_INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL2 |
				VERTEX_INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL3;

			ptrInstanceBuffer->Destroy();
			ptrInstanceBuffer->Create(dataInstanceBufferSize, NULL, FALSE, format, 1);
		}

		ptrInstanceBuffer->SetData(0, dataBufferSize, datas.data());
	}

	CGfxVertexBufferPtr& CBatchPartical::GetInstanceBuffer(void)
	{
		return ptrInstanceBuffer;
	}

	void CBatchPartical::UpdateInstanceBuffer(void)
	{
		m_firstInstance = datas.size();

		for (const auto &itDrawable : m_pDrawables) {
			InstanceData data;
			data.mtxModelToWorld = itDrawable.second->GetTransform();
			datas.push_back(data);
		}
	}

	void CBatchPartical::BuildCommandBuffer(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		m_ptrCommandBuffer = ptrCommandBuffer;
		m_ptrCommandBuffer->BeginSecondary(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, ptrFrameBuffer, ptrRenderPass, m_indexSubPass);
		{
			// ...
		}
		m_ptrCommandBuffer->End();
	}

}
