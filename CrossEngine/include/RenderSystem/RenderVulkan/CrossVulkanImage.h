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

	class CROSS_EXPORT CVulkanImage
	{
	protected:
		CVulkanImage(CVulkanDevice *pDevice);
		virtual ~CVulkanImage(void);


	protected:
		BOOL Create(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		void Destroy(void);

	protected:
		int CreateImage(VkImageViewType viewType, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage);
		int CreateImageView(VkImageViewType viewType, VkImageAspectFlags aspectMask);
		int CreateSampler(uint32_t mipLevels, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		int CheckParameters(const VkImageCreateInfo &createInfo) const;

		void DestroyImage(void);
		void DestroyImageView(void);
		void DestroySampler(void);

	public:
		VkFormat GetFormat(void) const;
		VkImageType GetType(void) const;
		const VkDescriptorImageInfo& GetDescriptorImageInfo(void) const;


	protected:
		CVulkanMemory *m_pMemory;

	protected:
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint32_t m_mipLevels;
		uint32_t m_arrayLayers;

		VkFormat m_format;
		VkImageType m_type;
		VkImageTiling m_tiling;
		VkSampleCountFlagBits m_samples;

		VkFilter m_minFilter;
		VkFilter m_magFilter;
		VkSamplerMipmapMode m_mipmapMode;
		VkSamplerAddressMode m_addressMode;

	protected:
		VkImage m_vkImage;
		VkImageView m_vkImageView;
		VkSampler m_vkSampler;
		VkDescriptorImageInfo m_vkDescriptorImageInfo;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
