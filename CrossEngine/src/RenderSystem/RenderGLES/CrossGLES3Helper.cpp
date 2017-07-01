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

	typedef struct {
		GLuint param;
		GLchar name[64];
	} String;

	typedef struct {
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

	GLboolean CGLES3Helper::glTranslateFormat(VkFormat format, GLenum &internalFormat, GLenum &externalFormat, GLenum &type)
	{
		for (GLint index = 0; index < sizeof(formats) / sizeof(Format); index++) {
			if (formats[index].format == format) {
				internalFormat = formats[index].internalFormat;
				externalFormat = formats[index].externalFormat;
				type = formats[index].type;
				return GL_TRUE;
			}
		}

		return GL_FALSE;
	}

}
