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

	static VkPresentModeKHR GetSwapchainPresentMode(const std::vector<VkPresentModeKHR> &modes)
	{
		for (uint32_t index = 0; index < modes.size(); index++) {
			if (modes[index] == VK_PRESENT_MODE_MAILBOX_KHR) {
				return modes[index];
			}

			if (modes[index] == VK_PRESENT_MODE_FIFO_KHR) {
				return modes[index];
			}
		}

		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	}

	static VkSurfaceFormatKHR GetSwapchainFormat(const std::vector<VkSurfaceFormatKHR> &formats)
	{
		if ((formats.size() == 1) && (formats[0].format == VK_FORMAT_UNDEFINED)) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		for (uint32_t index = 0; index < formats.size(); index++) {
			if (formats[index].format == VK_FORMAT_B8G8R8A8_UNORM) {
				return formats[index];
			}
		}

		return formats[0];
	}


	CVulkanSwapchain::CVulkanSwapchain(CVulkanDeviceGraphics *pDevice)
		: m_pDevice(pDevice)
		, m_vkSwapchain(VK_NULL_HANDLE)
		, m_vkAcquireSemaphore(VK_NULL_HANDLE)

		, m_width(0)
		, m_height(0)
		, m_format(VK_FORMAT_UNDEFINED)

		, m_indexImage(0)
	{

	}

	CVulkanSwapchain::~CVulkanSwapchain(void)
	{

	}

	BOOL CVulkanSwapchain::Create(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
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
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanSwapchain::Destroy(void)
	{
		DestroyImagesAndImageViews();
		DestroySwapchain();
	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceModes(std::vector<VkPresentModeKHR> &modes) const
	{
		modes.clear();

		uint32_t numModes;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetVulkan()->GetSurface(), &numModes, NULL));

		ASSERT(numModes > 0);
		modes.resize(numModes);
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetVulkan()->GetSurface(), &numModes, modes.data()));

		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats) const
	{
		formats.clear();

		uint32_t numFormats;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetVulkan()->GetSurface(), &numFormats, NULL));

		ASSERT(numFormats > 0);
		formats.resize(numFormats);
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetVulkan()->GetSurface(), &numFormats, formats.data()));

		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const
	{
		return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetVulkan()->GetSurface(), &capabilities);
	}

	VkResult CVulkanSwapchain::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform, const std::vector<VkPresentModeKHR> &modes, const std::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities)
	{
		uint32_t numImages = GetSwapchainNumImages(capabilities);
		VkExtent2D imageExtent = GetSwapchainExtent(capabilities, width, height);
		VkImageUsageFlags imageUsage = GetSwapchainUsageFlags(capabilities);
		VkSurfaceTransformFlagBitsKHR preTransform = GetSwapchainTransform(capabilities, transform);
		VkPresentModeKHR presentMode = GetSwapchainPresentMode(modes);
		VkSurfaceFormatKHR imageFormat = GetSwapchainFormat(formats);

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.pNext = NULL;
		swapchainInfo.flags = 0;
		swapchainInfo.surface = m_pDevice->GetVulkan()->GetSurface();
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
		CALL_VK_FUNCTION_RETURN(vkCreateSwapchainKHR(m_pDevice->GetDevice(), &swapchainInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkSwapchain));

		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		CALL_VK_FUNCTION_RETURN(vkCreateSemaphore(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkAcquireSemaphore));

		m_width = width;
		m_height = height;
		m_format = imageFormat.format;

		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::CreateImagesAndImageViews(void)
	{
		uint32_t numImages;
		CALL_VK_FUNCTION_RETURN(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, NULL));

		ASSERT(numImages > 0);
		m_images.resize(numImages);
		m_imageViews.resize(numImages);
		CALL_VK_FUNCTION_RETURN(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, m_images.data()));

		for (uint32_t index = 0; index < numImages; index++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.image = m_images[index];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_format;
			createInfo.components = CVulkanHelper::vkGetFormatComponentMapping(m_format);
			createInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
			CALL_VK_FUNCTION_RETURN(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_imageViews[index]));
		}

		return VK_SUCCESS;
	}

	void CVulkanSwapchain::DestroySwapchain(void)
	{
		if (m_vkSwapchain) {
			vkDestroySwapchainKHR(m_pDevice->GetDevice(), m_vkSwapchain, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_vkAcquireSemaphore) {
			vkDestroySemaphore(m_pDevice->GetDevice(), m_vkAcquireSemaphore, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSwapchain = VK_NULL_HANDLE;
		m_vkAcquireSemaphore = VK_NULL_HANDLE;
	}

	void CVulkanSwapchain::DestroyImagesAndImageViews(void)
	{
		for (uint32_t index = 0; index < m_imageViews.size(); index++) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_imageViews[index], m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_images.clear();
		m_imageViews.clear();
	}

	VkResult CVulkanSwapchain::Present(VkSemaphore vkSemaphoreWaitRenderingDone) const
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &vkSemaphoreWaitRenderingDone;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_vkSwapchain;
		presentInfo.pImageIndices = &m_indexImage;
		presentInfo.pResults = NULL;
		return vkQueuePresentKHR(m_pDevice->GetQueue()->GetQueue(), &presentInfo);
	}

	VkResult CVulkanSwapchain::AcquireNextImage(VkFence vkFence)
	{
		m_indexImage = -1;
		return vkAcquireNextImageKHR(m_pDevice->GetDevice(), m_vkSwapchain, UINT64_MAX, m_vkAcquireSemaphore, vkFence, &m_indexImage);
	}

	VkSemaphore CVulkanSwapchain::GetAcquireSemaphore(void) const
	{
		return m_vkAcquireSemaphore;
	}

	uint32_t CVulkanSwapchain::GetImageIndex(void) const
	{
		return m_indexImage;
	}

	uint32_t CVulkanSwapchain::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CVulkanSwapchain::GetHeight(void) const
	{
		return m_height;
	}

	VkFormat CVulkanSwapchain::GetFormat(void) const
	{
		return m_format;
	}

	uint32_t CVulkanSwapchain::GetImageCount(void) const
	{
		return m_images.size();
	}

	VkImageView CVulkanSwapchain::GetImageView(uint32_t indexImage) const
	{
		return m_imageViews[indexImage];
	}

}
