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

	CBatch::CBatch(void)
		: m_indexCount(0)
		, m_firstIndex(0)
		, m_vertexOffset(0)
		, m_firstInstance(0)

		, m_indexSubPass(0)
	{

	}

	CBatch::~CBatch(void)
	{

	}

	const CGfxCommandBufferPtr& CBatch::GetCommandBuffer(void) const
	{
		return m_ptrCommandBuffer;
	}

	void CBatch::Clear(void)
	{
		m_pDrawables.clear();

		m_indexCount = 0;
		m_firstIndex = 0;
		m_vertexOffset = 0;
		m_firstInstance = 0;

		m_indexSubPass = 0;
		m_ptrIndexBuffer.Release();
		m_ptrVertexBuffer.Release();
		m_ptrDrawDescriptorSet.Release();
		m_ptrMaterialDescriptorSet.Release();
		m_ptrMaterialPipelineGraphics.Release();
		m_ptrCommandBuffer.Release();
	}

	void CBatch::AddDrawable(uint32_t dwPassName, const CDrawable *pDrawable, const CGfxPipelineGraphicsPtr &ptrMaterialPipelineGraphics, const CGfxDescriptorSetPtr &ptrMaterialDescriptorSet, uint32_t indexSubPass)
	{
		m_pDrawables[pDrawable] = pDrawable;

		m_indexCount = pDrawable->GetIndexCount();
		m_firstIndex = pDrawable->GetFirstIndex();
		m_vertexOffset = pDrawable->GetVertexOffset();

		m_indexSubPass = indexSubPass;
		m_ptrIndexBuffer = pDrawable->GetIndexBuffer();
		m_ptrVertexBuffer = pDrawable->GetVertexBuffer();
		m_ptrDrawDescriptorSet = pDrawable->GetDescriptorSet(dwPassName);
		m_ptrMaterialDescriptorSet = ptrMaterialDescriptorSet;
		m_ptrMaterialPipelineGraphics = ptrMaterialPipelineGraphics;
	}

}
