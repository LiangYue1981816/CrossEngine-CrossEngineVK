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

	CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGLES3Buffer(pDevice)
		, CGfxUniformBuffer(pResourceManager)
		, m_vkDescriptorBufferInfo{ 0 }
	{

	}

	CGLES3UniformBuffer::~CGLES3UniformBuffer(void)
	{

	}

	CGLES3Device* CGLES3UniformBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3UniformBuffer::GetHandle(void) const
	{
		return (HANDLE)m_buffer;
	}

	BOOL CGLES3UniformBuffer::Create(size_t size, const void *pBuffer, BOOL bDynamic)
	{
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::Create(GL_UNIFORM_BUFFER, size, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		CALL_BOOL_FUNCTION_RETURN(CGLES3Buffer::SetData(GL_UNIFORM_BUFFER, 0, size, pBuffer));
		m_vkDescriptorBufferInfo.range = size;
		return TRUE;
	}

	void CGLES3UniformBuffer::Destroy(void)
	{
		CGLES3Buffer::Destroy();
	}

	BOOL CGLES3UniformBuffer::SetData(size_t offset, size_t size, const void *pBuffer) const
	{
		return CGLES3Buffer::SetData(GL_UNIFORM_BUFFER, offset, size, pBuffer);
	}

	const VkDescriptorBufferInfo& CGLES3UniformBuffer::GetDescriptorBufferInfo(void) const
	{
		return m_vkDescriptorBufferInfo;
	}

	BOOL CGLES3UniformBuffer::IsDynamic(void) const
	{
		return CGLES3Buffer::IsDynamic();
	}

	size_t CGLES3UniformBuffer::GetBufferSize(void) const
	{
		return CGLES3Buffer::GetBufferSize();
	}

	size_t CGLES3UniformBuffer::GetMemorySize(void) const
	{
		return CGLES3Buffer::GetMemorySize();
	}

	void CGLES3UniformBuffer::DumpLog(void) const
	{
		if (m_buffer) {
			LOGI("\t\tUniformBuffer 0x%x: buffer size = %d memory size = %d usage = %s\n", m_buffer, GetBufferSize(), GetMemorySize(), CGLES3Helper::glEnumToString(m_usage));
		}
	}

}
