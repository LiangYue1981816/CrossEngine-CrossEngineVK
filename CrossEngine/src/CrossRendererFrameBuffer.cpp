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

	CRendererFrameBuffer::CRendererFrameBuffer(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkFrameBuffer(VK_NULL_HANDLE)

		, m_width(0)
		, m_height(0)
	{

	}

	CRendererFrameBuffer::~CRendererFrameBuffer(void)
	{
		ASSERT(m_vkFrameBuffer == VK_NULL_HANDLE);
	}

	BOOL CRendererFrameBuffer::SetAttachment(uint32_t indexAttachment, uint32_t width, uint32_t height, VkImageView vkView)
	{
		if (indexAttachment >= m_pDevice->GetDeviceProperties().limits.maxColorAttachments) {
			return FALSE;
		}

		if (m_width == 0) m_width = width;
		if (m_height == 0) m_height = height;
		if (m_width != width || m_height != height) return FALSE;

		m_views[indexAttachment] = vkView;

		return TRUE;
	}

	BOOL CRendererFrameBuffer::Create(VkRenderPass vkRenderPass)
	{
		try {
			std::vector<VkImageView> attachments;
			uint32_t numAttachment = CreateAttachments(attachments);

			VkFramebufferCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.renderPass = vkRenderPass;
			createInfo.attachmentCount = numAttachment;
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_width;
			createInfo.height = m_height;
			createInfo.layers = 1;
			CALL_VK_FUNCTION_THROW(vkCreateFramebuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkFrameBuffer));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	uint32_t CRendererFrameBuffer::CreateAttachments(std::vector<VkImageView> &attachments)
	{
		uint32_t numAttachment = 0;

		attachments.clear();
		attachments.resize(m_pDevice->GetDeviceProperties().limits.maxColorAttachments);

		for (const auto &itAttachment : m_views) {
			numAttachment = max(numAttachment, itAttachment.first + 1);
			attachments[itAttachment.first] = itAttachment.second;
		}

		return numAttachment;
	}

	void CRendererFrameBuffer::Destroy(void)
	{
		if (m_vkFrameBuffer) {
			vkDestroyFramebuffer(m_pDevice->GetDevice(), m_vkFrameBuffer, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkFrameBuffer = VK_NULL_HANDLE;

		m_width = 0;
		m_height = 0;
		m_views.clear();
	}

	void CRendererFrameBuffer::DumpLog(void) const
	{
		if (m_vkFrameBuffer) {
			LOGI("\t\tFrameBuffer 0x%x: width = %d height = %d\n", m_vkFrameBuffer, m_width, m_height);
			for (const auto &itAttachment : m_views) {
				if (const VkImageView view = itAttachment.second) {
					LOGI("\t\t\tAttachment %d: view = 0x%x\n", itAttachment.first, view);
				}
			}
		}
	}

	VkFramebuffer CRendererFrameBuffer::GetFrameBuffer(void) const
	{
		return m_vkFrameBuffer;
	}

}
