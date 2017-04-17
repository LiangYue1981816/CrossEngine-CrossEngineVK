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

	struct VERTEX_ATTRIBUTE {
		uint32_t flag;
		uint32_t size;
		VkFormat format;
		const char *name;
	};

	static const VERTEX_ATTRIBUTE vertexAttributes[CRendererDevice::ATTRIBUTE_COUNT] = {
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_POSITION,    12, VK_FORMAT_R32G32B32_SFLOAT, "inPosition"   },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_NORMAL,      12, VK_FORMAT_R32G32B32_SFLOAT, "inNormal"     },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_BINORMAL,    12, VK_FORMAT_R32G32B32_SFLOAT, "inBinormal"   },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_COLOR,       12, VK_FORMAT_R32G32B32_SFLOAT, "inColor"      },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX,  8,  VK_FORMAT_R32G32_SFLOAT,    "inSkinIndex"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT, 8,  VK_FORMAT_R32G32_SFLOAT,    "inSkinWeight" },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD0,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord0"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD1,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord1"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD2,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord2"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD3,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord3"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD4,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord4"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD5,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord5"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD6,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord6"  },
		{ CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD7,   8,  VK_FORMAT_R32G32_SFLOAT,    "inTexcoord7"  },
	};

	uint32_t CRendererDevice::GetVertexSize(uint32_t format) const
	{
		uint32_t size = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (format & vertexAttributes[indexAttribute].flag) {
				size += vertexAttributes[indexAttribute].size;
			}
		}

		return size;
	}

	uint32_t CRendererDevice::GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const
	{
		uint32_t offset = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) return offset;
			if (format & vertexAttributes[indexAttribute].flag) offset += vertexAttributes[indexAttribute].size;
		}

		return -1;
	}

	uint32_t CRendererDevice::GetVertexAttributeFlag(const char *szName) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (strcmp(vertexAttributes[indexAttribute].name, szName) == 0) {
				return vertexAttributes[indexAttribute].flag;
			}
		}

		return 0;
	}

	VkFormat CRendererDevice::GetVertexAttributeFormat(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) {
				return vertexAttributes[indexAttribute].format;
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

}
