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

	const char* CGLES3Helper::glAttachmentLoadOpToString(GLAttachmentLoadOp loadOp)
	{
		static char szString[_MAX_STRING];

		switch (loadOp) {
		case GL_ATTACHMENT_LOAD_OP_LOAD: strcpy(szString, "GL_ATTACHMENT_LOAD_OP_LOAD"); break;
		case GL_ATTACHMENT_LOAD_OP_CLEAR: strcpy(szString, "GL_ATTACHMENT_LOAD_OP_CLEAR"); break;
		case GL_ATTACHMENT_LOAD_OP_DONT_CARE: strcpy(szString, "GL_ATTACHMENT_LOAD_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CGLES3Helper::glAttachmentStoreOpToString(GLAttachmentStoreOp storeOp)
	{
		static char szString[_MAX_STRING];

		switch (storeOp) {
		case GL_ATTACHMENT_STORE_OP_STORE: strcpy(szString, "GL_ATTACHMENT_STORE_OP_STORE"); break;
		case GL_ATTACHMENT_STORE_OP_DONT_CARE: strcpy(szString, "GL_ATTACHMENT_STORE_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

}
