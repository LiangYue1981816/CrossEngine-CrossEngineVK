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

	class CROSS_EXPORT CGLES3Texture : public CGfxTexture, public CGLES3Image
	{
		friend class CGLES3TextureManager;


	protected:
		CGLES3Texture(CGfxResourceManager *pResourceManager);
		virtual ~CGLES3Texture(void);


	public:
		HANDLE GetHandle(void) const;

	public:
		BOOL CreateTexture2D(const gli::texture2d &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		BOOL CreateTexture2DArray(const gli::texture2d_array &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		BOOL CreateTextureCube(const gli::texture_cube &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		void Destroy(void);
		void DumpLog(void) const;

	protected:
		BOOL TransferTexture2D(const gli::texture2d &texture, const gli::gl::format &format);
		BOOL TransferTexture2DArray(const gli::texture2d_array &texture, const gli::gl::format &format);
		BOOL TransferTextureCube(const gli::texture_cube &texture, const gli::gl::format &format);

	public:
		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		uint32_t GetDepth(void) const;
		uint32_t GetSamples(void) const;

	public:
		size_t GetMemorySize(void) const;
	};

}
