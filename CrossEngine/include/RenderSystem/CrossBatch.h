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

	class CROSS_EXPORT CBatch
	{
	public:
		CBatch(void);
		virtual ~CBatch(void);


	public:
		virtual void Clear(void);
		virtual void AddDrawable(const CDrawable *pDrawable, const CGfxPipelineGraphicsPtr &ptrPipelineGraphics);

	public:
		virtual void UpdateInstanceBuffer(void) = 0;
		virtual CGfxCommandBufferPtr BuildCommandBuffer(void) = 0;


	protected:
		uint32_t m_indexCount;
		uint32_t m_firstIndex;
		uint32_t m_vertexOffset;
		uint32_t m_firstInstance;

	protected:
		CGfxIndexBufferPtr m_ptrIndexBuffer;
		CGfxVertexBufferPtr m_ptrVertexBuffer;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
		CGfxPipelineGraphicsPtr m_ptrPipelineGraphics;

	protected:
		std::map<const CDrawable*, const CDrawable*> m_pDrawables;
	};

}
