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

	class CROSS_EXPORT CGLES3RenderTexture : public CGfxRenderTexture, public CGLES3Image
	{
	protected:
		CGLES3RenderTexture(CGfxResourceManager *pResourceManager);
		virtual ~CGLES3RenderTexture(void);


	public:
		HANDLE GetHandle(void) const;

	public:
		BOOL CreateColorTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		BOOL CreateDepthStencilTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		void Destroy(void);
		void DumpLog(void) const;

	public:
		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		uint32_t GetDepth(void) const;
		uint32_t GetSamples(void) const;

	public:
		size_t GetMemorySize(void) const;
	};

}
