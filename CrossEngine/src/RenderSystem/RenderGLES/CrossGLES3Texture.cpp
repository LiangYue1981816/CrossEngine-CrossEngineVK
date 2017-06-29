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

	BOOL CGLES3Texture::CreateTexture2D(const gli::texture2d &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		return TRUE;
	}

	BOOL CGLES3Texture::CreateTexture2DArray(const gli::texture2d_array &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		return TRUE;
	}

	BOOL CGLES3Texture::CreateTextureCube(const gli::texture_cube &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
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

}
