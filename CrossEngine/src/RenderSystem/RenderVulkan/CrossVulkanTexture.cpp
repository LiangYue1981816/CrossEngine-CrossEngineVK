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

	CVulkanTexture::CVulkanTexture(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanImage(pDevice)
		, CGfxTexture(pResourceManager)
	{

	}

	CVulkanTexture::~CVulkanTexture(void)
	{

	}

	CVulkanDevice* CVulkanTexture::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanTexture::GetHandle(void) const
	{
		return m_vkImageView;
	}

	BOOL CVulkanTexture::CreateTexture2D(const gli::texture2d &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		CALL_BOOL_FUNCTION_RETURN(Create(VK_IMAGE_VIEW_TYPE_2D, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, minFilter, magFilter, mipmapMode, addressMode));
		CALL_BOOL_FUNCTION_RETURN(TransferTexture2D(texture));

		return TRUE;
	}

	BOOL CVulkanTexture::CreateTexture2DArray(const gli::texture2d_array &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		CALL_BOOL_FUNCTION_RETURN(Create(VK_IMAGE_VIEW_TYPE_2D_ARRAY, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), texture.layers(), VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, minFilter, magFilter, mipmapMode, addressMode));
		CALL_BOOL_FUNCTION_RETURN(TransferTexture2DArray(texture));

		return TRUE;
	}

	BOOL CVulkanTexture::CreateTextureCube(const gli::texture_cube &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		CALL_BOOL_FUNCTION_RETURN(Create(VK_IMAGE_VIEW_TYPE_CUBE, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, minFilter, magFilter, mipmapMode, addressMode));
		CALL_BOOL_FUNCTION_RETURN(TransferTextureCube(texture));

		return TRUE;
	}

	void CVulkanTexture::Destroy(void)
	{
		CVulkanImage::Destroy();
	}

	BOOL CVulkanTexture::TransferTexture2D(const gli::texture2d &texture)
	{
		uint32_t offset = 0;
		std::vector<VkBufferImageCopy> regions;

		for (uint32_t level = 0; level < texture.levels(); level++) {
			VkBufferImageCopy region;
			region.bufferOffset = offset;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, level, 0, 1 };
			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = { (uint32_t)texture.extent(level).x, (uint32_t)texture.extent(level).y, 1 };

			regions.push_back(region);
			offset += texture.size(level);
		}

		CVulkanStagingBufferPtr ptrStagingBuffer = m_pDevice->GetStagingBufferManager()->AllocBuffer(m_pMemory->GetSize());
		CALL_VK_FUNCTION_RETURN_BOOL(ptrStagingBuffer->TransferImage(m_vkImage, texture.levels(), 1, regions.size(), regions.data(), texture.size(), texture.data()));

		return TRUE;
	}

	BOOL CVulkanTexture::TransferTexture2DArray(const gli::texture2d_array &texture)
	{
		uint32_t offset = 0;
		std::vector<VkBufferImageCopy> regions;

		for (uint32_t layer = 0; layer < texture.layers(); layer++) {
			for (uint32_t level = 0; level < texture.levels(); level++) {
				VkBufferImageCopy region;
				region.bufferOffset = offset;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;
				region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, level, layer, 1 };
				region.imageOffset = { 0, 0, 0 };
				region.imageExtent = { (uint32_t)texture[layer].extent(level).x, (uint32_t)texture[layer].extent(level).y, 1 };

				regions.push_back(region);
				offset += texture[layer].size(level);
			}
		}

		CVulkanStagingBufferPtr ptrStagingBuffer = m_pDevice->GetStagingBufferManager()->AllocBuffer(m_pMemory->GetSize());
		CALL_VK_FUNCTION_RETURN_BOOL(ptrStagingBuffer->TransferImage(m_vkImage, texture.levels(), texture.layers(), regions.size(), regions.data(), texture.size(), texture.data()));

		return TRUE;
	}

	BOOL CVulkanTexture::TransferTextureCube(const gli::texture_cube &texture)
	{
		uint32_t offset = 0;
		std::vector<VkBufferImageCopy> regions;

		for (uint32_t face = 0; face < texture.faces(); face++) {
			for (uint32_t level = 0; level < texture.levels(); level++) {
				VkBufferImageCopy region;
				region.bufferOffset = offset;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;
				region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, level, face, 1 };
				region.imageOffset = { 0, 0, 0 };
				region.imageExtent = { (uint32_t)texture[face].extent(level).x, (uint32_t)texture[face].extent(level).y, 1 };

				regions.push_back(region);
				offset += texture[face].size(level);
			}
		}

		CVulkanStagingBufferPtr ptrStagingBuffer = m_pDevice->GetStagingBufferManager()->AllocBuffer(m_pMemory->GetSize());
		CALL_VK_FUNCTION_RETURN_BOOL(ptrStagingBuffer->TransferImage(m_vkImage, texture.levels(), 6, regions.size(), regions.data(), texture.size(), texture.data()));

		return TRUE;
	}

	uint32_t CVulkanTexture::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CVulkanTexture::GetHeight(void) const
	{
		return m_height;
	}

	uint32_t CVulkanTexture::GetDepth(void) const
	{
		return m_depth;
	}

	uint32_t CVulkanTexture::GetSamples(void) const
	{
		return m_samples;
	}

	size_t CVulkanTexture::GetMemorySize(void) const
	{
		return m_pMemory->GetSize();
	}

	void CVulkanTexture::DumpLog(void) const
	{
		if (m_vkImage) {
			LOGI("\t\tTexture 0x%x: view = 0x%x sampler = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s tiling = %s min filter = %s mag filter = %s mipmap mode = %s address mode = %s\n",
				m_vkImage,
				m_vkImageView,
				m_vkSampler,
				m_pMemory->GetSize(),
				CVulkanHelper::ImageTypeToString(m_type),
				CVulkanHelper::FormatToString(m_format),
				m_width, m_height, m_depth, m_mipLevels, m_arrayLayers,
				CVulkanHelper::SampleCountFlagBitsToString(m_samples),
				CVulkanHelper::ImageTilingToString(m_tiling),
				CVulkanHelper::FilterToString(m_minFilter),
				CVulkanHelper::FilterToString(m_magFilter),
				CVulkanHelper::SamplerMipmapModeToString(m_mipmapMode),
				CVulkanHelper::SamplerAddressModeToString(m_addressMode));
		}
	}

}
