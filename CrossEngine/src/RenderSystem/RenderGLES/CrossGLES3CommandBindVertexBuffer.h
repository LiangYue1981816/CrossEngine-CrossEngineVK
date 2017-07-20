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

	class CROSS_EXPORT CGLES3CommandBindVertexBuffer : public CGLES3CommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandBindVertexBuffer(CGfxVertexBufferPtr &ptrVertexBuffer, CGfxPipelineGraphicsPtr& ptrPipelineGraphics)
		{
			m_ptrVertexBuffer = ptrVertexBuffer;
			m_ptrPipelineGraphics = ptrPipelineGraphics;
		}
		virtual ~CGLES3CommandBindVertexBuffer(void)
		{
			m_ptrVertexBuffer.Release();
			m_ptrPipelineGraphics.Release();
		}


	protected:
		virtual void Execute(void)
		{
			if (m_ptrVertexBuffer.IsNull()) {
				glBindVertexBuffer(0, 0, 0, 0);
			}
			else {
				const CGLES3PipelineGraphics *pPipeline = (CGLES3PipelineGraphics *)((CGfxPipelineGraphics *)m_ptrPipelineGraphics);
				const CGLES3Device *pDevice = pPipeline->GetDevice();
				const std::map<uint32_t, VkVertexInputAttributeDescription>& vertexInputAttributeDescriptions = pPipeline->GetInputAttributeDescriptions();

				GLuint bindingindex = 0;
				GLuint stride = pDevice->GetVertexSize(pPipeline->GetVertexFormat());
				glBindVertexBuffer(bindingindex, (GLuint)m_ptrVertexBuffer->GetHandle(), 0, stride);

				for (const auto &itVertexInputAttributeDescription : vertexInputAttributeDescriptions) {
					GLuint attribute = itVertexInputAttributeDescription.first;
					GLuint attribindex = itVertexInputAttributeDescription.second.location;

					GLenum type = GL_FLOAT;
					GLuint size = pDevice->GetVertexAttributeSize(attribute) / sizeof(float);
					GLuint offset = pDevice->GetVertexAttributeOffset(pPipeline->GetVertexFormat(), attribute);

					glVertexAttribBinding(attribindex, bindingindex);
					glVertexAttribFormat(attribindex, size, type, GL_FALSE, offset);
				}
			}
		}


	protected:
		CGfxVertexBufferPtr m_ptrVertexBuffer;
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;
	};

}
