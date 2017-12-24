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
		uint32_t location;
		VkFormat format;
		const char *name;
	};

	static const VERTEX_ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_FLAG_COUNT] = {
		{ VERTEX_ATTRIBUTE_FLAG_POSITION,             3, 0, VK_FORMAT_R32G32B32_SFLOAT,    "inPosition"            },
		{ VERTEX_ATTRIBUTE_FLAG_NORMAL,               3, 1, VK_FORMAT_R32G32B32_SFLOAT,    "inNormal"              },
		{ VERTEX_ATTRIBUTE_FLAG_BINORMAL,             3, 2, VK_FORMAT_R32G32B32_SFLOAT,    "inBinormal"            },
		{ VERTEX_ATTRIBUTE_FLAG_COLOR,                3, 3, VK_FORMAT_R32G32B32_SFLOAT,    "inColor"               },
		{ VERTEX_ATTRIBUTE_FLAG_SKIN,                 4, 4, VK_FORMAT_R32G32B32A32_SFLOAT, "inSkin"                },
		{ VERTEX_ATTRIBUTE_FLAG_TEXCOORD,             4, 5, VK_FORMAT_R32G32B32A32_SFLOAT, "inTexcoord"            },

		{ VERTEX_ATTRIBUTE_FLAG_INSTANCE_POSITION,    3, 8, VK_FORMAT_R32G32B32_SFLOAT,    "inInstancePosition"    },
		{ VERTEX_ATTRIBUTE_FLAG_INSTANCE_ORIENTATION, 4, 9, VK_FORMAT_R32G32B32A32_SFLOAT, "inInstanceOrientation" },
	};


	uint32_t CGfxDevice::GetVertexStride(uint32_t format) const
	{
		uint32_t stride = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (format & vertexAttributes[indexAttribute].flag) {
				stride += vertexAttributes[indexAttribute].size * sizeof(float);
			}
		}

		return stride;
	}

	uint32_t CGfxDevice::GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const
	{
		uint32_t offset = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) return offset;
			if (format & vertexAttributes[indexAttribute].flag) offset += vertexAttributes[indexAttribute].size * sizeof(float);
		}

		return -1;
	}

	uint32_t CGfxDevice::GetVertexAttributeFlag(const char *szName) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (strcmp(vertexAttributes[indexAttribute].name, szName) == 0) {
				return vertexAttributes[indexAttribute].flag;
			}
		}

		return 0;
	}

	uint32_t CGfxDevice::GetVertexAttributeSize(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) {
				return vertexAttributes[indexAttribute].size;
			}
		}

		return 0;
	}

	uint32_t CGfxDevice::GetVertexAttributeLocation(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) {
				return vertexAttributes[indexAttribute].location;
			}
		}

		return -1;
	}

	VkFormat CGfxDevice::GetVertexAttributeFormat(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == vertexAttributes[indexAttribute].flag) {
				return vertexAttributes[indexAttribute].format;
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

}
