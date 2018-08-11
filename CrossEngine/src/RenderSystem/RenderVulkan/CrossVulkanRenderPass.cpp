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

	CVulkanRenderPass::CVulkanRenderPass(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager, uint32_t numAttachments, uint32_t numSubpasses)
		: CGfxRenderPass(pResourceManager)
		, m_pDevice(pDevice)
		, m_vkRenderPass(VK_NULL_HANDLE)
	{
		m_clears.resize(numAttachments);
		m_attachments.resize(numAttachments);
		m_subpasses.resize(numSubpasses);
		m_dependencies.resize(numSubpasses - 1);
	}

	CVulkanRenderPass::~CVulkanRenderPass(void)
	{

	}

	CVulkanDevice* CVulkanRenderPass::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanRenderPass::GetHandle(void) const
	{
		return m_vkRenderPass;
	}

	BOOL CVulkanRenderPass::Create(void)
	{
		try {
			std::vector<VkSubpassDescription> subpasses;
			std::map<uint32_t, std::vector<VkAttachmentReference>> inputAttachments;
			std::map<uint32_t, std::vector<VkAttachmentReference>> colorAttachments;
			std::map<uint32_t, std::vector<VkAttachmentReference>> resolveAttachments;
			std::map<uint32_t, std::vector<uint32_t>> preserveAttachments;
			CALL_BOOL_FUNCTION_THROW(CreateSubpasses(subpasses, inputAttachments, colorAttachments, resolveAttachments, preserveAttachments));

			VkRenderPassCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.subpassCount = subpasses.size();
			createInfo.pSubpasses = subpasses.data();
			createInfo.attachmentCount = m_attachments.size();
			createInfo.pAttachments = m_attachments.data();
			createInfo.dependencyCount = m_dependencies.size();
			createInfo.pDependencies = m_dependencies.data();
			CALL_VK_FUNCTION_THROW(vkCreateRenderPass(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderPass));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanRenderPass::CreateSubpasses(std::vector<VkSubpassDescription> &subpasses, std::map<uint32_t, std::vector<VkAttachmentReference>> &inputAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &colorAttachments, std::map<uint32_t, std::vector<VkAttachmentReference>> &resolveAttachments, std::map<uint32_t, std::vector<uint32_t>> &preserveAttachments)
	{
		subpasses.clear();
		inputAttachments.clear();
		colorAttachments.clear();
		resolveAttachments.clear();
		preserveAttachments.clear();

		for (uint32_t indexSubPass = 0; indexSubPass < m_subpasses.size(); indexSubPass++) {
			for (const auto &itAttachment : m_subpasses[indexSubPass].inputAttachments) {
				inputAttachments[indexSubPass].emplace_back(VkAttachmentReference{ itAttachment.first, itAttachment.second });
			}

			for (const auto &itAttachment : m_subpasses[indexSubPass].colorAttachments) {
				colorAttachments[indexSubPass].emplace_back(VkAttachmentReference{ itAttachment.first, itAttachment.second });
			}

			for (const auto &itAttachment : m_subpasses[indexSubPass].resolveAttachments) {
				resolveAttachments[indexSubPass].emplace_back(VkAttachmentReference{ itAttachment.first, itAttachment.second });
			}

			for (const auto &itAttachment : m_subpasses[indexSubPass].preserveAttachments) {
				preserveAttachments[indexSubPass].emplace_back(itAttachment.second);
			}

			VkSubpassDescription subpass = {};
			subpass.flags = 0;
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.inputAttachmentCount = inputAttachments[indexSubPass].size();
			subpass.pInputAttachments = inputAttachments[indexSubPass].data();
			subpass.colorAttachmentCount = colorAttachments[indexSubPass].size();
			subpass.pColorAttachments = colorAttachments[indexSubPass].data();
			subpass.pResolveAttachments = resolveAttachments[indexSubPass].data();
			subpass.preserveAttachmentCount = preserveAttachments[indexSubPass].size();
			subpass.pPreserveAttachments = preserveAttachments[indexSubPass].data();
			subpass.pDepthStencilAttachment = m_subpasses[indexSubPass].depthStencilAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED ? NULL : &m_subpasses[indexSubPass].depthStencilAttachment;
			subpasses.emplace_back(subpass);
		}

		return TRUE;
	}

	void CVulkanRenderPass::Destroy(void)
	{
		if (m_vkRenderPass) {
			vkDestroyRenderPass(m_pDevice->GetDevice(), m_vkRenderPass, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkRenderPass = VK_NULL_HANDLE;
	}

	BOOL CVulkanRenderPass::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment] = {};
		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment] = {};
		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = finalLayout;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkClearValue clearValue, VkSampleCountFlagBits samples, VkImageLayout finalLayout)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_attachments[indexAttachment] = {};
		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = samples;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = stencilLoadOp;
		m_attachments[indexAttachment].stencilStoreOp = stencilStoreOp;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = finalLayout;
		m_clears[indexAttachment] = clearValue;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassInputColorReference(uint32_t indexSubPass, uint32_t indexAttachment)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].inputAttachments[indexAttachment] = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassInputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].inputAttachments[indexAttachment] = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassOutputColorReference(uint32_t indexSubPass, uint32_t indexAttachment)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].colorAttachments[indexAttachment] = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassOutputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].depthStencilAttachment.attachment = indexAttachment;
		m_subpasses[indexSubPass].depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassResolveColorReference(uint32_t indexSubPass, uint32_t indexAttachment, VkImageLayout imageLayout)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].resolveAttachments[indexAttachment] = imageLayout;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassPreserveReference(uint32_t indexSubPass, uint32_t indexAttachment)
	{
		if (indexSubPass >= m_subpasses.size()) {
			return FALSE;
		}

		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		m_subpasses[indexSubPass].preserveAttachments[indexAttachment] = indexAttachment;

		return TRUE;
	}

	BOOL CVulkanRenderPass::SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags)
	{
		if (indexDependency >= m_dependencies.size()) {
			return FALSE;
		}

		if (indexSrcSubpass >= m_subpasses.size() && indexSrcSubpass != VK_SUBPASS_EXTERNAL) {
			return FALSE;
		}

		if (indexDstSubpass >= m_subpasses.size() && indexDstSubpass != VK_SUBPASS_EXTERNAL) {
			return FALSE;
		}

		m_dependencies[indexDependency] = {};
		m_dependencies[indexDependency].srcSubpass = indexSrcSubpass;
		m_dependencies[indexDependency].dstSubpass = indexDstSubpass;
		m_dependencies[indexDependency].srcStageMask = srcStageMask;
		m_dependencies[indexDependency].dstStageMask = dstStageMask;
		m_dependencies[indexDependency].srcAccessMask = srcAccessMask;
		m_dependencies[indexDependency].dstAccessMask = dstAccessMask;
		m_dependencies[indexDependency].dependencyFlags = dependencyFlags;

		return TRUE;
	}

	uint32_t CVulkanRenderPass::GetSubpassCount(void) const
	{
		return m_subpasses.size();
	}

	uint32_t CVulkanRenderPass::GetSubpassOutputAttachmentCount(uint32_t indexSubPass) const
	{
		return indexSubPass < m_subpasses.size() ? m_subpasses[indexSubPass].colorAttachments.size() : 0;
	}

	const VkSubpassInformation* CVulkanRenderPass::GetSubpass(uint32_t indexSubPass) const
	{
		return indexSubPass < m_subpasses.size() ? &m_subpasses[indexSubPass] : NULL;
	}

	uint32_t CVulkanRenderPass::GetAttachmentCount(void) const
	{
		return m_attachments.size();
	}

	const VkClearValue* CVulkanRenderPass::GetAttachmentClearValue(uint32_t indexAttachment) const
	{
		return indexAttachment < m_clears.size() ? &m_clears[indexAttachment] : NULL;
	}

	const VkAttachmentDescription* CVulkanRenderPass::GetAttachmentDescription(uint32_t indexAttachment) const
	{
		return indexAttachment < m_attachments.size() ? &m_attachments[indexAttachment] : NULL;
	}

	void CVulkanRenderPass::DumpLog(void) const
	{
		if (m_vkRenderPass) {
			LOGI("\t\tRenderPass 0x%x:\n", m_vkRenderPass);

			LOGI("\t\t\tAttachments:\n");
			for (uint32_t indexAttachment = 0; indexAttachment < m_attachments.size(); indexAttachment++) {
				LOGI("\t\t\t\tAttachment %d: format = %s samples = %s loadOp = %s storeOp = %s stencilLoadOp = %s stencilStoreOp = %s initialLayout = %s finalLayout = %s\n",
					indexAttachment,
					CVulkanHelper::FormatToString(m_attachments[indexAttachment].format),
					CVulkanHelper::SampleCountFlagBitsToString(m_attachments[indexAttachment].samples),
					CVulkanHelper::AttachmentLoadOpToString(m_attachments[indexAttachment].loadOp),
					CVulkanHelper::AttachmentStoreOpToString(m_attachments[indexAttachment].storeOp),
					CVulkanHelper::AttachmentLoadOpToString(m_attachments[indexAttachment].stencilLoadOp),
					CVulkanHelper::AttachmentStoreOpToString(m_attachments[indexAttachment].stencilStoreOp),
					CVulkanHelper::ImageLayoutToString(m_attachments[indexAttachment].initialLayout),
					CVulkanHelper::ImageLayoutToString(m_attachments[indexAttachment].finalLayout));
			}

			LOGI("\t\t\tSubpasses:\n");
			for (uint32_t indexSubPass = 0; indexSubPass < m_subpasses.size(); indexSubPass++) {
				LOGI("\t\t\t\tSubpass %d:\n", indexSubPass);

				LOGI("\t\t\t\t\tInputAttachments:\n");
				for (const auto &itAttachment : m_subpasses[indexSubPass].inputAttachments) {
					LOGI("\t\t\t\t\t\tInputAttachment: attachment = %d layout = %s\n", itAttachment.first, CVulkanHelper::ImageLayoutToString(itAttachment.second));
				}

				LOGI("\t\t\t\t\tColorAttachments:\n");
				for (const auto &itAttachment : m_subpasses[indexSubPass].colorAttachments) {
					LOGI("\t\t\t\t\t\tColorAttachment: attachment = %d layout = %s\n", itAttachment.first, CVulkanHelper::ImageLayoutToString(itAttachment.second));
				}

				LOGI("\t\t\t\t\tResolveAttachments:\n");
				for (const auto &itAttachment : m_subpasses[indexSubPass].resolveAttachments) {
					LOGI("\t\t\t\t\t\tResolveAttachment: attachment = %d layout = %s\n", itAttachment.first, CVulkanHelper::ImageLayoutToString(itAttachment.second));
				}

				LOGI("\t\t\t\t\tPreserveAttachments:\n");
				for (const auto &itAttachment : m_subpasses[indexSubPass].preserveAttachments) {
					LOGI("\t\t\t\t\t\tPreserveAttachment: attachment = %d\n", itAttachment.first);
				}

				if (m_subpasses[indexSubPass].depthStencilAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
					LOGI("\t\t\t\t\tDepthStencilAttachment: NULL\n");
				}
				else {
					LOGI("\t\t\t\t\tDepthStencilAttachment: attachment = %d layout = %s\n", m_subpasses[indexSubPass].depthStencilAttachment.attachment, CVulkanHelper::ImageLayoutToString(m_subpasses[indexSubPass].depthStencilAttachment.layout));
				}
			}

			LOGI("\t\t\tDependencies:\n");
			for (uint32_t indexDependency = 0; indexDependency < m_dependencies.size(); indexDependency++) {
				char szSrcStageMask[_MAX_STRING];
				char szDstStageMask[_MAX_STRING];
				char szSrcAccessMask[_MAX_STRING];
				char szDstAccessMask[_MAX_STRING];
				LOGI("\t\t\t\tDependency %d: srcSubpass = %d dstSubpass = %d srcStageMask = %s dstStageMask = %s srcAccessMask = %s dstAccessMask = %s\n",
					indexDependency,
					m_dependencies[indexDependency].srcSubpass,
					m_dependencies[indexDependency].dstSubpass,
					CVulkanHelper::PipelineStageFlagsToString(m_dependencies[indexDependency].srcStageMask, szSrcStageMask),
					CVulkanHelper::PipelineStageFlagsToString(m_dependencies[indexDependency].dstStageMask, szDstStageMask),
					CVulkanHelper::AccessFlagsToString(m_dependencies[indexDependency].srcAccessMask, szSrcAccessMask),
					CVulkanHelper::AccessFlagsToString(m_dependencies[indexDependency].dstAccessMask, szDstAccessMask));
			}
		}
	}

}
