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

	CGLES3VertexBuffer::CGLES3VertexBuffer(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxVertexBuffer(pResourceManager)
		, m_pDevice(pDevice)
	{

	}

	CGLES3VertexBuffer::~CGLES3VertexBuffer(void)
	{

	}

	CGLES3Device* CGLES3VertexBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3VertexBuffer::GetHandle(void) const
	{
		return (HANDLE)m_buffer;
	}

	BOOL CGLES3VertexBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic)
	{
		return CGLES3Buffer::Create(GL_ARRAY_BUFFER, size, pBuffer, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	void CGLES3VertexBuffer::Destroy(void)
	{
		CGLES3Buffer::Destroy();
	}

	BOOL CGLES3VertexBuffer::UpdateData(size_t offset, size_t size, const void *pBuffer) const
	{
		return CGLES3Buffer::UpdateData(GL_ARRAY_BUFFER, offset, size, pBuffer);
	}

	size_t CGLES3VertexBuffer::GetBufferSize(void) const
	{
		return m_size;
	}

	size_t CGLES3VertexBuffer::GetMemorySize(void) const
	{
		return m_size;
	}

	void CGLES3VertexBuffer::DumpLog(void) const
	{
		if (m_buffer) {
			LOGI("\t\tVertexBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_buffer, m_size, m_size, CGLES3Helper::glEnumToString(m_usage));
		}
	}

}
