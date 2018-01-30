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

	class CROSS_EXPORT CResFrameBuffer : public CResource
	{
		friend class CResFrameBufferManager;


	protected:
		typedef struct AttachmentPresentParam {
			uint32_t indexAttachment;
			uint32_t indexSurface;
		} AttachmentPresentParam;

		typedef struct AttachmentColorParam {
			uint32_t indexAttachment;
			uint32_t dwName;
		} AttachmentColorParam;

		typedef struct AttachmentDepthStencilParam {
			uint32_t indexAttachment;
			uint32_t dwName;
		} AttachmentDepthStencilParam;

		typedef struct FrameBufferParam {
			std::vector<AttachmentPresentParam> presents;
			std::vector<AttachmentColorParam> colors;
			std::vector<AttachmentDepthStencilParam> depthStencils;
		} FrameBufferParam;


	protected:
		CResFrameBuffer(CResourceManager *pResourceManager);
		virtual ~CResFrameBuffer(void);


	public:
		const CGfxFrameBufferPtr& GetFrameBuffer(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual BOOL InternalLoad(BOOL bSyncPostLoad);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalLoadFail(void);
		virtual void InternalLoadSuccess(void);

	protected:
		BOOL LoadAttachmentPresents(TiXmlNode *pAttachmentsNode);
		BOOL LoadAttachmentColors(TiXmlNode *pAttachmentsNode);
		BOOL LoadAttachmentDepthStencils(TiXmlNode *pAttachmentsNode);


	protected:
		FrameBufferParam m_param;

	protected:
		CGfxFrameBufferPtr m_ptrFrameBuffer;
	};

}
