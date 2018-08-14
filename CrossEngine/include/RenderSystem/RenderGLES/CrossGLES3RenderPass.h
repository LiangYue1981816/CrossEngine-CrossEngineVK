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
		CGLES3RenderPass(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager, uint32_t numAttachments, uint32_t numSubpasses);
		virtual ~CGLES3RenderPass(void);


	public:
		CGLES3Device* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		BOOL Create(void);
		void Destroy(void);
		void DumpLog(void) const;

	public:
		BOOL SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples);
		BOOL SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		BOOL SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		BOOL SetSubpassInputColorReference(uint32_t indexSubPass, uint32_t indexAttachment);
		BOOL SetSubpassInputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment);
		BOOL SetSubpassOutputColorReference(uint32_t indexSubPass, uint32_t indexAttachment);
		BOOL SetSubpassOutputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment);
		BOOL SetSubpassResolveColorReference(uint32_t indexSubPass, uint32_t indexAttachment, VkImageLayout imageLayout);
		BOOL SetSubpassPreserveReference(uint32_t indexSubPass, uint32_t indexAttachment);
		BOOL SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass, VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VkAccessFlags srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VkAccessFlags dstAccessMask = VK_ACCESS_SHADER_READ_BIT, VkDependencyFlags dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT);

	public:
		uint32_t GetSubpassCount(void) const;
		uint32_t GetSubpassOutputAttachmentCount(uint32_t indexSubPass) const;
		const GLSubpassInformation* GetSubpass(uint32_t indexSubPass) const;

		uint32_t GetAttachmentCount(void) const;
		const VkClearValue* GetAttachmentClearValue(uint32_t indexAttachment) const;
		const VkAttachmentDescription* GetAttachmentDescription(uint32_t indexAttachment) const;


	protected:
		eastl::vector<VkClearValue> m_clears;
		eastl::vector<VkAttachmentDescription> m_attachments;
		eastl::vector<GLSubpassInformation> m_subpasses;

	protected:
		CGLES3Device *m_pDevice;
	};

}
