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

	static uint32_t BufferSize(uint32_t size)
	{
		uint32_t index = 0;
		while ((uint32_t)(1 << index) < size) index++;
		return (uint32_t)(1 << index);
	}

	CBatchBuffer::CBatchBuffer(uint32_t format)
		: m_format(format)
	{

	}

	CBatchBuffer::~CBatchBuffer(void)
	{

	}

	const CGfxVertexBufferPtr& CBatchBuffer::GetBuffer(void) const
	{
		return m_ptrVertexBuffer;
	}

	BOOL CBatchBuffer::UpdateBuffer(size_t size, const void *pBuffer)
	{
		if (m_ptrVertexBuffer.IsNull()) {
			m_ptrVertexBuffer = GfxDevice()->NewVertexBuffer();
		}

		if (m_ptrVertexBuffer->GetBufferSize() < size) {
			m_ptrVertexBuffer->Destroy();
			m_ptrVertexBuffer->Create(max(32 * 1024, BufferSize(size)), NULL, TRUE, m_format);
		}

		return m_ptrVertexBuffer->UpdateData(0, size, pBuffer);
	}

}
