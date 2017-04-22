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

	class CROSS_EXPORT CVulkanRenderPass : public CVulkanResource
	{
		friend class CVulkanRenderPassManager;


	protected:
		typedef struct {
			std::map<uint32_t, VkImageLayout> inputAttachments;
			std::map<uint32_t, VkImageLayout> colorAttachments;
			std::map<uint32_t, VkImageLayout> resolveAttachments;
			std::map<uint32_t, uint32_t> preserveAttachments;
			VkAttachmentReference depthStencilAttachment;
		} VkSubpassInformation;


	protected:
		CVulkanRenderPass(CVulkanDevice *pDevice, CVulkanResourceManager *pManager);
		virtual ~CVulkanRenderPass(void);


	public:
		virtual BOOL Create(void);
		virtual void Destroy(void);

	protected:
		BOOL CreateAttachments(std::vector<VkAttachmentDescription> &attachments);
		BOOL CreateSubpasses(std::vector<VkSubpassDescription> &subpasses, std::map<uint32_t, std::vector<VkAttachmentReference>> &inputAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &colorAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &resolveAttachments, std::map<uint32_t, std::vector<uint32_t>> &preserveAttachments, std::map<uint32_t, VkAttachmentReference> &depthStencilAttachment);
		BOOL CreateDependencies(std::vector<VkSubpassDependency> &dependencies);

	public:
		BOOL SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue);
		BOOL SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue);
		BOOL SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue);

		BOOL SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass);

	public:
		uint32_t GetSubpassCount(void) const;
		VkRenderPass GetRenderPass(void) const;
		std::vector<VkClearValue> GetClearValues(void) const;

	public:
		virtual void DumpLog(void) const;


	protected:
		VkRenderPass m_vkRenderPass;

	protected:
		std::map<uint32_t, VkClearValue> m_attachmentClearValues;
		std::map<uint32_t, VkAttachmentDescription> m_attachments;
		std::map<uint32_t, VkSubpassInformation> m_subpasses;
		std::map<uint32_t, VkSubpassDependency> m_dependencies;
	};

}
