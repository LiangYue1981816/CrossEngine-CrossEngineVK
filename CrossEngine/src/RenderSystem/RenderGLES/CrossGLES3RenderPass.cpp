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

	CGLES3RenderPass::CGLES3RenderPass(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager, uint32_t numAttachments, uint32_t numSubpasses)
		: CGfxRenderPass(pResourceManager)
		, m_pDevice(pDevice)
	{
		m_clears.resize(numAttachments);
		m_attachments.resize(numAttachments);
		m_subpasses.resize(numSubpasses);
	}

	CGLES3RenderPass::~CGLES3RenderPass(void)
	{

	}

	CGLES3Device* CGLES3RenderPass::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3RenderPass::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	BOOL CGLES3RenderPass::Create(void)
	{
		return TRUE;
	}

	void CGLES3RenderPass::Destroy(void)
	{

	}

	BOOL CGLES3RenderPass::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = stencilLoadOp;
		m_attachments[indexAttachment].stencilStoreOp = stencilStoreOp;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].inputAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].inputAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].colorAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].depthStencilAttachment = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassResolveColorReference(uint32_t indexSubpass, uint32_t indexAttachment, VkImageLayout imageLayout)
	{
		if (indexSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].resolveAttachments[indexAttachment] = imageLayout;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags)
	{
		return TRUE;
	}

	uint32_t CGLES3RenderPass::GetSubpassCount(void) const
	{
		return m_subpasses.size();
	}

	uint32_t CGLES3RenderPass::GetSubpassOutputAttachmentCount(uint32_t indexSubpass) const
	{
		return indexSubpass < m_subpasses.size() ? m_subpasses[indexSubpass].colorAttachments.size() : 0;
	}

	const GLSubpassInformation* CGLES3RenderPass::GetSubpass(uint32_t indexSubpass) const
	{
		return indexSubpass < m_subpasses.size() ? &m_subpasses[indexSubpass] : NULL;
	}

	uint32_t CGLES3RenderPass::GetAttachmentCount(void) const
	{
		return m_attachments.size();
	}

	const VkClearValue* CGLES3RenderPass::GetAttachmentClearValue(uint32_t indexAttachment) const
	{
		return indexAttachment < m_clears.size() ? &m_clears[indexAttachment] : NULL;
	}

	const VkAttachmentDescription* CGLES3RenderPass::GetAttachmentDescription(uint32_t indexAttachment) const
	{
		return indexAttachment < m_attachments.size() ? &m_attachments[indexAttachment] : NULL;
	}

	void CGLES3RenderPass::DumpLog(void) const
	{
		LOGI("\t\tRenderPass\n");

		LOGI("\t\t\tAttachments:\n");
		for (uint32_t indexAttachment = 0; indexAttachment < m_attachments.size(); indexAttachment++) {
			LOGI("\t\t\t\tAttachment %d: loadOp = %s storeOp = %s stencilLoadOp = %s stencilStoreOp = %s\n",
				indexAttachment,
				CGLES3Helper::glAttachmentLoadOpToString(m_attachments[indexAttachment].loadOp),
				CGLES3Helper::glAttachmentStoreOpToString(m_attachments[indexAttachment].storeOp),
				CGLES3Helper::glAttachmentLoadOpToString(m_attachments[indexAttachment].stencilLoadOp),
				CGLES3Helper::glAttachmentStoreOpToString(m_attachments[indexAttachment].stencilStoreOp));
		}

		LOGI("\t\t\tSubpasses:\n");
		for (uint32_t indexSubpass = 0; indexSubpass < m_subpasses.size(); indexSubpass++) {
			LOGI("\t\t\t\tSubpass %d:\n", indexSubpass);

			LOGI("\t\t\t\t\tInputAttachments:\n");
			for (const auto &itAttachment : m_subpasses[indexSubpass].inputAttachments) {
				LOGI("\t\t\t\t\t\tInputAttachment: attachment = %d\n", itAttachment.first);
			}

			LOGI("\t\t\t\t\tColorAttachments:\n");
			for (const auto &itAttachment : m_subpasses[indexSubpass].colorAttachments) {
				LOGI("\t\t\t\t\t\tColorAttachment: attachment = %d\n", itAttachment.first);
			}

			LOGI("\t\t\t\t\tDepthStencilAttachment: attachment = %d\n", m_subpasses[indexSubpass].depthStencilAttachment);
		}
	}

}
