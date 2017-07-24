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

	typedef struct String {
		GLuint64 param;
		GLchar name[64];
	} String;

	typedef struct Format {
		VkFormat format;
		GLenum internalFormat;
		GLenum externalFormat;
		GLenum type;
	} Format;

	static const Format formats[] = {
		#include "CrossGLES3Format.h"
	};

	static const String strings[] = {
		#include "CrossGLES3String.h"
	};

	const char* CGLES3Helper::glEnumToString(GLenum param)
	{
		for (GLint index = 0; index < sizeof(strings) / sizeof(String); index++) {
			if (strings[index].param == param) {
				return strings[index].name;
			}
		}

		return "unknown";
	}

	GLboolean CGLES3Helper::glIsFormatDepthOnly(GLenum format)
	{
		return format == GL_DEPTH_COMPONENT ? GL_TRUE : GL_FALSE;
	}

	GLboolean CGLES3Helper::glIsFormatStencilOnly(GLenum format)
	{
		return format == GL_STENCIL_INDEX ? GL_TRUE : GL_FALSE;
	}

	GLboolean CGLES3Helper::glIsFormatDepthStencil(GLenum format)
	{
		return format == GL_DEPTH_STENCIL ? GL_TRUE : GL_FALSE;
	}

	GLenum CGLES3Helper::glTranslateFormat(VkFormat format, GLenum &internalFormat, GLenum &externalFormat, GLenum &type)
	{
		for (GLint index = 0; index < sizeof(formats) / sizeof(Format); index++) {
			if (formats[index].format == format) {
				internalFormat = formats[index].internalFormat;
				externalFormat = formats[index].externalFormat;
				type = formats[index].type;
				return GL_NO_ERROR;
			}
		}

		return GL_INVALID_ENUM;
	}

	GLenum CGLES3Helper::glTranslateMagFilter(VkFilter magFilter)
	{
		switch (magFilter) {
		case VK_FILTER_NEAREST: return GL_NEAREST;
		case VK_FILTER_LINEAR: return GL_LINEAR;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateMinFilter(VkFilter minFilter, VkSamplerMipmapMode mipmapMode)
	{
		switch (minFilter) {
		case VK_FILTER_NEAREST:
			switch (mipmapMode) {
			case VK_SAMPLER_MIPMAP_MODE_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			case VK_SAMPLER_MIPMAP_MODE_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
			default: return GL_INVALID_ENUM;
			}
			break;
		case VK_FILTER_LINEAR:
			switch (mipmapMode) {
			case VK_SAMPLER_MIPMAP_MODE_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
			case VK_SAMPLER_MIPMAP_MODE_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
			default: return GL_INVALID_ENUM;
			}
			break;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateAddressMode(VkSamplerAddressMode addressMode)
	{
		switch (addressMode) {
		case VK_SAMPLER_ADDRESS_MODE_REPEAT: return GL_REPEAT;
		case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
//		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateStencilOp(VkStencilOp op)
	{
		switch (op) {
		case VK_STENCIL_OP_KEEP: return GL_KEEP;
		case VK_STENCIL_OP_ZERO: return GL_ZERO;
		case VK_STENCIL_OP_REPLACE: return GL_REPLACE;
		case VK_STENCIL_OP_INCREMENT_AND_CLAMP: return GL_INCR;
		case VK_STENCIL_OP_DECREMENT_AND_CLAMP: return GL_DECR;
		case VK_STENCIL_OP_INVERT: return GL_INVERT;
		case VK_STENCIL_OP_INCREMENT_AND_WRAP: return GL_INCR_WRAP;
		case VK_STENCIL_OP_DECREMENT_AND_WRAP: return GL_DECR_WRAP;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateCompareOp(VkCompareOp op)
	{
		switch (op) {
		case VK_COMPARE_OP_NEVER: return GL_NEVER;
		case VK_COMPARE_OP_LESS: return GL_LESS;
		case VK_COMPARE_OP_EQUAL: return GL_EQUAL;
		case VK_COMPARE_OP_LESS_OR_EQUAL: return GL_LEQUAL;
		case VK_COMPARE_OP_GREATER: return GL_GREATER;
		case VK_COMPARE_OP_NOT_EQUAL: return GL_NOTEQUAL;
		case VK_COMPARE_OP_GREATER_OR_EQUAL: return GL_GEQUAL;
		case VK_COMPARE_OP_ALWAYS: return GL_ALWAYS;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateBlendOp(VkBlendOp op)
	{
		switch (op) {
		case VK_BLEND_OP_ADD: return GL_FUNC_ADD;
		case VK_BLEND_OP_SUBTRACT: return GL_FUNC_SUBTRACT;
		case VK_BLEND_OP_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
		case VK_BLEND_OP_MIN: return GL_MIN;
		case VK_BLEND_OP_MAX: return GL_MAX;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateBlendFactor(VkBlendFactor factor)
	{
		switch (factor) {
		case VK_BLEND_FACTOR_ZERO: return GL_ZERO;
		case VK_BLEND_FACTOR_ONE: return GL_ONE;
		case VK_BLEND_FACTOR_SRC_COLOR: return GL_SRC_COLOR;
		case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case VK_BLEND_FACTOR_DST_COLOR: return GL_DST_COLOR;
		case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case VK_BLEND_FACTOR_SRC_ALPHA: return GL_SRC_ALPHA;
		case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case VK_BLEND_FACTOR_DST_ALPHA: return GL_DST_ALPHA;
		case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case VK_BLEND_FACTOR_CONSTANT_COLOR: return GL_CONSTANT_COLOR;
		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
		case VK_BLEND_FACTOR_CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
		case VK_BLEND_FACTOR_SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
		//case VK_BLEND_FACTOR_SRC1_COLOR:
		//case VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
		//case VK_BLEND_FACTOR_SRC1_ALPHA:
		//case VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateCullMode(VkCullModeFlags cullMode)
	{
		switch (cullMode) {
		case VK_CULL_MODE_FRONT_BIT: return GL_FRONT;
		case VK_CULL_MODE_BACK_BIT: return GL_BACK;
		case VK_CULL_MODE_FRONT_AND_BACK: return GL_FRONT_AND_BACK;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateFrontFace(VkFrontFace frontFace)
	{
		switch (frontFace) {
		case VK_FRONT_FACE_COUNTER_CLOCKWISE: return GL_CCW;
		case VK_FRONT_FACE_CLOCKWISE: return GL_CW;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslateStencilFace(VkStencilFaceFlags stencilFace)
	{
		switch (stencilFace) {
		case VK_STENCIL_FACE_FRONT_BIT: return GL_FRONT;
		case VK_STENCIL_FACE_BACK_BIT: return GL_BACK;
		case VK_STENCIL_FRONT_AND_BACK: return GL_FRONT_AND_BACK;
		default: return GL_INVALID_ENUM;
		}
	}

	GLenum CGLES3Helper::glTranslatePrimitiveTopology(VkPrimitiveTopology topology)
	{
		switch (topology) {
		case VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return GL_POINTS;
		case VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return GL_LINES;
		case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return GL_LINE_STRIP;
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return GL_TRIANGLES;
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		//case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:
		//case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:
		//case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:
		//case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY:
		//case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST:
		default: return GL_INVALID_ENUM;
		}
	}

	const char* CGLES3Helper::glAttachmentLoadOpToString(VkAttachmentLoadOp loadOp)
	{
		static char szString[_MAX_STRING];

		switch (loadOp) {
		case VK_ATTACHMENT_LOAD_OP_LOAD: strcpy(szString, "ATTACHMENT_LOAD_OP_LOAD"); break;
		case VK_ATTACHMENT_LOAD_OP_CLEAR: strcpy(szString, "ATTACHMENT_LOAD_OP_CLEAR"); break;
		case VK_ATTACHMENT_LOAD_OP_DONT_CARE: strcpy(szString, "ATTACHMENT_LOAD_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CGLES3Helper::glAttachmentStoreOpToString(VkAttachmentStoreOp storeOp)
	{
		static char szString[_MAX_STRING];

		switch (storeOp) {
		case VK_ATTACHMENT_STORE_OP_STORE: strcpy(szString, "ATTACHMENT_STORE_OP_STORE"); break;
		case VK_ATTACHMENT_STORE_OP_DONT_CARE: strcpy(szString, "ATTACHMENT_STORE_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

}
