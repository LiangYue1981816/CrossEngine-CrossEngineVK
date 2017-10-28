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

	CVulkanImage::CVulkanImage(CVulkanDevice *pDevice)
		: m_pDevice(pDevice)
		, m_pMemory(NULL)
		, m_vkImage(VK_NULL_HANDLE)
		, m_vkImageView(VK_NULL_HANDLE)
		, m_vkSampler(VK_NULL_HANDLE)
		, m_vkDescriptorImageInfo{}

		, m_width(0)
		, m_height(0)
		, m_depth(0)
		, m_mipLevels(0)
		, m_arrayLayers(0)

		, m_format(VK_FORMAT_R8G8B8A8_UNORM)
		, m_type(VK_IMAGE_TYPE_2D)
		, m_tiling(VK_IMAGE_TILING_OPTIMAL)
		, m_samples(VK_SAMPLE_COUNT_1_BIT)

		, m_minFilter(VK_FILTER_NEAREST)
		, m_magFilter(VK_FILTER_LINEAR)
		, m_mipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST)
		, m_addressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
	{

	}

	CVulkanImage::~CVulkanImage(void)
	{

	}

	BOOL CVulkanImage::Create(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		try {
			CALL_VK_FUNCTION_THROW(CreateImage(viewType, format, width, height, depth, mipLevels, arrayLayers, samples, tiling, usage));
			CALL_VK_FUNCTION_THROW(CreateImageView(viewType, format, aspectMask, mipLevels));
			CALL_VK_FUNCTION_THROW(CreateSampler(minFilter, magFilter, mipmapMode, addressMode));

			m_vkDescriptorImageInfo.sampler = m_vkSampler;
			m_vkDescriptorImageInfo.imageView = m_vkImageView;
			m_vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanImage::Destroy(void)
	{
		DestroyImageView();
		DestroyImage();
		DestroySampler();
	}

	int CVulkanImage::Create(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.format = format;
		createInfo.mipLevels = mipLevels;
		createInfo.samples = samples;
		createInfo.tiling = tiling;
		createInfo.usage = usage | VK_IMAGE_USAGE_SAMPLED_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = NULL;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		switch (viewType) {
		case VK_IMAGE_VIEW_TYPE_1D:
		case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
			if (width > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension1D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			createInfo.imageType = VK_IMAGE_TYPE_1D;
			createInfo.extent.width = width;
			createInfo.extent.height = 1;
			createInfo.extent.depth = 1;
			createInfo.arrayLayers = 1;
			break;

		case VK_IMAGE_VIEW_TYPE_2D:
		case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
			if (width > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension2D || height > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension2D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			createInfo.imageType = VK_IMAGE_TYPE_2D;
			createInfo.extent.width = width;
			createInfo.extent.height = height;
			createInfo.extent.depth = 1;
			createInfo.arrayLayers = 1;
			break;

		case VK_IMAGE_VIEW_TYPE_CUBE:
		case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
			if (width > m_pDevice->GetPhysicalDeviceLimits().maxImageDimensionCube || height > m_pDevice->GetPhysicalDeviceLimits().maxImageDimensionCube) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			if (width != height) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			createInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			createInfo.imageType = VK_IMAGE_TYPE_2D;
			createInfo.extent.width = width;
			createInfo.extent.height = height;
			createInfo.extent.depth = 1;
			createInfo.arrayLayers = 6;
			break;

		case VK_IMAGE_VIEW_TYPE_3D:
			if (width > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension3D || height > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension3D || depth > m_pDevice->GetPhysicalDeviceLimits().maxImageDimension3D) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}
			createInfo.imageType = VK_IMAGE_TYPE_3D;
			createInfo.extent.width = width;
			createInfo.extent.height = height;
			createInfo.extent.depth = depth;
			createInfo.arrayLayers = 1;
			break;
		}

		if (createInfo.usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
			if (width > m_pDevice->GetPhysicalDeviceLimits().maxFramebufferWidth || height > m_pDevice->GetPhysicalDeviceLimits().maxFramebufferHeight) {
				return VK_ERROR_VALIDATION_FAILED_EXT;
			}

			if (CVulkanHelper::vkIsFormatDepthOnly(format) || CVulkanHelper::vkIsFormatStencilOnly(format) || CVulkanHelper::vkIsFormatDepthStencil(format)) {
				createInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			}
			else {
				createInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			}
		}

		if (createInfo.tiling == VK_IMAGE_TILING_LINEAR) {
			createInfo.mipLevels = 1;
			createInfo.arrayLayers = 1;
		}

		if (createInfo.imageType != VK_IMAGE_TYPE_2D || (createInfo.flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) != 0 || createInfo.tiling == VK_IMAGE_TILING_LINEAR || createInfo.mipLevels != 1) {
			createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		}

		CALL_VK_FUNCTION_RETURN(CheckParameters(createInfo));
		CALL_VK_FUNCTION_RETURN(vkCreateImage(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkImage));

		VkMemoryPropertyFlags memoryPropertyFlags;
		memoryPropertyFlags = createInfo.tiling == VK_IMAGE_TILING_LINEAR ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(m_pDevice->GetDevice(), m_vkImage, &requirements);
		m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
		m_pMemory->BindImage(m_vkImage);

		m_type = createInfo.imageType;
		m_format = createInfo.format;
		m_width = createInfo.extent.width;
		m_height = createInfo.extent.height;
		m_depth = createInfo.extent.depth;
		m_mipLevels = createInfo.mipLevels;
		m_arrayLayers = createInfo.arrayLayers;
		m_samples = createInfo.samples;
		m_tiling = createInfo.tiling;

		return VK_SUCCESS;
	}

	int CVulkanImage::CreateImageView(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectMask, uint32_t mipLevels)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.image = m_vkImage;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = CVulkanHelper::vkGetFormatComponentMapping(format);
		createInfo.subresourceRange.aspectMask = aspectMask;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = viewType == VK_IMAGE_VIEW_TYPE_CUBE ? 6 : 1;
		return vkCreateImageView(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView);
	}

	int CVulkanImage::CreateSampler(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
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
		createInfo.compareOp = VK_COMPARE_OP_NEVER;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 0.0f;
		createInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		CALL_VK_FUNCTION_RETURN(vkCreateSampler(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

		m_minFilter = minFilter;
		m_magFilter = magFilter;
		m_mipmapMode = mipmapMode;
		m_addressMode = addressMode;

		return VK_SUCCESS;
	}

	int CVulkanImage::CheckParameters(const VkImageCreateInfo &createInfo) const
	{
		VkImageFormatProperties formatProperties;
		CALL_VK_FUNCTION_RETURN(vkGetPhysicalDeviceImageFormatProperties(m_pDevice->GetPhysicalDevice(), createInfo.format, createInfo.imageType, createInfo.tiling, createInfo.usage, 0, &formatProperties));

		if (createInfo.extent.width < 1 || createInfo.extent.width > formatProperties.maxExtent.width) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (createInfo.extent.height < 1 || createInfo.extent.height > formatProperties.maxExtent.height) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (createInfo.extent.depth < 1 || createInfo.extent.depth > formatProperties.maxExtent.depth) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (createInfo.mipLevels < 1 || createInfo.mipLevels > formatProperties.maxMipLevels) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (createInfo.arrayLayers < 1 || createInfo.arrayLayers > formatProperties.maxArrayLayers) return VK_ERROR_FORMAT_NOT_SUPPORTED;
		if (createInfo.samples != (createInfo.samples & formatProperties.sampleCounts)) return VK_ERROR_FORMAT_NOT_SUPPORTED;

		return VK_SUCCESS;
	}

	void CVulkanImage::DestroyImage(void)
	{
		if (m_vkImage) {
			vkDestroyImage(m_pDevice->GetDevice(), m_vkImage, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		if (m_pMemory) {
			m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
		}

		m_pMemory = NULL;
		m_vkImage = VK_NULL_HANDLE;
	}

	void CVulkanImage::DestroyImageView(void)
	{
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkImageView = VK_NULL_HANDLE;
	}

	void CVulkanImage::DestroySampler(void)
	{
		if (m_vkSampler) {
			vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSampler = VK_NULL_HANDLE;
	}

	const VkDescriptorImageInfo& CVulkanImage::GetDescriptorImageInfo(void) const
	{
		return m_vkDescriptorImageInfo;
	}

	VkFormat CVulkanImage::GetFormat(void) const
	{
		return m_format;
	}

	VkImageType CVulkanImage::GetType(void) const
	{
		return m_type;
	}

}
