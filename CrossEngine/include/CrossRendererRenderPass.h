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

	class CROSS_EXPORT CRendererRenderPass : public CRendererResource
	{
		friend class CRendererRenderPassManager;


	protected:
		typedef struct {
			std::map<uint32_t, VkImageLayout> inputAttachments;
			std::map<uint32_t, VkImageLayout> colorAttachments;
			std::map<uint32_t, VkImageLayout> resolveAttachments;
			std::map<uint32_t, uint32_t> preserveAttachments;
			VkAttachmentReference depthStencilAttachment;
		} VkSubpassInformation;


	protected:
		CRendererRenderPass(CRendererDevice *pDevice, CRendererResourceManager *pManager);
		virtual ~CRendererRenderPass(void);


	public:
		BOOL SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp);
		BOOL SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp);
		BOOL SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp);

		BOOL SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment);
		BOOL SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass);

		void SetClearColorValue(float red, float green, float blue, float alpha);
		void SetClearDepthStencilValue(float depth, uint32_t stencil);

	public:
		virtual BOOL Create(void);
		virtual void Destroy(void);
		virtual void DumpLog(void) const;

	protected:
		BOOL CreateAttachments(std::vector<VkAttachmentDescription> &attachments);
		BOOL CreateSubpasses(std::vector<VkSubpassDescription> &subpasses, std::map<uint32_t, std::vector<VkAttachmentReference>> &inputAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &colorAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &resolveAttachments, std::map<uint32_t, std::vector<uint32_t>> &preserveAttachments, std::map<uint32_t, VkAttachmentReference> &depthStencilAttachment);
		BOOL CreateDependencies(std::vector<VkSubpassDependency> &dependencies);


	public:
		VkRenderPass GetRenderPass(void) const;

	public:
		void BeginRenderPass(VkCommandBuffer vkCommandBuffer, VkFramebuffer vkFramebuffer, VkRect2D renderArea, VkSubpassContents contents) const;
		void EndRenderPass(VkCommandBuffer vkCommandBuffer) const;
		void NextSubpass(VkCommandBuffer vkCommandBuffer, VkSubpassContents contents) const;
		uint32_t GetSubpassCount(void) const;


	protected:
		VkClearValue m_clearValues[2];
		std::map<uint32_t, VkAttachmentDescription> m_attachments;
		std::map<uint32_t, VkSubpassInformation> m_subpasses;
		std::map<uint32_t, VkSubpassDependency> m_dependencies;

	protected:
		VkRenderPass m_vkRenderPass;
	};

}
