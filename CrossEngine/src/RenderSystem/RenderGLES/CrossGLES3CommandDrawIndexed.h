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
		CGLES3CommandDrawIndexed(VkPrimitiveTopology topology, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstVertex, uint32_t firstInstance)
			: m_topology(topology)
			, m_indexCount(indexCount)
			, m_instanceCount(instanceCount)
			, m_firstIndex(firstIndex)
			, m_firstVertex(firstVertex)
			, m_firstInstance(firstInstance)
		{

		}


	protected:
		virtual void Execute(void) const
		{
			typedef struct {
				uint32_t indexCount;
				uint32_t instanceCount;
				uint32_t firstIndex;
				uint32_t firstVertex;
				uint32_t reservedMustBeZero;
			} DrawElementsIndirectCommand;

			GLenum mode = CGLES3Helper::glTranslatePrimitiveTopology(m_topology);
			DrawElementsIndirectCommand cmd = { m_indexCount, m_instanceCount, m_firstIndex, m_firstVertex, 0 };
			glDrawElementsIndirect(mode, GL_UNSIGNED_INT, &cmd);
		}


	protected:
		VkPrimitiveTopology m_topology;
		uint32_t m_indexCount;
		uint32_t m_instanceCount;
		uint32_t m_firstIndex;
		uint32_t m_firstVertex;
		uint32_t m_firstInstance;
	};

}
