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
		, m_binding(0)
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

	BOOL CGLES3VertexBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic, uint32_t format, uint32_t binding)
	{
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::Create(GL_ARRAY_BUFFER, size, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::SetData(GL_ARRAY_BUFFER, 0, size, pBuffer));
		CALL_BOOL_FUNCTION_RETURN(SetupFormat(format));
		m_binding = binding;
		return TRUE;
	}

	void CGLES3VertexBuffer::Destroy(void)
	{
		m_binding = 0;
		CGLES3Buffer::Destroy();
	}

	BOOL CGLES3VertexBuffer::SetupFormat(uint32_t format) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		{
			GLuint stride = m_pDevice->GetStride(format);

			for (GLuint indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
				GLuint attribute = (1 << indexAttribute);

				if (format & attribute) {
					GLuint location = m_pDevice->GetAttributeLocation(attribute);
					GLuint components = m_pDevice->GetAttributeComponents(attribute);
					GLuint offset = m_pDevice->GetAttributeOffset(format, attribute);

					glEnableVertexAttribArray(location);
					glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
					glVertexAttribDivisor(location, attribute & INSTANCE_ATTRIBUTE_MASK ? 1 : 0);
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return TRUE;
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

	void CGLES3VertexBuffer::DumpLog(void) const
	{
		if (m_buffer) {
			LOGI("\t\tVertexBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_buffer, GetBufferSize(), GetMemorySize(), CGLES3Helper::glEnumToString(m_usage));
		}
	}

}
