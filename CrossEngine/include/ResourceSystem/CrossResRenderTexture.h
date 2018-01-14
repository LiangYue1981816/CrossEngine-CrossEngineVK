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

	class CROSS_EXPORT CResRenderTexture : public CResource
	{
		friend class CResRenderTextureManager;


	public:
		typedef enum {
			RENDER_TEXTURE_TYPE_COLOR = 0,
			RENDER_TEXTURE_TYPE_DEPTH
		} RENDER_TEXTURE_TYPE;


	protected:
		CResRenderTexture(CResourceManager *pResourceManager, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		virtual ~CResRenderTexture(void);


	public:
		const CGfxRenderTexturePtr& GetRenderTexture(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual void Init(void);
		virtual void Free(void);

	protected:
		virtual BOOL InternalLoad(BOOL bSyncPostLoad);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalCleanup(void);


	protected:
		RENDER_TEXTURE_TYPE m_type;

		uint32_t m_width;
		uint32_t m_height;
		VkFormat m_format;
		VkSampleCountFlagBits m_samples;

	protected:
		VkFilter m_minFilter;
		VkFilter m_magFilter;
		VkSamplerMipmapMode m_mipmapMode;
		VkSamplerAddressMode m_addressMode;

	protected:
		CGfxRenderTexturePtr m_ptrRenderTexture;
	};

}
