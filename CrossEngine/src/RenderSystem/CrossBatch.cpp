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
		: m_offset(0)
	{

	}

	CBatch::~CBatch(void)
	{

	}

	uint32_t CBatch::GetBufferOffset(void) const
	{
		return m_offset;
	}

	uint32_t CBatch::GetDrawableCount(void) const
	{
		return m_pDrawables.size();
	}

	void CBatch::Clear(void)
	{
		m_pDrawables.clear();
	}

	void CBatch::AddDrawable(const CDrawable *pDrawable)
	{
		m_pDrawables[pDrawable] = pDrawable;

		m_ptrIndexBuffer = pDrawable->GetIndexBuffer();
		m_ptrVertexBuffer = pDrawable->GetVertexBuffer();
		m_ptrDescriptorSet = pDrawable->GetDescriptorSet();
	}

}
