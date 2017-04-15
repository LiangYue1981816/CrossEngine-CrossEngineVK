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

	static uint32_t GetSwapchainNumImages(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		return max(min(3, capabilities.maxImageCount), capabilities.minImageCount);
	}

	static VkExtent2D GetSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height)
	{
		VkExtent2D extent = { width, height };

		if (extent.width < capabilities.minImageExtent.width) extent.width = capabilities.minImageExtent.width;
		if (extent.width > capabilities.maxImageExtent.width) extent.width = capabilities.maxImageExtent.width;
		if (extent.height < capabilities.minImageExtent.height) extent.height = capabilities.minImageExtent.height;
		if (extent.height > capabilities.maxImageExtent.height) extent.height = capabilities.maxImageExtent.height;

		return extent;
	}

	static VkImageUsageFlags GetSwapchainUsageFlags(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		ASSERT((capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}

	static VkSurfaceTransformFlagBitsKHR GetSwapchainTransform(const VkSurfaceCapabilitiesKHR &capabilities, VkSurfaceTransformFlagBitsKHR transform)
	{
		if (capabilities.supportedTransforms & transform) {
			return transform;
		}
		else {
			return capabilities.currentTransform;
		}
	}

	static VkPresentModeKHR GetSwapchainMode(const std::vector<VkPresentModeKHR> &modes)
	{
		for (uint32_t index = 0; index < modes.size(); index++) {
			if (modes[index] == VK_PRESENT_MODE_MAILBOX_KHR) {
				return modes[index];
			}

			if (modes[index] == VK_PRESENT_MODE_FIFO_KHR) {
				return modes[index];
			}
		}

		return (VkPresentModeKHR)(-1);
	}

	static VkSurfaceFormatKHR GetSwapchainFormat(const std::vector<VkSurfaceFormatKHR> &formats)
	{
		if ((formats.size() == 1) && (formats[0].format == VK_FORMAT_UNDEFINED)) {
			return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		for (uint32_t index = 0; index < formats.size(); index++) {
			if (formats[index].format == VK_FORMAT_R8G8B8A8_UNORM) {
				return formats[index];
			}
		}

		return formats[0];
	}


	CRendererSwapchain::CRendererSwapchain(CRendererDeviceGraphics *pDevice)
		: m_pDevice(pDevice)
		, m_vkSwapchain(VK_NULL_HANDLE)

		, m_indexImage(0)
		, m_indexSemaphore(0)

		, m_width(0)
		, m_height(0)
		, m_format(VK_FORMAT_UNDEFINED)
	{

	}

	CRendererSwapchain::~CRendererSwapchain(void)
	{
		ASSERT(m_vkSwapchain == VK_NULL_HANDLE);
		ASSERT(m_images.empty());
		ASSERT(m_views.empty());
		ASSERT(m_semaphores.empty());
	}

	BOOL CRendererSwapchain::Create(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		try {
			std::vector<VkPresentModeKHR> modes;
			std::vector<VkSurfaceFormatKHR> formats;
			VkSurfaceCapabilitiesKHR capabilities;

			CALL_VK_FUNCTION_THROW(EnumDeviceSurfaceModes(modes));
			CALL_VK_FUNCTION_THROW(EnumDeviceSurfaceFormats(formats));
			CALL_VK_FUNCTION_THROW(EnumDeviceSurfaceCapabilities(capabilities));
			CALL_VK_FUNCTION_THROW(CreateSwapchain(width, height, transform, modes, formats, capabilities));
			CALL_VK_FUNCTION_THROW(CreateImagesAndImageViews());

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererSwapchain::Destroy(void)
	{
		DestroyImagesAndImageViews();
		DestroySwapchain();
	}

	VkResult CRendererSwapchain::EnumDeviceSurfaceModes(std::vector<VkPresentModeKHR> &modes) const
	{
		modes.clear();

		uint32_t numModes;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetRenderer()->GetSurface(), &numModes, NULL));

		ASSERT(numModes > 0);
		modes.resize(numModes);
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetRenderer()->GetSurface(), &numModes, modes.data()));

		return VK_SUCCESS;
	}

	VkResult CRendererSwapchain::EnumDeviceSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats) const
	{
		formats.clear();

		uint32_t numFormats;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetRenderer()->GetSurface(), &numFormats, NULL));

		ASSERT(numFormats > 0);
		formats.resize(numFormats);
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetRenderer()->GetSurface(), &numFormats, formats.data()));

		return VK_SUCCESS;
	}

	VkResult CRendererSwapchain::EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const
	{
		return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetRenderer()->GetSurface(), &capabilities);
	}

	VkResult CRendererSwapchain::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform, const std::vector<VkPresentModeKHR> &modes, const std::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities)
	{
		uint32_t numImages = GetSwapchainNumImages(capabilities);
		VkExtent2D imageExtent = GetSwapchainExtent(capabilities, width, height);
		VkImageUsageFlags imageUsage = GetSwapchainUsageFlags(capabilities);
		VkSurfaceTransformFlagBitsKHR preTransform = GetSwapchainTransform(capabilities, transform);
		VkPresentModeKHR presentMode = GetSwapchainMode(modes);
		VkSurfaceFormatKHR imageFormat = GetSwapchainFormat(formats);

		m_format = imageFormat.format;

		VkSwapchainCreateInfoKHR swapchainInfo;
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.pNext = NULL;
		swapchainInfo.flags = 0;
		swapchainInfo.surface = m_pDevice->GetRenderer()->GetSurface();
		swapchainInfo.minImageCount = numImages;
		swapchainInfo.imageFormat = imageFormat.format;
		swapchainInfo.imageColorSpace = imageFormat.colorSpace;
		swapchainInfo.imageExtent = imageExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = imageUsage;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = NULL;
		swapchainInfo.preTransform = preTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = TRUE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		return vkCreateSwapchainKHR(m_pDevice->GetDevice(), &swapchainInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkSwapchain);
	}

	VkResult CRendererSwapchain::CreateImagesAndImageViews(void)
	{
		uint32_t numImages;
		CALL_VK_FUNCTION_RETURN(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, NULL));

		ASSERT(numImages > 0);
		m_images.resize(numImages);
		CALL_VK_FUNCTION_RETURN(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, m_images.data()));

		m_views.resize(numImages);
		for (uint32_t index = 0; index < numImages; index++) {
			VkImageViewCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.image = m_images[index];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_format;
			createInfo.components = CRendererHelper::vkGetFormatComponentMapping(m_format);
			createInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
			CALL_VK_FUNCTION_RETURN(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_views[index]));
		}

		m_semaphores.resize(numImages);
		for (uint32_t index = 0; index < numImages; index++) {
			VkSemaphoreCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			CALL_VK_FUNCTION_RETURN(vkCreateSemaphore(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_semaphores[index]));
		}

		return VK_SUCCESS;
	}

	void CRendererSwapchain::DestroySwapchain(void)
	{
		if (m_vkSwapchain) {
			vkDestroySwapchainKHR(m_pDevice->GetDevice(), m_vkSwapchain, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSwapchain = VK_NULL_HANDLE;
	}

	void CRendererSwapchain::DestroyImagesAndImageViews(void)
	{
		for (uint32_t index = 0; index < m_views.size(); index++) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_views[index], m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		for (uint32_t index = 0; index < m_semaphores.size(); index++) {
			vkDestroySemaphore(m_pDevice->GetDevice(), m_semaphores[index], m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_images.clear();
		m_views.clear();
		m_semaphores.clear();
	}

	VkResult CRendererSwapchain::Present(VkSemaphore vkSemaphoreRenderingDone) const
	{
		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &vkSemaphoreRenderingDone;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_vkSwapchain;
		presentInfo.pImageIndices = &m_indexImage;
		presentInfo.pResults = NULL;
		return vkQueuePresentKHR(m_pDevice->GetQueue()->GetQueue(), &presentInfo);
	}

	VkResult CRendererSwapchain::AcquireNextImage(VkFence vkFence)
	{
		m_indexImage = 0;
		m_indexSemaphore = (m_indexSemaphore + 1) % m_semaphores.size();
		return vkAcquireNextImageKHR(m_pDevice->GetDevice(), m_vkSwapchain, UINT64_MAX, m_semaphores[m_indexSemaphore], vkFence, &m_indexImage);
	}

	uint32_t CRendererSwapchain::GetImageIndex(void) const
	{
		return m_indexImage;
	}

	uint32_t CRendererSwapchain::GetImageCount(void) const
	{
		return m_images.size();
	}

	VkImage CRendererSwapchain::GetImage(uint32_t indexImage) const
	{
		return m_images[indexImage];
	}

	VkImageView CRendererSwapchain::GetImageView(uint32_t indexImage) const
	{
		return m_views[indexImage];
	}

	uint32_t CRendererSwapchain::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CRendererSwapchain::GetHeight(void) const
	{
		return m_height;
	}

	VkFormat CRendererSwapchain::GetFormat(void) const
	{
		return m_format;
	}

}
