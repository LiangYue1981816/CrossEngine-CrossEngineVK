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

	CGLES3Buffer::CGLES3Buffer(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_buffer(0)

		, m_size(0)
		, m_usage(GL_STATIC_DRAW)
	{

	}

	CGLES3Buffer::~CGLES3Buffer(void)
	{

	}

	BOOL CGLES3Buffer::Create(GLenum target, size_t size, GLenum usage)
	{
		m_size = size;
		m_usage = usage;

		glGenBuffers(1, &m_buffer);
		glBindBuffer(target, m_buffer);
		glBufferData(target, size, NULL, m_usage);
		glBindBuffer(target, 0);

		return TRUE;
	}

	void CGLES3Buffer::Destroy(void)
	{
		if (m_buffer) {
			glDeleteBuffers(1, &m_buffer);
		}

		m_buffer = 0;
		m_size = 0;
		m_usage = GL_STATIC_DRAW;
	}

	BOOL CGLES3Buffer::SetData(GLenum target, size_t offset, size_t size, const void *pBuffer) const
	{
		glBindBuffer(target, m_buffer);
		glBufferSubData(target, offset, size, pBuffer);
		glBindBuffer(target, 0);

		return TRUE;
	}

	BOOL CGLES3Buffer::IsDynamic(void) const
	{
		return m_usage == GL_DYNAMIC_DRAW ? TRUE : FALSE;
	}

	size_t CGLES3Buffer::GetBufferSize(void) const
	{
		return m_size;
	}

	size_t CGLES3Buffer::GetMemorySize(void) const
	{
		return m_size;
	}

}
