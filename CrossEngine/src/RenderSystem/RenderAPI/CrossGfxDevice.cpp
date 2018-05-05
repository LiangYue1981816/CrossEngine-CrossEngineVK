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

	struct ATTRIBUTE {
		uint32_t flag;
		uint32_t size;
		uint32_t location;
		VkFormat format;
		const char *name;
	};

	static const ATTRIBUTE attributes[ATTRIBUTE_FLAG_COUNT] = {
		{ VERTEX_ATTRIBUTE_POSITION,  3, 0, VK_FORMAT_R32G32B32_SFLOAT,    "inPosition"  },
		{ VERTEX_ATTRIBUTE_NORMAL,    3, 1, VK_FORMAT_R32G32B32_SFLOAT,    "inNormal"    },
		{ VERTEX_ATTRIBUTE_BINORMAL,  3, 2, VK_FORMAT_R32G32B32_SFLOAT,    "inBinormal"  },
		{ VERTEX_ATTRIBUTE_COLOR,     3, 3, VK_FORMAT_R32G32B32_SFLOAT,    "inColor"     },
		{ VERTEX_ATTRIBUTE_TEXCOORD0, 2, 4, VK_FORMAT_R32G32_SFLOAT,       "inTexcoord0" },
		{ VERTEX_ATTRIBUTE_TEXCOORD1, 2, 5, VK_FORMAT_R32G32_SFLOAT,       "inTexcoord1" },
		{ VERTEX_ATTRIBUTE_INDICES,   4, 6, VK_FORMAT_R32G32B32A32_SFLOAT, "inIndices"   },
		{ VERTEX_ATTRIBUTE_WEIGHTS,   4, 7, VK_FORMAT_R32G32B32A32_SFLOAT, "inWeights"   },

		{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 8,  VK_FORMAT_R32G32B32A32_SFLOAT, "inInstanceTransformMatrixCol0" },
		{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 9,  VK_FORMAT_R32G32B32A32_SFLOAT, "inInstanceTransformMatrixCol1" },
		{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 10, VK_FORMAT_R32G32B32A32_SFLOAT, "inInstanceTransformMatrixCol2" },
		{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 11, VK_FORMAT_R32G32B32A32_SFLOAT, "inInstanceTransformMatrixCol3" },
	};


	uint32_t CGfxDevice::GetStride(uint32_t format) const
	{
		uint32_t stride = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (format & attributes[indexAttribute].flag) {
				stride += attributes[indexAttribute].size * 4;
			}
		}

		return stride;
	}

	uint32_t CGfxDevice::GetAttributeFlag(const char *szName) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (strcmp(attributes[indexAttribute].name, szName) == 0) {
				return attributes[indexAttribute].flag;
			}
		}

		return 0;
	}

	uint32_t CGfxDevice::GetAttributeOffset(uint32_t format, uint32_t attribute) const
	{
		uint32_t offset = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == attributes[indexAttribute].flag) return offset;
			if (format & attributes[indexAttribute].flag) offset += attributes[indexAttribute].size * 4;
		}

		return -1;
	}

	uint32_t CGfxDevice::GetAttributeSize(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == attributes[indexAttribute].flag) {
				return attributes[indexAttribute].size;
			}
		}

		return 0;
	}

	uint32_t CGfxDevice::GetAttributeLocation(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == attributes[indexAttribute].flag) {
				return attributes[indexAttribute].location;
			}
		}

		return -1;
	}

	VkFormat CGfxDevice::GetAttributeFormat(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < ATTRIBUTE_FLAG_COUNT; indexAttribute++) {
			if (attribute == attributes[indexAttribute].flag) {
				return attributes[indexAttribute].format;
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

}
