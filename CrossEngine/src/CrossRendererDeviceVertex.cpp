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

	static const uint32_t vertexAttributeFlags[CRendererDevice::ATTRIBUTE_COUNT] = {
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_POSITION,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_NORMAL,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_BINORMAL,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_COLOR,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD0,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD1,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD2,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD3,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD4,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD5,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD6,
		CRendererDevice::VERTEX_ATTRIBUTE_FLAG_TEXCOORD7
	};

	static const VkFormat vertexAttributeFormats[CRendererDevice::ATTRIBUTE_COUNT] = {
		VK_FORMAT_R32G32B32_SFLOAT, // ATTRIBUTE_POSITION
		VK_FORMAT_R32G32B32_SFLOAT, // ATTRIBUTE_NORMAL
		VK_FORMAT_R32G32B32_SFLOAT, // ATTRIBUTE_BINORMAL
		VK_FORMAT_R32G32B32_SFLOAT, // ATTRIBUTE_COLOR
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_SKIN_INDEX
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_SKIN_WEIGHT
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD0
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD1
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD2
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD3
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD4
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD5
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD6
		VK_FORMAT_R32G32_SFLOAT,    // ATTRIBUTE_TEXCOORD7
	};

	static const uint32_t vertexAttributeSizes[CRendererDevice::ATTRIBUTE_COUNT] = {
		12, // ATTRIBUTE_POSITION
		12, // ATTRIBUTE_NORMAL
		12, // ATTRIBUTE_BINORMAL
		12, // ATTRIBUTE_COLOR
		8,  // ATTRIBUTE_SKIN_INDEX
		8,  // ATTRIBUTE_SKIN_WEIGHT
		8,  // ATTRIBUTE_TEXCOORD0
		8,  // ATTRIBUTE_TEXCOORD1
		8,  // ATTRIBUTE_TEXCOORD2
		8,  // ATTRIBUTE_TEXCOORD3
		8,  // ATTRIBUTE_TEXCOORD4
		8,  // ATTRIBUTE_TEXCOORD5
		8,  // ATTRIBUTE_TEXCOORD6
		8   // ATTRIBUTE_TEXCOORD7
	};

	static const char* vertexAttributeNames[CRendererDevice::ATTRIBUTE_COUNT] = {
		"inPosition",   // ATTRIBUTE_POSITION
		"inNormal",     // ATTRIBUTE_NORMAL
		"inBinormal",   // ATTRIBUTE_BINORMAL
		"inColor",      // ATTRIBUTE_COLOR
		"inSkinIndex",  // ATTRIBUTE_SKIN_INDEX
		"inSkinWeight", // ATTRIBUTE_SKIN_WEIGHT
		"inTexcoord0",  // ATTRIBUTE_TEXCOORD0
		"inTexcoord1",  // ATTRIBUTE_TEXCOORD1
		"inTexcoord2",  // ATTRIBUTE_TEXCOORD2
		"inTexcoord3",  // ATTRIBUTE_TEXCOORD3
		"inTexcoord4",  // ATTRIBUTE_TEXCOORD4
		"inTexcoord5",  // ATTRIBUTE_TEXCOORD5
		"inTexcoord6",  // ATTRIBUTE_TEXCOORD6
		"inTexcoord7"   // ATTRIBUTE_TEXCOORD7
	};

	uint32_t CRendererDevice::GetVertexSize(uint32_t format) const
	{
		uint32_t size = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (format & vertexAttributeFlags[indexAttribute]) {
				size += vertexAttributeSizes[indexAttribute];
			}
		}

		return size;
	}

	uint32_t CRendererDevice::GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const
	{
		uint32_t offset = 0;

		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (attribute == vertexAttributeFlags[indexAttribute]) return offset;
			if (format & vertexAttributeFlags[indexAttribute]) offset += vertexAttributeSizes[indexAttribute];
		}

		return -1;
	}

	uint32_t CRendererDevice::GetVertexAttributeFlag(const char *szName) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (strcmp(vertexAttributeNames[indexAttribute], szName) == 0) {
				return vertexAttributeFlags[indexAttribute];
			}
		}

		return 0;
	}

	VkFormat CRendererDevice::GetVertexAttributeFormat(uint32_t attribute) const
	{
		for (uint32_t indexAttribute = 0; indexAttribute < CRendererDevice::ATTRIBUTE_COUNT; indexAttribute++) {
			if (attribute == vertexAttributeFlags[indexAttribute]) {
				return vertexAttributeFormats[indexAttribute];
			}
		}

		return VK_FORMAT_UNDEFINED;
	}

}
