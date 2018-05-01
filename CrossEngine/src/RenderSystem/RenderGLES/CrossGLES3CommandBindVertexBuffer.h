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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CGLES3CommandBindVertexBuffer : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, size_t offset, uint32_t binding)
			: m_offset(offset)
			, m_binding(binding)
		{
			m_ptrVertexBuffer = ptrVertexBuffer;
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrVertexBuffer.IsNull() || m_ptrVertexBuffer->GetHandle() == NULL) {
				return;
			}

			const CGLES3VertexBuffer *pVertexBuffer = (CGLES3VertexBuffer *)((CGfxVertexBuffer *)m_ptrVertexBuffer);
			const GLuint stride = pVertexBuffer->GetDevice()->GetVertexStride(pVertexBuffer->GetVertexFormat());

			glBindVertexArray((GLuint)pVertexBuffer->GetHandleVAO());
			glBindVertexBuffer(m_binding, (GLuint)pVertexBuffer->GetHandle(), m_offset, stride);
		}


	protected:
		GLuint m_offset;
		GLuint m_binding;
		CGfxVertexBufferPtr m_ptrVertexBuffer;
	};

}
