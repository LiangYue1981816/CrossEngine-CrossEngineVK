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

	CVulkanTexture::CVulkanTexture(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager)
		: CVulkanImage(pDevice, pResourceManager)
		, m_vkSampler(VK_NULL_HANDLE)
		, m_vkDescriptorImageInfo{}

		, m_minFilter(VK_FILTER_NEAREST)
		, m_magFilter(VK_FILTER_NEAREST)
		, m_mipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST)
		, m_addressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
	{

	}

	CVulkanTexture::~CVulkanTexture(void)
	{

	}

	BOOL CVulkanTexture::CreateTexture2D(const gli::texture2d &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		if (CVulkanHelper::vkIsFormatDepthOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatStencilOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatDepthStencil((VkFormat)texture.format())) {
			return FALSE;
		}

		try {
			CALL_BOOL_FUNCTION_THROW(Create(VK_IMAGE_VIEW_TYPE_2D, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT));
			CALL_BOOL_FUNCTION_THROW(CreateSampler(minFilter, magFilter, mipmapMode, addressMode));
			CALL_BOOL_FUNCTION_THROW(TransferTexture2D(texture));

			m_vkDescriptorImageInfo.sampler = m_vkSampler;
			m_vkDescriptorImageInfo.imageView = m_vkImageView;
			m_vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			return TRUE;
		}
		catch (BOOL) {
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanTexture::CreateTexture2DArray(const gli::texture2d_array &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		if (CVulkanHelper::vkIsFormatDepthOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatStencilOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatDepthStencil((VkFormat)texture.format())) {
			return FALSE;
		}

		try {
			CALL_BOOL_FUNCTION_THROW(Create(VK_IMAGE_VIEW_TYPE_2D_ARRAY, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), texture.layers(), VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT));
			CALL_BOOL_FUNCTION_THROW(CreateSampler(minFilter, magFilter, mipmapMode, addressMode));
			CALL_BOOL_FUNCTION_THROW(TransferTexture2DArray(texture));

			m_vkDescriptorImageInfo.sampler = m_vkSampler;
			m_vkDescriptorImageInfo.imageView = m_vkImageView;
			m_vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			return TRUE;
		}
		catch (BOOL) {
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanTexture::CreateTextureCube(const gli::texture_cube &texture, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		if (CVulkanHelper::vkIsFormatDepthOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatStencilOnly((VkFormat)texture.format()) || CVulkanHelper::vkIsFormatDepthStencil((VkFormat)texture.format())) {
			return FALSE;
		}

		try {
			CALL_BOOL_FUNCTION_THROW(Create(VK_IMAGE_VIEW_TYPE_CUBE, (VkFormat)texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.extent(0).x, texture.extent(0).y, 1, texture.levels(), 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT));
			CALL_BOOL_FUNCTION_THROW(CreateSampler(minFilter, magFilter, mipmapMode, addressMode));
			CALL_BOOL_FUNCTION_THROW(TransferTextureCube(texture));

			m_vkDescriptorImageInfo.sampler = m_vkSampler;
			m_vkDescriptorImageInfo.imageView = m_vkImageView;
			m_vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			return TRUE;
		}
		catch (BOOL) {
			Destroy();

			return FALSE;
		}
	}

	BOOL CVulkanTexture::CreateSampler(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		try {
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.magFilter = magFilter;
			createInfo.minFilter = minFilter;
			createInfo.mipmapMode = mipmapMode;
			createInfo.addressModeU = addressMode;
			createInfo.addressModeV = addressMode;
			createInfo.addressModeW = addressMode;
			createInfo.mipLodBias = 0.0f;
			createInfo.anisotropyEnable = VK_FALSE;
			createInfo.maxAnisotropy = 1.0f;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = 0.0f;
			createInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			CALL_VK_FUNCTION_THROW(vkCreateSampler(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

			m_minFilter = minFilter;
			m_magFilter = magFilter;
			m_mipmapMode = mipmapMode;
			m_addressMode = addressMode;

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanTexture::Destroy(void)
	{
		DestroySampler();
		CVulkanImage::Destroy();
	}

	void CVulkanTexture::DestroySampler(void)
	{
		if (m_vkSampler) {
			vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSampler = VK_NULL_HANDLE;
	}

	BOOL CVulkanTexture::TransferTexture2D(const gli::texture2d &texture)
	{
		try {
			CVulkanStagingBufferAutoRelease buffer(m_pDevice, m_pMemory->GetSize());
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

				CALL_VK_FUNCTION_THROW(buffer.GetBuffer()->TransferImage(m_vkImage, texture.levels(), 1, regions.size(), regions.data(), texture.size(), texture.data()));
			}

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);

			return FALSE;
		}
	}

	BOOL CVulkanTexture::TransferTexture2DArray(const gli::texture2d_array &texture)
	{
		try {
			CVulkanStagingBufferAutoRelease buffer(m_pDevice, m_pMemory->GetSize());
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

				CALL_VK_FUNCTION_THROW(buffer.GetBuffer()->TransferImage(m_vkImage, texture.levels(), texture.layers(), regions.size(), regions.data(), texture.size(), texture.data()));
			}

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);

			return FALSE;
		}
	}

	BOOL CVulkanTexture::TransferTextureCube(const gli::texture_cube &texture)
	{
		try {
			CVulkanStagingBufferAutoRelease buffer(m_pDevice, m_pMemory->GetSize());
			{
				uint32_t offset = 0;
				std::vector<VkBufferImageCopy> regions;

				for (uint32_t face = 0; face < 6; face++) {
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

				CALL_VK_FUNCTION_THROW(buffer.GetBuffer()->TransferImage(m_vkImage, texture.levels(), 6, regions.size(), regions.data(), texture.size(), texture.data()));
			}

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);

			return FALSE;
		}
	}
	
	const VkDescriptorImageInfo& CVulkanTexture::GetDescriptorImageInfo(void) const
	{
		return m_vkDescriptorImageInfo;
	}

	void CVulkanTexture::DumpLog(void) const
	{
		if (m_vkImage) {
			LOGI("\t\tTexture 0x%x: view = 0x%x sampler = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s tiling = %s min filter = %s mag filter = %s mipmap mode = %s address mode = %s\n",
				m_vkImage,
				m_vkImageView,
				m_vkSampler,
				m_size,
				CVulkanHelper::vkImageTypeToString(m_type),
				CVulkanHelper::vkFormatToString(m_format),
				m_width, m_height, m_depth, m_mipLevels, m_arrayLayers,
				CVulkanHelper::vkSampleCountFlagBitsToString(m_samples),
				CVulkanHelper::vkImageTilingToString(m_tiling),
				CVulkanHelper::vkFilterToString(m_minFilter),
				CVulkanHelper::vkFilterToString(m_magFilter),
				CVulkanHelper::vkSamplerMipmapModeToString(m_mipmapMode),
				CVulkanHelper::vkSamplerAddressModeToString(m_addressMode));
		}
	}

}
