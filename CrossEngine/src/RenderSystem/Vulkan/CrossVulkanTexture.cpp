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

	CVulkanTexture::CVulkanTexture(CVulkanDevice *pDevice, CVulkanResourceManager *pManager)
		: CVulkanImage(pDevice, pManager)
		, m_pSampler(NULL)
	{

	}

	CVulkanTexture::~CVulkanTexture(void)
	{
		ASSERT(m_pSampler == NULL);
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

			m_vkDescriptorImageInfo.sampler = m_pSampler->GetSampler();
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

			m_vkDescriptorImageInfo.sampler = m_pSampler->GetSampler();
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

			m_vkDescriptorImageInfo.sampler = m_pSampler->GetSampler();
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
		m_pSampler = m_pDevice->GetSamplerManager()->AllocSampler();
		return m_pSampler->Create(minFilter, magFilter, mipmapMode, addressMode);
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
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->Submit(buffer.GetBuffer()->GetCommandBuffer()->GetCommandBuffer(), VK_NULL_HANDLE));
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->WaitIdle());
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
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->Submit(buffer.GetBuffer()->GetCommandBuffer()->GetCommandBuffer(), VK_NULL_HANDLE));
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->WaitIdle());
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
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->Submit(buffer.GetBuffer()->GetCommandBuffer()->GetCommandBuffer(), VK_NULL_HANDLE));
				CALL_VK_FUNCTION_THROW(m_pDevice->GetQueue()->WaitIdle());
			}

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);

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
		if (m_pSampler) {
			m_pSampler->Release();
			m_pSampler = NULL;
		}
	}

	const VkDescriptorImageInfo& CVulkanTexture::GetDescriptorImageInfo(void) const
	{
		return m_vkDescriptorImageInfo;
	}

}
