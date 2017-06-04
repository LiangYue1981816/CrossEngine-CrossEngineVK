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

	class CROSS_EXPORT CVulkanSwapchain : public CGfxSwapchain
	{
		friend class CVulkanInstance;


	protected:
		CVulkanSwapchain(CVulkanDevice *pDevice);
		virtual ~CVulkanSwapchain(void);


	protected:
		VkResult Create(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		void Destroy(void);

	protected:
		VkResult EnumDeviceSurfaceModes(std::vector<VkPresentModeKHR> &modes) const;
		VkResult EnumDeviceSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats) const;
		VkResult EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const;
		VkResult CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform, const std::vector<VkPresentModeKHR> &modes, const std::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities);
		VkResult CreateImagesAndImageViews(void);

	protected:
		void DestroySwapchain(void);
		void DestroyImagesAndImageViews(void);

	public:
		BOOL Present(void) const;
		BOOL AcquireNextImage(CGfxFence fence);

		CGfxSemaphore GetAcquireSemaphore(void) const;
		CGfxSemaphore GetRenderDoneSemaphore(void) const;

	public:
		uint32_t GetImageIndex(void) const;
		uint32_t GetImageCount(void) const;

	public:
		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		VkFormat GetFormat(void) const;


	protected:
		static const int SWAPCHAIN_IMAGE_COUNT = 3;

	protected:
		uint32_t m_width;
		uint32_t m_height;
		VkFormat m_format;

	protected:
		uint32_t m_indexImage;
		VkImage m_vkImages[SWAPCHAIN_IMAGE_COUNT];
		VkImageView m_vkImageViews[SWAPCHAIN_IMAGE_COUNT];

	protected:
		VkSwapchainKHR m_vkSwapchain;
		VkSemaphore m_vkAcquireSemaphore;
		VkSemaphore m_vkRenderDoneSemaphores[SWAPCHAIN_IMAGE_COUNT];

	protected:
		CVulkanDevice *m_pDevice;
	};

}
