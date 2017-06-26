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

	const char* CGLES3Helper::glUsageToString(GLenum usage)
	{
		static char szString[_MAX_STRING];

		switch (usage) {
		case GL_STREAM_DRAW: strcpy(szString, "GL_STREAM_DRAW"); break;
		case GL_STREAM_READ: strcpy(szString, "GL_STREAM_READ"); break;
		case GL_STREAM_COPY: strcpy(szString, "GL_STREAM_COPY"); break;
		case GL_STATIC_DRAW: strcpy(szString, "GL_STATIC_DRAW"); break;
		case GL_STATIC_READ: strcpy(szString, "GL_STATIC_READ"); break;
		case GL_STATIC_COPY: strcpy(szString, "GL_STATIC_COPY"); break;
		case GL_DYNAMIC_DRAW: strcpy(szString, "GL_DYNAMIC_DRAW"); break;
		case GL_DYNAMIC_READ: strcpy(szString, "GL_DYNAMIC_READ"); break;
		case GL_DYNAMIC_COPY: strcpy(szString, "GL_DYNAMIC_COPY"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

}
