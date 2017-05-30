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

	class CROSS_EXPORT CGfxTexture : public CGfxResource
	{
	protected:
		CGfxTexture(CGfxResourceManager *pResourceManager)
			: CGfxResource(pResourceManager)
		{

		}
		virtual ~CGfxTexture(void)
		{

		}


	public:
		virtual BOOL CreateTexture2D(const gli::texture2d &texture, TextureFilter minFilter, TextureFilter magFilter, SamplerMipmapMode mipmapMode, SamplerAddressMode addressMode) = 0;
		virtual BOOL CreateTexture2DArray(const gli::texture2d_array &texture, TextureFilter minFilter, TextureFilter magFilter, SamplerMipmapMode mipmapMode, SamplerAddressMode addressMode) = 0;
		virtual BOOL CreateTextureCube(const gli::texture_cube &texture, TextureFilter minFilter, TextureFilter magFilter, SamplerMipmapMode mipmapMode, SamplerAddressMode addressMode) = 0;
		virtual void Destroy(void) = 0;
		virtual void DumpLog(void) const = 0;

	public:
		virtual size_t GetMemorySize(void) const = 0;
	};

}
