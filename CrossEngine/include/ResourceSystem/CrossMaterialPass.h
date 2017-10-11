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

	class CROSS_EXPORT CMaterialPass
	{
	protected:
		CMaterialPass(void);
		virtual ~CMaterialPass(void);


	public:
		const CGfxPipelineGraphicsPtr& GetGfxPipeline(void) const;

	public:
		BOOL Load(CStream *pStream);
		BOOL PostLoad(void);

	protected:
		BOOL LoadShaders(CStream *pStream);
		BOOL LoadInputAssemblyState(CStream *pStream);
		BOOL LoadTessellationState(CStream *pStream);
		BOOL LoadRasterizationState(CStream *pStream);
		BOOL LoadMultisampleState(CStream *pStream);
		BOOL LoadDepthStencilState(CStream *pStream);
		BOOL LoadColorBlendState(CStream *pStream);
		BOOL LoadTextures(CStream *pStream);
		BOOL LoadUniforms(CStream *pStream);


	protected:
		CGfxPipelineGraphicsPtr m_ptrGfxPipeline;

	public:
		CMaterialPass *pNext;
	};

}
