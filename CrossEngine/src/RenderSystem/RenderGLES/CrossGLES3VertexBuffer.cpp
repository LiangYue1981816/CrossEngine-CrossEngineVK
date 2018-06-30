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
		: CGLES3Buffer(pDevice)
		, CGfxVertexBuffer(pResourceManager)
		, m_vao(0)
		, m_binding(0)
		, m_vertexFormat(0)
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

	HANDLE CGLES3VertexBuffer::GetHandleVAO(void) const
	{
		return (HANDLE)m_vao;
	}

	BOOL CGLES3VertexBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic, uint32_t format, uint32_t binding)
	{
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::Create(GL_ARRAY_BUFFER, size, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::SetData(GL_ARRAY_BUFFER, 0, size, pBuffer));
		CALL_BOOL_FUNCTION_RETURN(CreateVAO(format, binding));
		return TRUE;
	}

	BOOL CGLES3VertexBuffer::CreateVAO(uint32_t format, uint32_t binding)
	{
		m_vao = 0;
		m_binding = binding;
		m_vertexFormat = format;

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindVertexBuffer(m_binding, m_buffer, 0, m_pDevice->GetStride(m_vertexFormat));
		{
			SetupFormat();
		}
		glBindVertexArray(0);
		glBindVertexBuffer(0, 0, 0, 0);

		return TRUE;
	}

	void CGLES3VertexBuffer::Destroy(void)
	{
		DestroyVAO();
		CGLES3Buffer::Destroy();
	}

	void CGLES3VertexBuffer::DestroyVAO(void)
	{
		if (m_vao) {
			glDeleteVertexArrays(1, &m_vao);
		}

		m_vao = 0;
		m_binding = 0;
		m_vertexFormat = 0;
	}

	void CGLES3VertexBuffer::SetupFormat(void) const
	{
		for (GLuint indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			GLuint attribute = (1 << indexAttribute);

			if (m_vertexFormat & attribute) {
				GLuint location = m_pDevice->GetAttributeLocation(attribute);
				GLuint size = m_pDevice->GetAttributeSize(attribute);
				GLuint offset = m_pDevice->GetAttributeOffset(m_vertexFormat, attribute);

				glEnableVertexAttribArray(location);
				glVertexAttribBinding(location, m_binding);
				glVertexAttribFormat(location, size, GL_FLOAT, GL_FALSE, offset);
			}
		}
	}

	BOOL CGLES3VertexBuffer::SetData(size_t offset, size_t size, const void *pBuffer) const
	{
		return CGLES3Buffer::SetData(GL_ARRAY_BUFFER, offset, size, pBuffer);
	}

	BOOL CGLES3VertexBuffer::IsDynamic(void) const
	{
		return CGLES3Buffer::IsDynamic();
	}

	size_t CGLES3VertexBuffer::GetBufferSize(void) const
	{
		return CGLES3Buffer::GetBufferSize();
	}

	size_t CGLES3VertexBuffer::GetMemorySize(void) const
	{
		return CGLES3Buffer::GetMemorySize();
	}

	uint32_t CGLES3VertexBuffer::GetBinding(void) const
	{
		return m_binding;
	}

	uint32_t CGLES3VertexBuffer::GetVertexFormat(void) const
	{
		return m_vertexFormat;
	}

	void CGLES3VertexBuffer::DumpLog(void) const
	{
		if (m_buffer) {
			LOGI("\t\tVertexBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_buffer, GetBufferSize(), GetMemorySize(), CGLES3Helper::glEnumToString(m_usage));
		}
	}

}
