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

	CGLES3Image::CGLES3Image(void)
		: m_texture(0)
		, m_sampler(0)

		, m_width(0)
		, m_height(0)
		, m_depth(0)
		, m_mipLevels(0)
		, m_arrayLayers(0)

		, m_target(GL_TEXTURE_2D)
		, m_format(GL_RGBA)
		, m_internalFormat(GL_RGBA)
	{

	}

	CGLES3Image::~CGLES3Image(void)
	{

	}

	int CGLES3Image::CreateImage(GLenum target, GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint mipLevels, GLint arrayLayers, GLsizei samples)
	{
		return NO_ERROR;
	}

	int CGLES3Image::CreateSampler(GLenum minFilter, GLenum maxFilter, GLenum addressMode)
	{
		return NO_ERROR;
	}

	void CGLES3Image::Destroy(void)
	{
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}

		if (m_sampler) {
			glDeleteSamplers(1, &m_sampler);
		}

		m_texture = 0;
		m_sampler = 0;
	}

	GLenum CGLES3Image::GetTarget(void) const
	{
		return m_target;
	}

	GLenum CGLES3Image::GetFormat(void) const
	{
		return m_format;
	}

}
