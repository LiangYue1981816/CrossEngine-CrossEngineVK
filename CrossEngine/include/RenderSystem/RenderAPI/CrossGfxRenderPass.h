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

	class CROSS_EXPORT CGfxRenderPass : public CGfxResource
	{
	protected:
		CGfxRenderPass(CGfxResourceManager *pResourceManager)
			: CGfxResource(pResourceManager)
		{

		}
		virtual ~CGfxRenderPass(void)
		{

		}


	public:
		virtual BOOL Create(void) = 0;

	public:
		virtual BOOL SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples) = 0;
		virtual BOOL SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) = 0;
		virtual BOOL SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) = 0;

		virtual BOOL SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment) = 0;
		virtual BOOL SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment) = 0;
		virtual BOOL SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment) = 0;
		virtual BOOL SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment) = 0;
		virtual BOOL SetSubpassResolveColorReference(uint32_t indexSubpass, uint32_t indexAttachment, VkImageLayout imageLayout) = 0;
		virtual BOOL SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment) = 0;
		virtual BOOL SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass, VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VkAccessFlags srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VkAccessFlags dstAccessMask = VK_ACCESS_SHADER_READ_BIT, VkDependencyFlags dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT) = 0;

	public:
		virtual uint32_t GetSubpassCount(void) const = 0;
	};

}
