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

	class CROSS_EXPORT CGLES3Image
	{
	protected:
		CGLES3Image(void);
		virtual ~CGLES3Image(void);


	protected:
		BOOL Create(GLenum target, GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint mipLevels, GLint arrayLayers, GLsizei samples, GLenum minFilter, GLenum magFilter, GLenum addressMode);
		int CreateImage(GLenum target, GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint mipLevels, GLint arrayLayers, GLsizei samples);
		int CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode);
		void Destroy(void);

	public:
		GLenum GetTarget(void) const;
		GLenum GetFormat(void) const;


	protected:
		GLint m_width;
		GLint m_height;
		GLint m_depth;
		GLint m_mipLevels;
		GLint m_arrayLayers;
		GLint m_samples;

		GLenum m_minFilter;
		GLenum m_magFilter;
		GLenum m_addressMode;

		GLenum m_target;
		GLenum m_format;
		GLenum m_internalFormat;

		GLuint m_size;

	protected:
		GLuint m_texture;
		GLuint m_sampler;
	};

}
