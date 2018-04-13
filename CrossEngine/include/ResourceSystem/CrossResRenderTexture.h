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


	protected:
		typedef enum {
			RENDER_TEXTURE_TYPE_COLOR = 0,
			RENDER_TEXTURE_TYPE_DEPTH_STENCIL
		} RENDER_TEXTURE_TYPE;

		typedef struct RenderTargetParam {
			RENDER_TEXTURE_TYPE type;
			uint32_t width;
			uint32_t height;
			VkFormat format;
			VkSampleCountFlagBits samples;
		} RenderTargetParam;

		typedef struct SamplerParam {
			VkFilter minFilter;
			VkFilter magFilter;
			VkSamplerMipmapMode mipmapMode;
			VkSamplerAddressMode addressMode;
		};

		typedef struct RenderTextureParam {
			RenderTargetParam renderTarget;
			SamplerParam sampler;
		} RenderTextureParam;


	protected:
		CResRenderTexture(CResourceManager *pResourceManager);
		virtual ~CResRenderTexture(void);


	public:
		const CGfxRenderTexturePtr& GetRenderTexture(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual BOOL InternalLoad(BOOL bSyncPostLoad);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalLoadFail(void);
		virtual void InternalLoadSuccess(void);

	protected:
		BOOL LoadRenderTexture(TiXmlNode *pRenderTextureNode);
		BOOL LoadRenderTarget(TiXmlNode *pRenderTextureNode);
		BOOL LoadSampler(TiXmlNode *pRenderTextureNode);


	protected:
		RenderTextureParam m_param;

	protected:
		CGfxRenderTexturePtr m_ptrRenderTexture;
	};

}
