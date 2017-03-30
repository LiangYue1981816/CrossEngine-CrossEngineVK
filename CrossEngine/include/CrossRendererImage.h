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

	class CROSS_EXPORT CRendererImage : public CRendererResource
	{
		friend class CRendererTextureManager;


	protected:
		CRendererImage(CRendererDevice *pDevice, CRendererResourceManager *pManager);
		virtual ~CRendererImage(void);


	protected:
		virtual BOOL Create(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage);
		virtual void Destroy(void);
		virtual void DumpLog(void) const;

	protected:
		VkResult CreateImage(VkImageViewType viewType, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage);
		VkResult CreateImageView(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t mipLevels);
		VkResult CheckParameters(VkImageType type, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage) const;

	protected:
		void DestroyImage(void);
		void DestroyImageView(void);


	public:
		VkImage GetImage(void) const;
		VkImageView GetImageView(void) const;

		uint32_t GetWidth(void) const;
		uint32_t GetHeight(void) const;
		uint32_t GetDepth(void) const;
		uint32_t GetMips(void) const;
		uint32_t GetLayers(void) const;

		VkImageType GetType(void) const;
		VkFormat GetFormat(void) const;
		VkSampleCountFlagBits GetSamples(void) const;
		VkImageTiling GetTiling(void) const;
		VkDeviceSize GetSize(void) const;


	protected:
		VkImageType m_type;
		VkFormat m_format;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint32_t m_mipLevels;
		uint32_t m_arrayLayers;
		VkSampleCountFlagBits m_samples;
		VkImageTiling m_tiling;
		VkDeviceSize m_size;

	protected:
		VkImage m_vkImage;
		VkImageView m_vkImageView;
		CRendererMemory *m_pMemory;
	};

}
