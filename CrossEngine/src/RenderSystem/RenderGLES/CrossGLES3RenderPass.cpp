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

	CGLES3RenderPass::CGLES3RenderPass(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxRenderPass(pResourceManager)
		, m_pDevice(pDevice)
	{

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
		m_attachments.clear();
		m_attachmentClearValues.clear();
		m_subpasses.clear();
	}

	BOOL CGLES3RenderPass::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples)
	{
		if (indexAttachment >= m_pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachmentClearValues[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachmentClearValues[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_pDevice->GetPhysicalDeviceLimits().MAX_COLOR_ATTACHMENTS) {
			return FALSE;
		}

		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = stencilLoadOp;
		m_attachments[indexAttachment].stencilStoreOp = stencilStoreOp;
		m_attachmentClearValues[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassInputReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (m_attachments.find(indexAttachment) == m_attachments.end()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].inputAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (m_attachments.find(indexAttachment) == m_attachments.end()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].colorAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (m_attachments.find(indexAttachment) == m_attachments.end()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].depthStencilAttachment = indexAttachment;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassResolveColorReference(uint32_t indexSubpass, uint32_t indexAttachment, VkImageLayout imageLayout)
	{
		if (m_attachments.find(indexAttachment) == m_attachments.end()) {
			return FALSE;
		}

		m_subpasses[indexSubpass].resolveAttachments[indexAttachment] = imageLayout;

		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		return TRUE;
	}

	BOOL CGLES3RenderPass::SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass)
	{
		return TRUE;
	}

	uint32_t CGLES3RenderPass::GetSubpassCount(void) const
	{
		return m_subpasses.size();
	}

	const GLSubpassInformation* CGLES3RenderPass::GetSubpass(uint32_t indexSubpass) const
	{
		const auto &itSubpass = m_subpasses.find(indexSubpass);
		return itSubpass != m_subpasses.end() ? &itSubpass->second : NULL;
	}

	const VkClearValue* CGLES3RenderPass::GetAttachmentClearValue(uint32_t indexAttachment) const
	{
		const auto &itAttachmentClearValue = m_attachmentClearValues.find(indexAttachment);
		return itAttachmentClearValue != m_attachmentClearValues.end() ? &itAttachmentClearValue->second : NULL;
	}

	const VkAttachmentDescription* CGLES3RenderPass::GetAttachmentDescription(uint32_t indexAttachment) const
	{
		const auto &itAttachment = m_attachments.find(indexAttachment);
		return itAttachment != m_attachments.end() ? &itAttachment->second : NULL;
	}

	const std::map<uint32_t, VkAttachmentDescription>& CGLES3RenderPass::GetAttachmentDescriptions(void) const
	{
		return m_attachments;
	}

	void CGLES3RenderPass::DumpLog(void) const
	{
		LOGI("\t\tRenderPass\n");

		LOGI("\t\t\tAttachments:\n");
		for (const auto &itAttachment : m_attachments) {
			LOGI("\t\t\t\tAttachment %d: loadOp = %s storeOp = %s stencilLoadOp = %s stencilStoreOp = %s\n",
				itAttachment.first,
				CGLES3Helper::glAttachmentLoadOpToString(itAttachment.second.loadOp),
				CGLES3Helper::glAttachmentStoreOpToString(itAttachment.second.storeOp),
				CGLES3Helper::glAttachmentLoadOpToString(itAttachment.second.stencilLoadOp),
				CGLES3Helper::glAttachmentStoreOpToString(itAttachment.second.stencilStoreOp));
		}

		LOGI("\t\t\tSubpasses:\n");
		for (const auto &itSubpass : m_subpasses) {
			LOGI("\t\t\t\tSubpass %d:\n", itSubpass.first);

			LOGI("\t\t\t\t\tInputAttachments:\n");
			for (const auto &itAttachment : itSubpass.second.inputAttachments) {
				LOGI("\t\t\t\t\t\tInputAttachment: attachment = %d\n", itAttachment.first);
			}

			LOGI("\t\t\t\t\tColorAttachments:\n");
			for (const auto &itAttachment : itSubpass.second.colorAttachments) {
				LOGI("\t\t\t\t\t\tColorAttachment: attachment = %d\n", itAttachment.first);
			}

			LOGI("\t\t\t\t\tDepthStencilAttachment: attachment = %d\n", itSubpass.second.depthStencilAttachment);
		}
	}

}
