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

	CGLES3Image::CGLES3Image(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_size(0)
		, m_texture(0)
		, m_sampler(0)

		, m_width(0)
		, m_height(0)
		, m_depth(0)
		, m_mipLevels(0)
		, m_arrayLayers(0)

		, m_target(GL_TEXTURE_2D)
		, m_format(GL_RGBA)
		, m_internalFormat(GL_RGBA)
		, m_samples(1)

		, m_minFilter(GL_NEAREST_MIPMAP_LINEAR)
		, m_magFilter(GL_LINEAR)
		, m_addressMode(GL_REPEAT)
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

	void CGLES3Image::Destroy(void)
	{
		DestroyImage();
		DestroySampler();

		m_width = 0;
		m_height = 0;
		m_depth = 0;
		m_mipLevels = 0;
		m_arrayLayers = 0;

		m_target = GL_TEXTURE_2D;
		m_format = GL_RGBA;
		m_internalFormat = GL_RGBA;
		m_samples = 1;

		m_minFilter = GL_NEAREST_MIPMAP_LINEAR;
		m_magFilter = GL_LINEAR;
		m_addressMode = GL_REPEAT;
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

		switch (m_target) {
		case GL_TEXTURE_2D:
			m_samples = 1;
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			{
				glTexStorage2D(GL_TEXTURE_2D, m_mipLevels, m_internalFormat, m_width, m_height);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			break;

		case GL_TEXTURE_2D_MULTISAMPLE:
			m_samples = samples;
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
			{
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, m_internalFormat, m_width, m_height, GL_TRUE);
			}
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			break;

		case GL_TEXTURE_2D_ARRAY:
			m_samples = 1;
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
			{
				glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_mipLevels, m_internalFormat, m_width, m_height, m_arrayLayers);
			}
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
			break;

		case GL_TEXTURE_3D:
			m_samples = 1;
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_3D, m_texture);
			{
				glTexStorage3D(GL_TEXTURE_3D, m_mipLevels, m_internalFormat, m_width, m_height, m_depth);
			}
			glBindTexture(GL_TEXTURE_3D, 0);
			break;

		case GL_TEXTURE_CUBE_MAP:
			m_samples = 1;
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
			{
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_mipLevels, m_internalFormat, m_width, m_height);
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_mipLevels, m_internalFormat, m_width, m_height);
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_mipLevels, m_internalFormat, m_width, m_height);
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_mipLevels, m_internalFormat, m_width, m_height);
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_mipLevels, m_internalFormat, m_width, m_height);
				glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_mipLevels, m_internalFormat, m_width, m_height);
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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

	void CGLES3Image::DestroyImage(void)
	{
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}

		m_size = 0;
		m_texture = 0;
	}

	void CGLES3Image::DestroySampler(void)
	{
		if (m_sampler) {
			glDeleteSamplers(1, &m_sampler);
		}

		m_sampler = 0;
	}

	uint32_t CGLES3Image::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3Image::GetHeight(void) const
	{
		return m_height;
	}

	uint32_t CGLES3Image::GetDepth(void) const
	{
		return m_depth;
	}

	uint32_t CGLES3Image::GetSamples(void) const
	{
		return m_samples;
	}

	uint32_t CGLES3Image::GetMipLevels(void) const
	{
		return m_mipLevels;
	}

	uint32_t CGLES3Image::GetArrayLayers(void) const
	{
		return m_arrayLayers;
	}

	GLenum CGLES3Image::GetTarget(void) const
	{
		return m_target;
	}

	GLenum CGLES3Image::GetFormat(void) const
	{
		return m_format;
	}

	GLuint CGLES3Image::GetSampler(void) const
	{
		return m_sampler;
	}

	size_t CGLES3Image::GetMemorySize(void) const
	{
		return m_size;
	}

}
