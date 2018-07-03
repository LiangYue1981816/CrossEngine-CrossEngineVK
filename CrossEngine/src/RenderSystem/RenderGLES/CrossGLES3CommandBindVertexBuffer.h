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
		CGLES3CommandBindVertexBuffer(const CGfxVertexBufferPtr &ptrVertexBuffer, const CGfxPipelineGraphicsPtr &ptrPipelineGraphics)
		{
			m_ptrVertexBuffer = ptrVertexBuffer;
			m_ptrPipelineGraphics = ptrPipelineGraphics;
		}


	protected:
		virtual void Execute(void) const
		{
			if (m_ptrVertexBuffer.IsNull() || m_ptrVertexBuffer->GetHandle() == NULL) {
				return;
			}

			if (m_ptrPipelineGraphics.IsNull() || m_ptrPipelineGraphics->GetHandle() == NULL) {
				return;
			}

			glBindBuffer(GL_ARRAY_BUFFER, (GLuint)(CGLES3VertexBuffer *)((CGfxVertexBuffer *)m_ptrVertexBuffer)->GetHandle());
			{
				GLuint format = m_ptrPipelineGraphics->GetVertexFormat();
				GLuint stride = GfxDevice()->GetStride(format);

				for (GLuint indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
					GLuint attribute = (1 << indexAttribute);

					if (format & attribute) {
						GLuint location = GfxDevice()->GetAttributeLocation(attribute);
						GLuint components = GfxDevice()->GetAttributeComponents(attribute);
						GLuint offset = GfxDevice()->GetAttributeOffset(format, attribute);

						glEnableVertexAttribArray(location);
						glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
						glVertexAttribDivisor(location, attribute & INSTANCE_ATTRIBUTE_MASK ? 1 : 0);
					}
				}
			}
		}


	protected:
		CGfxVertexBufferPtr m_ptrVertexBuffer;
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;
	};

}
