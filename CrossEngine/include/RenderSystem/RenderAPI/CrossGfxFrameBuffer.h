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

	class CROSS_EXPORT CGfxFrameBuffer : public CGfxResource
	{
	protected:
		CGfxFrameBuffer(CGfxResourceManager *pResourceManager)
			: CGfxResource(pResourceManager)
		{

		}
		virtual ~CGfxFrameBuffer(void)
		{

		}


	public:
		virtual BOOL Create(HANDLE hRenderPass) = 0;

	public:
		virtual BOOL SetPresentAttachment(uint32_t indexAttachment, uint32_t width, uint32_t height, HANDLE hImageView) = 0;
		virtual BOOL SetColorAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture) = 0;
		virtual BOOL SetDepthStencilAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture) = 0;

	public:
		virtual uint32_t GetWidth(void) const = 0;
		virtual uint32_t GetHeight(void) const = 0;
	};

}
