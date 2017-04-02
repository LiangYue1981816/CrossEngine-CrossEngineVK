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

	class CROSS_EXPORT CRendererVertex
	{
	public:
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_POSITION    = 0x00000001;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_NORMAL      = 0x00000002;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_BINORMAL    = 0x00000004;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_COLOR       = 0x00000008;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX  = 0x00000010;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT = 0x00000020;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD0   = 0x00000040;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD1   = 0x00000080;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD2   = 0x00000100;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD3   = 0x00000200;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD4   = 0x00000400;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD5   = 0x00000800;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD6   = 0x00001000;
		static const uint32_t VERTEX_ATTRIBUTE_FLAG_TEXCOORD7   = 0x00002000;

		static uint32_t GetVertexSize(uint32_t format);
		static uint32_t GetAttributeOffset(uint32_t format, uint32_t attribute);
	};

}
