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

	class CROSS_EXPORT CGLES3RenderPass : public CGfxRenderPass
	{
		friend class CGLES3RenderPassManager;


	protected:
		CGLES3RenderPass(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager);
		virtual ~CGLES3RenderPass(void);


	public:
		HANDLE GetHandle(void) const;

	public:
		BOOL Create(void);
		void Destroy(void);
		void DumpLog(void) const;

	public:
		BOOL SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples);
		BOOL SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		BOOL SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		BOOL SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassResolveAttachment(uint32_t indexSubpass, uint32_t indexAttachment, VkImageLayout imageLayout);
		BOOL SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass);

	public:
		uint32_t GetSubpassCount(void) const;
		const GLSubpassInformation* GetSubpass(uint32_t indexSubpass) const;
		const GLAttachmentDescription* GetAttachment(uint32_t indexAttachment) const;
		const GLClearValue* GetAttachmentClearValue(uint32_t indexAttachment) const;


	protected:
		std::map<uint32_t, GLClearValue> m_attachmentClearValues;
		std::map<uint32_t, GLAttachmentDescription> m_attachments;
		std::map<uint32_t, GLSubpassInformation> m_subpasses;

	protected:
		CGLES3Device *m_pDevice;
	};

}
