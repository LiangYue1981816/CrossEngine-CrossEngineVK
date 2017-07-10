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
		, m_samples(1)

		, m_minFilter(GL_NEAREST_MIPMAP_LINEAR)
		, m_magFilter(GL_LINEAR)
		, m_addressMode(GL_REPEAT)

		, m_target(GL_TEXTURE_2D)
		, m_format(GL_RGBA)
		, m_internalFormat(GL_RGBA)

		, m_size(0)
	{

	}

	CGLES3Image::~CGLES3Image(void)
	{

	}

	BOOL CGLES3Image::Create(GLenum target, GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint mipLevels, GLint arrayLayers, GLsizei samples, GLenum minFilter, GLenum magFilter, GLenum addressMode)
	{
		try {
			CALL_GL_FUNCTION_THROW(CreateImage(target, format, internalFormat, width, height, depth, mipLevels, arrayLayers, samples));
			CALL_GL_FUNCTION_THROW(CreateSampler(minFilter, magFilter, addressMode));

			return TRUE;
		}
		catch (int) {
			Destroy();

			return FALSE;
		}
	}

	int CGLES3Image::CreateImage(GLenum target, GLenum format, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint mipLevels, GLint arrayLayers, GLsizei samples)
	{
		m_target = target;
		m_format = format;
		m_internalFormat = internalFormat;

		m_width = width;
		m_height = height;
		m_depth = depth;
		m_mipLevels = mipLevels;
		m_arrayLayers = arrayLayers;
		m_samples = samples;

		switch (m_target) {
		case GL_TEXTURE_2D:
		case GL_TEXTURE_CUBE_MAP:
			if (m_samples > 1) {
				glGenTextures(1, &m_texture);
				glTexStorage2DMultisample(m_target, m_samples, m_internalFormat, m_width, m_height, GL_TRUE);
			}
			else {
				glGenTextures(1, &m_texture);
				glTexStorage2D(m_target, m_mipLevels, m_internalFormat, m_width, m_height);
			}
			break;

		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_3D:
			m_samples = 1;
			glGenTextures(1, &m_texture);
			glTexStorage3D(m_target, m_mipLevels, m_internalFormat, m_width, m_height, m_target == GL_TEXTURE_3D ? m_depth : m_arrayLayers);
			break;

		default:
			return GL_INVALID_ENUM;
		}

		return NO_ERROR;
	}

	int CGLES3Image::CreateSampler(GLenum minFilter, GLenum magFilter, GLenum addressMode)
	{
		m_minFilter = minFilter;
		m_magFilter = magFilter;
		m_addressMode = addressMode;

		glGenSamplers(1, &m_sampler);
		glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, m_minFilter);
		glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, m_magFilter);
		glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, m_addressMode);
		glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, m_addressMode);
		glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, m_addressMode);

		return NO_ERROR;
	}

	void CGLES3Image::Destroy(void)
	{
		glDeleteTextures(1, &m_texture);
		glDeleteSamplers(1, &m_sampler);
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
