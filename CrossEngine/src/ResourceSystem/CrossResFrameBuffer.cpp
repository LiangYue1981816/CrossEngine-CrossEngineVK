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

	CResFrameBuffer::CResFrameBuffer(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResFrameBuffer::~CResFrameBuffer(void)
	{

	}

	RESOURCE_TYPE CResFrameBuffer::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_FRAME_BUFFER;
	}

	const CGfxFrameBufferPtr& CResFrameBuffer::GetFrameBuffer(void) const
	{
		return m_ptrFrameBuffer;
	}

	BOOL CResFrameBuffer::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrFrameBuffer.IsNull() || m_ptrFrameBuffer->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResFrameBuffer::InternalLoad(BOOL bSyncPostLoad)
	{
		return FALSE;
	}

	BOOL CResFrameBuffer::InternalPostLoad(void)
	{
		return FALSE;
	}

	void CResFrameBuffer::InternalLoadFail(void)
	{
		m_ptrFrameBuffer.Release();
		CResource::InternalLoadFail();
	}

	void CResFrameBuffer::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResFrameBuffer::LoadAttachmentPresents(TiXmlNode *pAttachmentsNode)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentsNode->FirstChild("Attachment")) {
			do {
				AttachmentPresentParam param;

				param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
				param.indexSurface = pAttachmentNode->ToElement()->AttributeInt1("index_surface");

				m_param.presents.push_back(param);
			} while (pAttachmentNode = pAttachmentNode->IterateChildren("Attachment", pAttachmentNode));
		}

		return TRUE;
	}

	BOOL CResFrameBuffer::LoadAttachmentColors(TiXmlNode *pAttachmentsNode)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentsNode->FirstChild("Attachment")) {
			do {
				AttachmentColorParam param;

				param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
				param.dwName = HashValue(pAttachmentNode->ToElement()->AttributeString("color_render_texture"));

				m_param.colors.push_back(param);
			} while (pAttachmentNode = pAttachmentNode->IterateChildren("Attachment", pAttachmentNode));
		}

		return TRUE;
	}

	BOOL CResFrameBuffer::LoadAttachmentDepthStencils(TiXmlNode *pAttachmentsNode)
	{
		if (TiXmlNode *pAttachmentNode = pAttachmentsNode->FirstChild("Attachment")) {
			do {
				AttachmentDepthStencilParam param;

				param.indexAttachment = pAttachmentNode->ToElement()->AttributeInt1("index");
				param.dwName = HashValue(pAttachmentNode->ToElement()->AttributeString("depthstencil_render_texture"));

				m_param.depthStencils.push_back(param);
			} while (pAttachmentNode = pAttachmentNode->IterateChildren("Attachment", pAttachmentNode));
		}

		return TRUE;
	}

}
