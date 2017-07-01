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

	CGLES3Texture::CGLES3Texture(CGfxResourceManager *pResourceManager)
		: CGfxTexture(pResourceManager)
	{

	}

	CGLES3Texture::~CGLES3Texture(void)
	{

	}

	HANDLE CGLES3Texture::GetHandle(void) const
	{
		return (HANDLE)m_texture;
	}

	BOOL CGLES3Texture::CreateTexture2D(const gli::texture2d &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		CALL_BOOL_FUNCTION_RETURN(Create(GL_TEXTURE_2D, format.External, format.Internal, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, 1, CGLES3Helper::glTranslateMinFilter(minFilter, mipmapMode), CGLES3Helper::glTranslateMagFilter(magFilter), CGLES3Helper::glTranslateAddressMode(addressMode)));
		CALL_BOOL_FUNCTION_RETURN(TransferTexture2D(texture, format));

		m_size = texture.size();

		return TRUE;
	}

	BOOL CGLES3Texture::CreateTexture2DArray(const gli::texture2d_array &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		CALL_BOOL_FUNCTION_RETURN(Create(GL_TEXTURE_2D_ARRAY, format.External, format.Internal, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), texture.layers(), 1, CGLES3Helper::glTranslateMinFilter(minFilter, mipmapMode), CGLES3Helper::glTranslateMagFilter(magFilter), CGLES3Helper::glTranslateAddressMode(addressMode)));
		CALL_BOOL_FUNCTION_RETURN(TransferTexture2DArray(texture, format));

		m_size = texture.size();

		return TRUE;
	}

	BOOL CGLES3Texture::CreateTextureCube(const gli::texture_cube &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

		CALL_BOOL_FUNCTION_RETURN(Create(GL_TEXTURE_CUBE_MAP, format.External, format.Internal, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, 1, CGLES3Helper::glTranslateMinFilter(minFilter, mipmapMode), CGLES3Helper::glTranslateMagFilter(magFilter), CGLES3Helper::glTranslateAddressMode(addressMode)));
		CALL_BOOL_FUNCTION_RETURN(TransferTextureCube(texture, format));

		m_size = texture.size();

		return TRUE;
	}

	void CGLES3Texture::Destroy(void)
	{
		CGLES3Image::Destroy();
	}

	BOOL CGLES3Texture::TransferTexture2D(const gli::texture2d &texture, const gli::gl::format &format)
	{
		for (int layer = 0; layer < texture.layers(); layer++) {
			for (int face = 0; face < texture.faces(); face++) {
				for (int level = 0; level < texture.levels(); level++) {
					if (gli::is_compressed(texture.format())) {
						glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture.extent(level).x, texture.extent(level).y, format.Internal, texture.size(level), texture.data(layer, face, level));
					}
					else {
						glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture.extent(level).x, texture.extent(level).y, format.External, format.Type, texture.data(layer, face, level));
					}
				}
			}
		}

		return TRUE;
	}

	BOOL CGLES3Texture::TransferTexture2DArray(const gli::texture2d_array &texture, const gli::gl::format &format)
	{
		for (int layer = 0; layer < texture.layers(); layer++) {
			for (int face = 0; face < texture.faces(); face++) {
				for (int level = 0; level < texture.levels(); level++) {
					if (gli::is_compressed(texture.format())) {
						glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, 0, texture.extent(level).x, texture.extent(level).y, layer, format.Internal, texture.size(level), texture.data(layer, face, level));
					}
					else {
						glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, 0, texture.extent(level).x, texture.extent(level).y, layer, format.External, format.Type, texture.data(layer, face, level));
					}
				}
			}
		}

		return TRUE;
	}

	BOOL CGLES3Texture::TransferTextureCube(const gli::texture_cube &texture, const gli::gl::format &format)
	{
		for (int layer = 0; layer < texture.layers(); layer++) {
			for (int face = 0; face < texture.faces(); face++) {
				for (int level = 0; level < texture.levels(); level++) {
					if (gli::is_compressed(texture.format())) {
						glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP, level, 0, 0, texture.extent(level).x, texture.extent(level).y, format.Internal, texture.size(level), texture.data(layer, face, level));
					}
					else {
						glTexSubImage2D(GL_TEXTURE_CUBE_MAP, level, 0, 0, texture.extent(level).x, texture.extent(level).y, format.External, format.Type, texture.data(layer, face, level));
					}
				}
			}
		}

		return TRUE;
	}

	uint32_t CGLES3Texture::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3Texture::GetHeight(void) const
	{
		return m_height;
	}

	uint32_t CGLES3Texture::GetDepth(void) const
	{
		return m_depth;
	}

	uint32_t CGLES3Texture::GetSamples(void) const
	{
		return m_samples;
	}

	size_t CGLES3Texture::GetMemorySize(void) const
	{
		return m_size;
	}

	void CGLES3Texture::DumpLog(void) const
	{
		if (m_texture) {
			LOGI("\t\tTexture 0x%x: sampler = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s min filter = %s mag filter = %s address mode = %s\n",
				m_texture,
				m_sampler,
				m_size,
				CGLES3Helper::glEnumToString(m_target),
				CGLES3Helper::glEnumToString(m_format),
				m_width, m_height, m_depth, m_mipLevels, m_arrayLayers,
				m_samples,
				CGLES3Helper::glEnumToString(m_minFilter),
				CGLES3Helper::glEnumToString(m_magFilter),
				CGLES3Helper::glEnumToString(m_addressMode));
		}
	}

}
