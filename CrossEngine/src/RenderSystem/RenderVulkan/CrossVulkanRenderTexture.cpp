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

	CVulkanRenderTexture::CVulkanRenderTexture(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanImage(pDevice)
		, CGfxRenderTexture(pResourceManager)
	{

	}

	CVulkanRenderTexture::~CVulkanRenderTexture(void)
	{

	}

	CVulkanDevice* CVulkanRenderTexture::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanRenderTexture::GetHandle(void) const
	{
		return m_vkImageView;
	}

	BOOL CVulkanRenderTexture::CreateColorTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		if (CVulkanHelper::IsFormatDepthOnly(format)) {
			return FALSE;
		}

		if (CVulkanHelper::IsFormatStencilOnly(format)) {
			return FALSE;
		}

		if (CVulkanHelper::IsFormatDepthStencil(format)) {
			return FALSE;
		}

		return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_COLOR_BIT, width, height, 1, 1, 1, samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, minFilter, magFilter, mipmapMode, addressMode);
	}

	BOOL CVulkanRenderTexture::CreateDepthStencilTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		if (CVulkanHelper::IsFormatDepthOnly(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT, width, height, 1, 1, 1, samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, minFilter, magFilter, mipmapMode, addressMode);
		}

		if (CVulkanHelper::IsFormatStencilOnly(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_STENCIL_BIT, width, height, 1, 1, 1, samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, minFilter, magFilter, mipmapMode, addressMode);
		}

		if (CVulkanHelper::IsFormatDepthStencil(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, width, height, 1, 1, 1, samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, minFilter, magFilter, mipmapMode, addressMode);
		}

		return FALSE;
	}

	void CVulkanRenderTexture::Destroy(void)
	{
		CVulkanImage::Destroy();
	}

	uint32_t CVulkanRenderTexture::GetWidth(void) const
	{
		return CVulkanImage::GetWidth();
	}

	uint32_t CVulkanRenderTexture::GetHeight(void) const
	{
		return CVulkanImage::GetHeight();
	}

	uint32_t CVulkanRenderTexture::GetDepth(void) const
	{
		return CVulkanImage::GetDepth();
	}

	uint32_t CVulkanRenderTexture::GetSamples(void) const
	{
		return CVulkanImage::GetSamples();
	}

	size_t CVulkanRenderTexture::GetMemorySize(void) const
	{
		return CVulkanImage::GetMemorySize();
	}

	void CVulkanRenderTexture::DumpLog(void) const
	{
		if (m_vkImage) {
			LOGI("\t\tRenderTexture 0x%x: view = 0x%x sampler = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s tiling = %s min filter = %s mag filter = %s mipmap mode = %s address mode = %s\n",
				m_vkImage,
				m_vkImageView,
				m_vkSampler,
				GetMemorySize(),
				CVulkanHelper::ImageTypeToString(m_type),
				CVulkanHelper::FormatToString(m_format),
				GetWidth(), GetHeight(), GetDepth(), GetMipLevels(), GetArrayLayers(),
				CVulkanHelper::SampleCountFlagBitsToString(m_samples),
				CVulkanHelper::ImageTilingToString(m_tiling),
				CVulkanHelper::FilterToString(m_minFilter),
				CVulkanHelper::FilterToString(m_magFilter),
				CVulkanHelper::SamplerMipmapModeToString(m_mipmapMode),
				CVulkanHelper::SamplerAddressModeToString(m_addressMode));
		}
	}

}
