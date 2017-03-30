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

	CRendererImage::CRendererImage(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkImage(VK_NULL_HANDLE)
		, m_vkImageView(VK_NULL_HANDLE)

		, m_type(VK_IMAGE_TYPE_2D)
		, m_format(VK_FORMAT_R8G8B8A8_UNORM)
		, m_width(0)
		, m_height(0)
		, m_depth(0)
		, m_mipLevels(0)
		, m_arrayLayers(0)
		, m_samples(VK_SAMPLE_COUNT_1_BIT)
		, m_tiling(VK_IMAGE_TILING_OPTIMAL)
		, m_size(0)

		, m_pMemory(NULL)
	{

	}

	CRendererImage::~CRendererImage(void)
	{
		ASSERT(m_pMemory == NULL);
		ASSERT(m_vkImage == VK_NULL_HANDLE);
		ASSERT(m_vkImageView == VK_NULL_HANDLE);
	}

	BOOL CRendererImage::Create(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage)
	{
		try {
			CALL_VK_FUNCTION_THROW(CreateImage(viewType, format, width, height, depth, mipLevels, arrayLayers, samples, tiling, usage));
			CALL_VK_FUNCTION_THROW(CreateImageView(viewType, format, aspectMask, mipLevels));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererImage::Destroy(void)
	{
		DestroyImageView();
		DestroyImage();
	}

	void CRendererImage::DumpLog(void) const
	{
		if (m_vkImage) {
			LOGI("\t\tTexture 0x%x: view = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s tiling = %s\n",
				m_vkImage,
				m_vkImageView,
				m_size,
				CRendererHelper::vkImageTypeToString(m_type),
				CRendererHelper::vkFormatToString(m_format),
				m_width, m_height, m_depth, m_mipLevels, m_arrayLayers,
				CRendererHelper::vkSampleCountFlagBitsToString(m_samples),
				CRendererHelper::vkImageTilingToString(m_tiling));
		}
	}

	VkResult CRendererImage::CreateImage(VkImageViewType viewType, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage)
	{
		depth = max(depth, 1);
		width = max(width, 1);
		height = max(height, 1);
		mipLevels = max(mipLevels, 1);

		if (CRendererHelper::vkIsFormatSupported(format) == FALSE) {
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

		VkImageCreateInfo imageInfo;
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.pNext = NULL;
		imageInfo.flags = 0;
		imageInfo.format = format;
		imageInfo.mipLevels = mipLevels;
		imageInfo.samples = samples;
		imageInfo.tiling = tiling;
		imageInfo.usage = usage | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = 0;
		imageInfo.pQueueFamilyIndices = NULL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		switch (viewType) {
		case VK_IMAGE_VIEW_TYPE_1D:
		case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
			if (width > m_pDevice->GetDeviceProperties().limits.maxImageDimension1D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			imageInfo.imageType = VK_IMAGE_TYPE_1D;
			imageInfo.arrayLayers = arrayLayers;
			imageInfo.extent.width = width;
			imageInfo.extent.height = 1;
			imageInfo.extent.depth = 1;
			break;

		case VK_IMAGE_VIEW_TYPE_2D:
		case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
			if (width > m_pDevice->GetDeviceProperties().limits.maxImageDimension2D || height > m_pDevice->GetDeviceProperties().limits.maxImageDimension2D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.arrayLayers = arrayLayers;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			break;

		case VK_IMAGE_VIEW_TYPE_CUBE:
		case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
			if (width > m_pDevice->GetDeviceProperties().limits.maxImageDimensionCube || height > m_pDevice->GetDeviceProperties().limits.maxImageDimensionCube) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			if (width != height) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.arrayLayers = 6;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			break;

		case VK_IMAGE_VIEW_TYPE_3D:
			if (width > m_pDevice->GetDeviceProperties().limits.maxImageDimension3D || height > m_pDevice->GetDeviceProperties().limits.maxImageDimension3D || depth > m_pDevice->GetDeviceProperties().limits.maxImageDimension3D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			imageInfo.imageType = VK_IMAGE_TYPE_3D;
			imageInfo.arrayLayers = 1;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = depth;
			break;
		}

		if (imageInfo.usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
			if (width > m_pDevice->GetDeviceProperties().limits.maxFramebufferWidth || height > m_pDevice->GetDeviceProperties().limits.maxFramebufferHeight) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}

			if (CRendererHelper::vkIsFormatDepthOnly(format) || CRendererHelper::vkIsFormatStencilOnly(format) || CRendererHelper::vkIsFormatDepthStencil(format)) {
				imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			}
			else {
				imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			}
		}

		if (imageInfo.tiling == VK_IMAGE_TILING_LINEAR) {
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
		}

		if (imageInfo.imageType != VK_IMAGE_TYPE_2D || (imageInfo.flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) != 0 || imageInfo.tiling == VK_IMAGE_TILING_LINEAR || imageInfo.mipLevels != 1) {
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		}

		CALL_VK_FUNCTION_RETURN(CheckParameters(imageInfo.imageType, imageInfo.format, imageInfo.extent.width, imageInfo.extent.height, imageInfo.extent.depth, imageInfo.mipLevels, imageInfo.arrayLayers, imageInfo.samples, imageInfo.tiling, imageInfo.usage));
		CALL_VK_FUNCTION_RETURN(vkCreateImage(m_pDevice->GetDevice(), &imageInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

		VkMemoryPropertyFlags memoryPropertyFlags;
		memoryPropertyFlags = imageInfo.tiling == VK_IMAGE_TILING_LINEAR ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
		m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
		m_pMemory->BindImage(m_vkImage);

		m_type = imageInfo.imageType;
		m_format = imageInfo.format;
		m_width = imageInfo.extent.width;
		m_height = imageInfo.extent.height;
		m_depth = imageInfo.extent.depth;
		m_mipLevels = imageInfo.mipLevels;
		m_arrayLayers = imageInfo.arrayLayers;
		m_samples = imageInfo.samples;
		m_tiling = imageInfo.tiling;
		m_size = m_pMemory->GetSize();

		return VK_SUCCESS;
	}

	VkResult CRendererImage::CreateImageView(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t mipLevels)
	{
		VkImageViewCreateInfo viewInfo;
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.pNext = NULL;
		viewInfo.flags = 0;
		viewInfo.image = m_vkImage;
		viewInfo.viewType = viewType;
		viewInfo.format = format;
		viewInfo.components = CRendererHelper::vkGetFormatComponentMapping(format);
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = viewType == VK_IMAGE_VIEW_TYPE_CUBE ? 6 : 1;
		return vkCreateImageView(m_pDevice->GetDevice(), &viewInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView);
	}

	VkResult CRendererImage::CheckParameters(VkImageType type, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage) const
	{
		VkImageFormatProperties formatProperties;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceImageFormatProperties(m_pDevice->GetPhysicalDevice(), format, type, tiling, usage, 0, &formatProperties));

		if (width < 1 || width > formatProperties.maxExtent.width) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (height < 1 || height > formatProperties.maxExtent.height) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (depth < 1 || depth > formatProperties.maxExtent.depth) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (mipLevels < 1 || mipLevels > formatProperties.maxMipLevels) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (arrayLayers < 1 || arrayLayers > formatProperties.maxArrayLayers) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (samples != (samples & formatProperties.sampleCounts)) return VK_ERROR_FORMAT_NOT_SUPPORTED;

		return VK_SUCCESS;
	}

	void CRendererImage::DestroyImage(void)
	{
		if (m_vkImage) {
			vkDestroyImage(m_pDevice->GetDevice(), m_vkImage, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_pMemory) {
			m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
		}

		m_pMemory = NULL;
		m_vkImage = VK_NULL_HANDLE;
	}

	void CRendererImage::DestroyImageView(void)
	{
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkImageView = VK_NULL_HANDLE;
	}

	VkImage CRendererImage::GetImage(void) const
	{
		return m_vkImage;
	}

	VkImageView CRendererImage::GetImageView(void) const
	{
		return m_vkImageView;
	}

	uint32_t CRendererImage::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CRendererImage::GetHeight(void) const
	{
		return m_height;
	}

	uint32_t CRendererImage::GetDepth(void) const
	{
		return m_depth;
	}

	uint32_t CRendererImage::GetMips(void) const
	{
		return m_mipLevels;
	}

	uint32_t CRendererImage::GetLayers(void) const
	{
		return m_arrayLayers;
	}

	VkImageType CRendererImage::GetType(void) const
	{
		return m_type;
	}

	VkFormat CRendererImage::GetFormat(void) const
	{
		return m_format;
	}

	VkSampleCountFlagBits CRendererImage::GetSamples(void) const
	{
		return m_samples;
	}

	VkImageTiling CRendererImage::GetTiling(void) const
	{
		return m_tiling;
	}

	VkDeviceSize CRendererImage::GetSize(void) const
	{
		return m_size;
	}

}
