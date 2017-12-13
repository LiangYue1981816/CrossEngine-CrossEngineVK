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

	CVulkanFrameBuffer::CVulkanFrameBuffer(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager, uint32_t numAttachments)
		: CGfxFrameBuffer(pResourceManager)
		, m_pDevice(pDevice)
		, m_vkFrameBuffer(VK_NULL_HANDLE)

		, m_width(0)
		, m_height(0)
	{
		m_attachments.resize(numAttachments);
	}

	CVulkanFrameBuffer::~CVulkanFrameBuffer(void)
	{

	}

	CVulkanDevice* CVulkanFrameBuffer::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanFrameBuffer::GetHandle(void) const
	{
		return m_vkFrameBuffer;
	}

	BOOL CVulkanFrameBuffer::Create(HANDLE hRenderPass)
	{
		try {
			std::vector<VkImageView> attachments;
			CALL_BOOL_FUNCTION_THROW(CreateAttachments(attachments));

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.renderPass = (VkRenderPass)hRenderPass;
			createInfo.attachmentCount = attachments.size();
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_width;
			createInfo.height = m_height;
			createInfo.layers = 1;
			CALL_VK_FUNCTION_THROW(vkCreateFramebuffer(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkFrameBuffer));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanFrameBuffer::CreateAttachments(std::vector<VkImageView> &attachments)
	{
		attachments.clear();

		for (uint32_t indexAttachment = 0; indexAttachment < m_attachments.size(); indexAttachment++) {
			attachments.push_back(m_attachments[indexAttachment].vkImageView);
		}

		return TRUE;
	}

	void CVulkanFrameBuffer::Destroy(void)
	{
		if (m_vkFrameBuffer) {
			vkDestroyFramebuffer(m_pDevice->GetDevice(), m_vkFrameBuffer, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_width = 0;
		m_height = 0;
		m_vkFrameBuffer = VK_NULL_HANDLE;
	}

	BOOL CVulkanFrameBuffer::SetAttachment(uint32_t indexAttachment, VkFormat format, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		if (indexAttachment >= m_attachments.size()) {
			return FALSE;
		}

		if (m_width == 0) m_width = width;
		if (m_height == 0) m_height = height;
		if (m_width != width || m_height != height) return FALSE;

		m_attachments[indexAttachment].foramt = format;
		m_attachments[indexAttachment].vkImageView = (VkImageView)hImageView;

		return TRUE;
	}

	BOOL CVulkanFrameBuffer::SetPresentAttachment(uint32_t indexAttachment, VkFormat format, uint32_t width, uint32_t height, HANDLE hImageView)
	{
		if (SetAttachment(indexAttachment, format, width, height, hImageView)) {
			m_attachments[indexAttachment].ptrRenderTexture.Release();
			return TRUE;
		}

		return FALSE;
	}

	BOOL CVulkanFrameBuffer::SetColorAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		if (SetAttachment(indexAttachment, ((CVulkanRenderTexture *)((CGfxRenderTexture *)ptrRenderTexture))->GetFormat(), ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle())) {
			m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
			return TRUE;
		}

		return FALSE;
	}

	BOOL CVulkanFrameBuffer::SetDepthStencilAttachment(uint32_t indexAttachment, const CGfxRenderTexturePtr &ptrRenderTexture)
	{
		if (SetAttachment(indexAttachment, ((CVulkanRenderTexture *)((CGfxRenderTexture *)ptrRenderTexture))->GetFormat(), ptrRenderTexture->GetWidth(), ptrRenderTexture->GetHeight(), ptrRenderTexture->GetHandle())) {
			m_attachments[indexAttachment].ptrRenderTexture = ptrRenderTexture;
			return TRUE;
		}

		return FALSE;
	}

	uint32_t CVulkanFrameBuffer::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CVulkanFrameBuffer::GetHeight(void) const
	{
		return m_height;
	}

	void CVulkanFrameBuffer::DumpLog(void) const
	{
		if (m_vkFrameBuffer) {
			LOGI("\t\tFrameBuffer 0x%x: width = %d height = %d\n", m_vkFrameBuffer, m_width, m_height);
			for (uint32_t indexAttachment = 0; indexAttachment < m_attachments.size(); indexAttachment++) {
				LOGI("\t\t\tAttachment %d: view = 0x%x format = %s\n", indexAttachment, m_attachments[indexAttachment].vkImageView, CVulkanHelper::vkFormatToString(m_attachments[indexAttachment].foramt));
			}
		}
	}

}
