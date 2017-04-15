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

	class CROSS_EXPORT CRendererSwapchain
	{
		friend class CRenderer;


	protected:
		CRendererSwapchain(CRendererDeviceGraphics *pDevice);
		virtual ~CRendererSwapchain(void);


	protected:
		BOOL Create(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
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
		VkResult Present(VkSemaphore vkSemaphoreRenderingDone) const;
		VkResult AcquireNextImage(VkFence vkFence);
		uint32_t GetImageIndex(void) const;


	public:
		uint32_t GetImageCount(void) const;
		VkImage  GetImage(uint32_t indexImage) const;
		VkImageView GetImageView(uint32_t indexImage) const;

		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		VkFormat GetFormat(void) const;


	protected:
		uint32_t m_indexImage;
		uint32_t m_indexSemaphore;

		uint32_t m_width;
		uint32_t m_height;
		VkFormat m_format;
		
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_views;
		std::vector<VkSemaphore> m_semaphores;

		VkSwapchainKHR m_vkSwapchain;

	protected:
		CRendererDeviceGraphics *m_pDevice;
	};

}
