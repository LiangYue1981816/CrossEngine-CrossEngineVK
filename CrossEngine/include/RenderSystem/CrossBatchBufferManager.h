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

	class CROSS_EXPORT CBatchBufferManager
	{
		friend class CRenderSystem;


	public:
		typedef enum {
			BUFFER_TYPE_PARTICAL = 0,
			BUFFER_TYPE_SKIN_MESH,
			BUFFER_TYPE_STATIC_MESH,
			BUFFER_TYPE_COUNT
		} BUFFER_TYPE;


	protected:
		CBatchBufferManager(void);
		virtual ~CBatchBufferManager(void);


	public:
		CBatchBuffer* GetBuffer(BUFFER_TYPE type, int index) const;


	protected:
		CBatchBuffer *m_pBuffers[BUFFER_TYPE_COUNT][CGfxSwapchain::SWAPCHAIN_IMAGE_COUNT];
	};

}
