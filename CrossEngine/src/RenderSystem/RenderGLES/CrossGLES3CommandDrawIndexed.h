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

	class CROSS_EXPORT CGLES3CommandDrawIndexed : public CGfxCommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandDrawIndexed(VkPrimitiveTopology topology, VkIndexType indexType, uint32_t indexCount, uint32_t instanceCount, uint32_t indexFirst, int32_t indexBufferOffset)
			: m_topology(topology)
			, m_indexType(indexType)
			, m_indexCount(indexCount)
			, m_instanceCount(instanceCount)
			, m_indexFirst(indexFirst)
			, m_indexBufferOffset(indexBufferOffset)
		{

		}


	protected:
		virtual void Execute(void) const
		{
			GLenum mode = CGLES3Helper::glTranslatePrimitiveTopology(m_topology);
			GLenum type = CGLES3Helper::glTranslateIndexType(m_indexType);
			GLsizei size = CGLES3Helper::glGetIndexTypeSize(m_indexType);
			glDrawElementsInstanced(mode, m_indexCount, type, (const void *)(m_indexBufferOffset + m_indexFirst * size), m_instanceCount);
		}


	protected:
		VkPrimitiveTopology m_topology;
		VkIndexType m_indexType;
		uint32_t m_indexCount;
		uint32_t m_instanceCount;
		uint32_t m_indexFirst;
		uint32_t m_indexBufferOffset;
	};

}
