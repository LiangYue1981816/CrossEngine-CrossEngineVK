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

	CVulkanSwapchain::CVulkanSwapchain(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)

		, m_vkSwapchain(VK_NULL_HANDLE)
		, m_vkAcquireSemaphore(VK_NULL_HANDLE)

		, m_indexImage(0)
		, m_vkImages{ VK_NULL_HANDLE }
		, m_vkImageViews{ VK_NULL_HANDLE }
		, m_vkRenderDoneSemaphores{ VK_NULL_HANDLE }
	{

	}

	CVulkanSwapchain::~CVulkanSwapchain(void)
	{

	}

	VkResult CVulkanSwapchain::Create(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		return VK_SUCCESS;
	}

	void CVulkanSwapchain::Destroy(void)
	{

	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceModes(std::vector<VkPresentModeKHR> &modes) const
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats) const
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform, const std::vector<VkPresentModeKHR> &modes, const std::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities)
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanSwapchain::CreateImagesAndImageViews(void)
	{
		return VK_SUCCESS;
	}

	void CVulkanSwapchain::DestroySwapchain(void)
	{

	}

	void CVulkanSwapchain::DestroyImagesAndImageViews(void)
	{

	}

	BOOL CVulkanSwapchain::Present(void) const
	{
		return TRUE;
	}

	BOOL CVulkanSwapchain::AcquireNextImage(CGfxFence fence)
	{
		return TRUE;
	}

	CGfxSemaphore CVulkanSwapchain::GetAcquireSemaphore(void) const
	{
		return m_vkAcquireSemaphore;
	}

	CGfxSemaphore CVulkanSwapchain::GetRenderDoneSemaphore(void) const
	{
		return m_vkRenderDoneSemaphores[m_indexImage];
	}

}
