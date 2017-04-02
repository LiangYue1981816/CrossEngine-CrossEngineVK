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

	static const std::map<uint32_t, uint32_t> vertexAttributeSizes = {
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_POSITION,    12 },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_NORMAL,      12 },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_BINORMAL,    12 },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_COLOR,       12 },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX,  8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT, 8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD0,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD1,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD2,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD3,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD4,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD5,   8  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD6,   8  }, 
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD7,   8  }
	};

	static const std::map<uint32_t, std::string> vertexAttributeNames = {
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_POSITION,    "inPosition"   },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_NORMAL,      "inNormal"     },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_BINORMAL,    "inBinormal"   },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_COLOR,       "inColor"      },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX,  "inSkinIndex"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT, "inSkinWeight" },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD0,   "inTexcoord0"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD1,   "inTexcoord1"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD2,   "inTexcoord2"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD3,   "inTexcoord3"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD4,   "inTexcoord4"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD5,   "inTexcoord5"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD6,   "inTexcoord6"  },
		{ CRendererVertex::VERTEX_ATTRIBUTE_FLAG_TEXCOORD7,   "inTexcoord7"  }
	};

	uint32_t CRendererVertex::GetVertexSize(uint32_t format)
	{
		uint32_t size = 0;

		for (std::map<uint32_t, uint32_t>::const_iterator itAttributeSize = vertexAttributeSizes.begin(); itAttributeSize != vertexAttributeSizes.end(); ++itAttributeSize) {
			if (format & itAttributeSize->first) size += itAttributeSize->second;
		}

		return size;
	}

	uint32_t CRendererVertex::GetAttributeOffset(uint32_t format, uint32_t attribute)
	{
		uint32_t offset = 0;

		for (std::map<uint32_t, uint32_t>::const_iterator itAttributeSize = vertexAttributeSizes.begin(); itAttributeSize != vertexAttributeSizes.end(); ++itAttributeSize) {
			if (attribute == itAttributeSize->first) return offset;
			if (format & itAttributeSize->first) offset += itAttributeSize->second;
		}

		return -1;
	}

}
