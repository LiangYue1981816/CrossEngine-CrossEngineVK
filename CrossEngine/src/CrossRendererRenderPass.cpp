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

	CRendererRenderPass::CRendererRenderPass(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkRenderPass(VK_NULL_HANDLE)
		, m_clearValues{ {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0} }
	{

	}

	CRendererRenderPass::~CRendererRenderPass(void)
	{
		ASSERT(m_vkRenderPass == VK_NULL_HANDLE);
	}

	BOOL CRendererRenderPass::SetColorAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = VK_SAMPLE_COUNT_1_BIT;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return TRUE;
	}

	BOOL CRendererRenderPass::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = VK_SAMPLE_COUNT_1_BIT;
		m_attachments[indexAttachment].loadOp = loadOp;
		m_attachments[indexAttachment].storeOp = storeOp;
		m_attachments[indexAttachment].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		return TRUE;
	}

	BOOL CRendererRenderPass::SetDepthStencilAttachment(uint32_t indexAttachment, VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_attachments[indexAttachment].flags = 0;
		m_attachments[indexAttachment].format = format;
		m_attachments[indexAttachment].samples = VK_SAMPLE_COUNT_1_BIT;
		m_attachments[indexAttachment].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments[indexAttachment].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments[indexAttachment].stencilLoadOp = loadOp;
		m_attachments[indexAttachment].stencilStoreOp = storeOp;
		m_attachments[indexAttachment].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments[indexAttachment].finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassInputColorReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_subpasses[indexSubpass].inputAttachments[indexAttachment] = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassInputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_subpasses[indexSubpass].inputAttachments[indexAttachment] = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassOutputColorReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_subpasses[indexSubpass].colorAttachments[indexAttachment] = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassOutputDepthStencilReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_subpasses[indexSubpass].depthStencilAttachment.attachment = indexAttachment;
		m_subpasses[indexSubpass].depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassPreserveReference(uint32_t indexSubpass, uint32_t indexAttachment)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		m_subpasses[indexSubpass].preserveAttachments[indexAttachment] = indexAttachment;
		return TRUE;
	}

	BOOL CRendererRenderPass::SetSubpassDependency(uint32_t indexDependency, uint32_t indexSrcSubpass, uint32_t indexDstSubpass)
	{
		if (indexSrcSubpass >= m_subpasses.size() || indexDstSubpass >= m_subpasses.size()) {
			return FALSE;
		}

		m_dependencies[indexDependency].srcSubpass = indexSrcSubpass;
		m_dependencies[indexDependency].dstSubpass = indexDstSubpass;
		m_dependencies[indexDependency].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		m_dependencies[indexDependency].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		m_dependencies[indexDependency].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		m_dependencies[indexDependency].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		m_dependencies[indexDependency].dependencyFlags = 0;

		return TRUE;
	}

	void CRendererRenderPass::SetClearColorValue(float red, float green, float blue, float alpha)
	{
		m_clearValues[0].color.float32[0] = red;
		m_clearValues[0].color.float32[1] = green;
		m_clearValues[0].color.float32[2] = blue;
		m_clearValues[0].color.float32[3] = alpha;
	}

	void CRendererRenderPass::SetClearDepthStencilValue(float depth, uint32_t stencil)
	{
		m_clearValues[1].depthStencil.depth = depth;
		m_clearValues[1].depthStencil.stencil = stencil;
	}

	BOOL CRendererRenderPass::Create(void)
	{
		try {
			std::vector<VkAttachmentDescription> attachments;
			std::vector<VkSubpassDescription> subpasses;
			std::vector<VkSubpassDependency> dependencies;

			std::map<uint32_t, std::vector<VkAttachmentReference>> inputAttachments;
			std::map<uint32_t, std::vector<VkAttachmentReference>> colorAttachments;
			std::map<uint32_t, std::vector<VkAttachmentReference>> resolveAttachments;
			std::map<uint32_t, std::vector<uint32_t>> preserveAttachments;
			std::map<uint32_t, VkAttachmentReference> depthStencilAttachment;

			for (std::map<uint32_t, VkAttachmentDescription>::const_iterator itAttachment = m_attachments.begin(); itAttachment != m_attachments.end(); ++itAttachment) {
				attachments.push_back(itAttachment->second);
			}

			for (std::map<uint32_t, VkSubpassInformation>::const_iterator itSubpass = m_subpasses.begin(); itSubpass != m_subpasses.end(); ++itSubpass) {
				depthStencilAttachment[itSubpass->first] = itSubpass->second.depthStencilAttachment;

				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.inputAttachments.begin(); itAttachment != itSubpass->second.inputAttachments.end(); ++itAttachment) {
					inputAttachments[itSubpass->first].push_back(VkAttachmentReference{ itAttachment->first, itAttachment->second });
				}

				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.colorAttachments.begin(); itAttachment != itSubpass->second.colorAttachments.end(); ++itAttachment) {
					colorAttachments[itSubpass->first].push_back(VkAttachmentReference{ itAttachment->first, itAttachment->second });
				}

				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.resolveAttachments.begin(); itAttachment != itSubpass->second.resolveAttachments.end(); ++itAttachment) {
					resolveAttachments[itSubpass->first].push_back(VkAttachmentReference{ itAttachment->first, itAttachment->second });
				}

				for (std::map<uint32_t, uint32_t>::const_iterator itAttachment = itSubpass->second.preserveAttachments.begin(); itAttachment != itSubpass->second.preserveAttachments.end(); ++itAttachment) {
					preserveAttachments[itSubpass->first].push_back(itAttachment->second);
				}

				VkSubpassDescription subpass;
				subpass.flags = 0;
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.inputAttachmentCount = inputAttachments[itSubpass->first].size();
				subpass.colorAttachmentCount = colorAttachments[itSubpass->first].size();
				subpass.preserveAttachmentCount = preserveAttachments[itSubpass->first].size();
				subpass.pInputAttachments = inputAttachments[itSubpass->first].data();
				subpass.pColorAttachments = colorAttachments[itSubpass->first].data();
				subpass.pResolveAttachments = resolveAttachments[itSubpass->first].data();
				subpass.pPreserveAttachments = preserveAttachments[itSubpass->first].data();
				subpass.pDepthStencilAttachment = depthStencilAttachment[itSubpass->first].layout == VK_IMAGE_LAYOUT_UNDEFINED ? NULL : &depthStencilAttachment[itSubpass->first];
				subpasses.push_back(subpass);
			}

			for (std::map<uint32_t, VkSubpassDependency>::const_iterator itDependency = m_dependencies.begin(); itDependency != m_dependencies.end(); ++itDependency) {
				dependencies.push_back(itDependency->second);
			}

			VkRenderPassCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.attachmentCount = attachments.size();
			createInfo.subpassCount = subpasses.size();
			createInfo.dependencyCount = dependencies.size();
			createInfo.pAttachments = attachments.data();
			createInfo.pSubpasses = subpasses.data();
			createInfo.pDependencies = dependencies.data();
			CALL_VK_FUNCTION_THROW(vkCreateRenderPass(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderPass));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererRenderPass::Destroy(void)
	{
		if (m_vkRenderPass) {
			vkDestroyRenderPass(m_pDevice->GetDevice(), m_vkRenderPass, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkRenderPass = VK_NULL_HANDLE;

		m_attachments.clear();
		m_subpasses.clear();
		m_dependencies.clear();
	}

	void CRendererRenderPass::DumpLog(void) const
	{
		if (m_vkRenderPass) {
			LOGI("\t\tRenderPass 0x%x:\n", m_vkRenderPass);

			LOGI("\t\t\tAttachments:\n");
			for (std::map<uint32_t, VkAttachmentDescription>::const_iterator itAttachment = m_attachments.begin(); itAttachment != m_attachments.end(); ++itAttachment) {
				LOGI("\t\t\t\tAttachment %d: format = %s samples = %s loadOp = %s storeOp = %s stencilLoadOp = %s stencilStoreOp = %s initialLayout = %s finalLayout = %s\n",
					itAttachment->first,
					CRendererHelper::vkFormatToString(itAttachment->second.format),
					CRendererHelper::vkSampleCountFlagBitsToString(itAttachment->second.samples),
					CRendererHelper::vkAttachmentLoadOpToString(itAttachment->second.loadOp),
					CRendererHelper::vkAttachmentStoreOpToString(itAttachment->second.storeOp),
					CRendererHelper::vkAttachmentLoadOpToString(itAttachment->second.stencilLoadOp),
					CRendererHelper::vkAttachmentStoreOpToString(itAttachment->second.stencilStoreOp),
					CRendererHelper::vkImageLayoutToString(itAttachment->second.initialLayout),
					CRendererHelper::vkImageLayoutToString(itAttachment->second.finalLayout));
			}

			LOGI("\t\t\tSubpasses:\n");
			for (std::map<uint32_t, VkSubpassInformation>::const_iterator itSubpass = m_subpasses.begin(); itSubpass != m_subpasses.end(); ++itSubpass) {
				LOGI("\t\t\t\tSubpass %d:\n", itSubpass->first);

				LOGI("\t\t\t\t\tInputAttachments:\n");
				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.inputAttachments.begin(); itAttachment != itSubpass->second.inputAttachments.end(); ++itAttachment) {
					LOGI("\t\t\t\t\t\tInputAttachment: attachment = %d layout = %s\n", itAttachment->first, CRendererHelper::vkImageLayoutToString(itAttachment->second));
				}

				LOGI("\t\t\t\t\tColorAttachments:\n");
				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.colorAttachments.begin(); itAttachment != itSubpass->second.colorAttachments.end(); ++itAttachment) {
					LOGI("\t\t\t\t\t\tColorAttachment: attachment = %d layout = %s\n", itAttachment->first, CRendererHelper::vkImageLayoutToString(itAttachment->second));
				}

				LOGI("\t\t\t\t\tResolveAttachments:\n");
				for (std::map<uint32_t, VkImageLayout>::const_iterator itAttachment = itSubpass->second.resolveAttachments.begin(); itAttachment != itSubpass->second.resolveAttachments.end(); ++itAttachment) {
					LOGI("\t\t\t\t\t\tResolveAttachment: attachment = %d layout = %s\n", itAttachment->first, CRendererHelper::vkImageLayoutToString(itAttachment->second));
				}

				LOGI("\t\t\t\t\tPreserveAttachments:\n");
				for (std::map<uint32_t, uint32_t>::const_iterator itAttachment = itSubpass->second.preserveAttachments.begin(); itAttachment != itSubpass->second.preserveAttachments.end(); ++itAttachment) {
					LOGI("\t\t\t\t\t\tPreserveAttachment: attachment = %d\n", itAttachment->first);
				}

				if (itSubpass->second.depthStencilAttachment.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
					LOGI("\t\t\t\t\tDepthStencilAttachment: NULL\n");
				}
				else {
					LOGI("\t\t\t\t\tDepthStencilAttachment: attachment = %d layout = %s\n", itSubpass->second.depthStencilAttachment.attachment, CRendererHelper::vkImageLayoutToString(itSubpass->second.depthStencilAttachment.layout));
				}
			}

			LOGI("\t\t\tDependencies:\n");
			for (std::map<uint32_t, VkSubpassDependency>::const_iterator itDependency = m_dependencies.begin(); itDependency != m_dependencies.end(); ++itDependency) {
				LOGI("\t\t\t\tDependency %d: srcSubpass = %d dstSubpass = %d srcStageMask = %s dstStageMask = %s srcAccessMask = %s dstAccessMask = %s\n",
					itDependency->first,
					itDependency->second.srcSubpass,
					itDependency->second.dstSubpass,
					CRendererHelper::vkPipelineStageFlagsToString(itDependency->second.srcStageMask),
					CRendererHelper::vkPipelineStageFlagsToString(itDependency->second.dstStageMask),
					CRendererHelper::vkAccessFlagsToString(itDependency->second.srcAccessMask),
					CRendererHelper::vkAccessFlagsToString(itDependency->second.dstAccessMask));
			}
		}
	}

	VkRenderPass CRendererRenderPass::GetRenderPass(void) const
	{
		return m_vkRenderPass;
	}

	void CRendererRenderPass::BeginRenderPass(VkCommandBuffer vkCommandBuffer, VkFramebuffer vkFramebuffer, VkRect2D renderArea, VkSubpassContents contents) const
	{
		VkRenderPassBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.pNext = NULL;
		beginInfo.renderPass = m_vkRenderPass;
		beginInfo.framebuffer = vkFramebuffer;
		beginInfo.renderArea = renderArea;
		beginInfo.clearValueCount = 2;
		beginInfo.pClearValues = m_clearValues;
		vkCmdBeginRenderPass(vkCommandBuffer, &beginInfo, contents);
	}

	void CRendererRenderPass::EndRenderPass(VkCommandBuffer vkCommandBuffer) const
	{
		vkCmdEndRenderPass(vkCommandBuffer);
	}

	void CRendererRenderPass::NextSubpass(VkCommandBuffer vkCommandBuffer, VkSubpassContents contents) const
	{
		vkCmdNextSubpass(vkCommandBuffer, contents);
	}

	uint32_t CRendererRenderPass::GetSubpassCount(void) const
	{
		return m_subpasses.size();
	}

}
