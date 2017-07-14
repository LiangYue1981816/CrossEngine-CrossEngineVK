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

	static char* strcat_or(char *dst, char *src, BOOL *or);

	VkFormatProperties CVulkanHelper::vkFormatProperties[VK_FORMAT_RANGE_SIZE];
	VkComponentMapping CVulkanHelper::vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];

	void CVulkanHelper::vkSetupFormat(VkPhysicalDevice vkPhysicalDevice)
	{
		for (uint32_t indexFormat = 0; indexFormat < VK_FORMAT_RANGE_SIZE; indexFormat++) {
			vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, (VkFormat)indexFormat, &vkFormatProperties[indexFormat]);

			vkFormatComponentMapping[indexFormat].r = VK_COMPONENT_SWIZZLE_R;
			vkFormatComponentMapping[indexFormat].g = VK_COMPONENT_SWIZZLE_G;
			vkFormatComponentMapping[indexFormat].b = VK_COMPONENT_SWIZZLE_B;
			vkFormatComponentMapping[indexFormat].a = VK_COMPONENT_SWIZZLE_A;
		}

		vkFormatComponentMapping[VK_FORMAT_B8G8R8A8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_B10G11R11_UFLOAT_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_D24_UNORM_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		vkFormatComponentMapping[VK_FORMAT_D32_SFLOAT_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		vkFormatComponentMapping[VK_FORMAT_D16_UNORM_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		vkFormatComponentMapping[VK_FORMAT_D16_UNORM] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		vkFormatComponentMapping[VK_FORMAT_R32G32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R32G32B32A32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R16G16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16G16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R32_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R32_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R8_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_X8_D24_UNORM_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_D32_SFLOAT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		vkFormatComponentMapping[VK_FORMAT_R16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_A2B10G10R10_UNORM_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R5G6B5_UNORM_PACK16] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R8G8B8A8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R16G16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_R32G32B32_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R8G8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_R32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_BC1_RGB_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_BC2_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_BC3_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_BC4_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_BC5_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_BC6H_UFLOAT_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_BC7_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ASTC_4x4_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ASTC_6x6_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ASTC_8x8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ASTC_10x10_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ASTC_12x12_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		vkFormatComponentMapping[VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
		vkFormatComponentMapping[VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	}

	const VkComponentMapping& CVulkanHelper::vkGetFormatComponentMapping(VkFormat format)
	{
		return vkFormatComponentMapping[format];
	}

	VkBool32 CVulkanHelper::vkIsFormatSupported(VkFormat format)
	{
		return vkFormatProperties[format].bufferFeatures || vkFormatProperties[format].linearTilingFeatures || vkFormatProperties[format].optimalTilingFeatures ? VK_TRUE : VK_FALSE;
	}

	VkBool32 CVulkanHelper::vkIsFormatDepthOnly(VkFormat format)
	{
		switch (format) {
		case VK_FORMAT_D16_UNORM: return VK_TRUE;
		case VK_FORMAT_D32_SFLOAT: return VK_TRUE;
		default: return VK_FALSE;
		}
	}

	VkBool32 CVulkanHelper::vkIsFormatStencilOnly(VkFormat format)
	{
		switch (format) {
		case VK_FORMAT_S8_UINT: return VK_TRUE;
		default: return VK_FALSE;
		}
	}

	VkBool32 CVulkanHelper::vkIsFormatDepthStencil(VkFormat format)
	{
		switch (format) {
		case VK_FORMAT_D16_UNORM_S8_UINT: return VK_TRUE;
		case VK_FORMAT_D24_UNORM_S8_UINT: return VK_TRUE;
		case VK_FORMAT_D32_SFLOAT_S8_UINT: return VK_TRUE;
		default: return VK_FALSE;
		}
	}

	VkAccessFlags CVulkanHelper::vkGetAccessMask(VkImageLayout layout)
	{
		switch (layout) {
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return VK_ACCESS_TRANSFER_READ_BIT;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return VK_ACCESS_TRANSFER_WRITE_BIT;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return VK_ACCESS_MEMORY_READ_BIT;
		default: return 0;
		}
	}

	const char* CVulkanHelper::vkFormatToString(VkFormat format)
	{
		static char szString[_MAX_STRING];

		switch (format) {
		case VK_FORMAT_UNDEFINED: strcpy(szString, "VK_FORMAT_UNDEFINED"); break;
		case VK_FORMAT_R4G4_UNORM_PACK8: strcpy(szString, "VK_FORMAT_R4G4_UNORM_PACK8"); break;
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16: strcpy(szString, "VK_FORMAT_R4G4B4A4_UNORM_PACK16"); break;
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16: strcpy(szString, "VK_FORMAT_B4G4R4A4_UNORM_PACK16"); break;
		case VK_FORMAT_R5G6B5_UNORM_PACK16: strcpy(szString, "VK_FORMAT_R5G6B5_UNORM_PACK16"); break;
		case VK_FORMAT_B5G6R5_UNORM_PACK16: strcpy(szString, "VK_FORMAT_B5G6R5_UNORM_PACK16"); break;
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16: strcpy(szString, "VK_FORMAT_R5G5B5A1_UNORM_PACK16"); break;
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16: strcpy(szString, "VK_FORMAT_B5G5R5A1_UNORM_PACK16"); break;
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: strcpy(szString, "VK_FORMAT_A1R5G5B5_UNORM_PACK16"); break;
		case VK_FORMAT_R8_UNORM: strcpy(szString, "VK_FORMAT_R8_UNORM"); break;
		case VK_FORMAT_R8_SNORM: strcpy(szString, "VK_FORMAT_R8_SNORM"); break;
		case VK_FORMAT_R8_USCALED: strcpy(szString, "VK_FORMAT_R8_USCALED"); break;
		case VK_FORMAT_R8_SSCALED: strcpy(szString, "VK_FORMAT_R8_SSCALED"); break;
		case VK_FORMAT_R8_UINT: strcpy(szString, "VK_FORMAT_R8_UINT"); break;
		case VK_FORMAT_R8_SINT: strcpy(szString, "VK_FORMAT_R8_SINT"); break;
		case VK_FORMAT_R8_SRGB: strcpy(szString, "VK_FORMAT_R8_SRGB"); break;
		case VK_FORMAT_R8G8_UNORM: strcpy(szString, "VK_FORMAT_R8G8_UNORM"); break;
		case VK_FORMAT_R8G8_SNORM: strcpy(szString, "VK_FORMAT_R8G8_SNORM"); break;
		case VK_FORMAT_R8G8_USCALED: strcpy(szString, "VK_FORMAT_R8G8_USCALED"); break;
		case VK_FORMAT_R8G8_SSCALED: strcpy(szString, "VK_FORMAT_R8G8_SSCALED"); break;
		case VK_FORMAT_R8G8_UINT: strcpy(szString, "VK_FORMAT_R8G8_UINT"); break;
		case VK_FORMAT_R8G8_SINT: strcpy(szString, "VK_FORMAT_R8G8_SINT"); break;
		case VK_FORMAT_R8G8_SRGB: strcpy(szString, "VK_FORMAT_R8G8_SRGB"); break;
		case VK_FORMAT_R8G8B8_UNORM: strcpy(szString, "VK_FORMAT_R8G8B8_UNORM"); break;
		case VK_FORMAT_R8G8B8_SNORM: strcpy(szString, "VK_FORMAT_R8G8B8_SNORM"); break;
		case VK_FORMAT_R8G8B8_USCALED: strcpy(szString, "VK_FORMAT_R8G8B8_USCALED"); break;
		case VK_FORMAT_R8G8B8_SSCALED: strcpy(szString, "VK_FORMAT_R8G8B8_SSCALED"); break;
		case VK_FORMAT_R8G8B8_UINT: strcpy(szString, "VK_FORMAT_R8G8B8_UINT"); break;
		case VK_FORMAT_R8G8B8_SINT: strcpy(szString, "VK_FORMAT_R8G8B8_SINT"); break;
		case VK_FORMAT_R8G8B8_SRGB: strcpy(szString, "VK_FORMAT_R8G8B8_SRGB"); break;
		case VK_FORMAT_B8G8R8_UNORM: strcpy(szString, "VK_FORMAT_B8G8R8_UNORM"); break;
		case VK_FORMAT_B8G8R8_SNORM: strcpy(szString, "VK_FORMAT_B8G8R8_SNORM"); break;
		case VK_FORMAT_B8G8R8_USCALED: strcpy(szString, "VK_FORMAT_B8G8R8_USCALED"); break;
		case VK_FORMAT_B8G8R8_SSCALED: strcpy(szString, "VK_FORMAT_B8G8R8_SSCALED"); break;
		case VK_FORMAT_B8G8R8_UINT: strcpy(szString, "VK_FORMAT_B8G8R8_UINT"); break;
		case VK_FORMAT_B8G8R8_SINT: strcpy(szString, "VK_FORMAT_B8G8R8_SINT"); break;
		case VK_FORMAT_B8G8R8_SRGB: strcpy(szString, "VK_FORMAT_B8G8R8_SRGB"); break;
		case VK_FORMAT_R8G8B8A8_UNORM: strcpy(szString, "VK_FORMAT_R8G8B8A8_UNORM"); break;
		case VK_FORMAT_R8G8B8A8_SNORM: strcpy(szString, "VK_FORMAT_R8G8B8A8_SNORM"); break;
		case VK_FORMAT_R8G8B8A8_USCALED: strcpy(szString, "VK_FORMAT_R8G8B8A8_USCALED"); break;
		case VK_FORMAT_R8G8B8A8_SSCALED: strcpy(szString, "VK_FORMAT_R8G8B8A8_SSCALED"); break;
		case VK_FORMAT_R8G8B8A8_UINT: strcpy(szString, "VK_FORMAT_R8G8B8A8_UINT"); break;
		case VK_FORMAT_R8G8B8A8_SINT: strcpy(szString, "VK_FORMAT_R8G8B8A8_SINT"); break;
		case VK_FORMAT_R8G8B8A8_SRGB: strcpy(szString, "VK_FORMAT_R8G8B8A8_SRGB"); break;
		case VK_FORMAT_B8G8R8A8_UNORM: strcpy(szString, "VK_FORMAT_B8G8R8A8_UNORM"); break;
		case VK_FORMAT_B8G8R8A8_SNORM: strcpy(szString, "VK_FORMAT_B8G8R8A8_SNORM"); break;
		case VK_FORMAT_B8G8R8A8_USCALED: strcpy(szString, "VK_FORMAT_B8G8R8A8_USCALED"); break;
		case VK_FORMAT_B8G8R8A8_SSCALED: strcpy(szString, "VK_FORMAT_B8G8R8A8_SSCALED"); break;
		case VK_FORMAT_B8G8R8A8_UINT: strcpy(szString, "VK_FORMAT_B8G8R8A8_UINT"); break;
		case VK_FORMAT_B8G8R8A8_SINT: strcpy(szString, "VK_FORMAT_B8G8R8A8_SINT"); break;
		case VK_FORMAT_B8G8R8A8_SRGB: strcpy(szString, "VK_FORMAT_B8G8R8A8_SRGB"); break;
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_UNORM_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_SNORM_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_USCALED_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_SSCALED_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_UINT_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_UINT_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_SINT_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_SINT_PACK32"); break;
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32: strcpy(szString, "VK_FORMAT_A8B8G8R8_SRGB_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_UNORM_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_SNORM_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_USCALED_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_SSCALED_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_UINT_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_UINT_PACK32"); break;
		case VK_FORMAT_A2R10G10B10_SINT_PACK32: strcpy(szString, "VK_FORMAT_A2R10G10B10_SINT_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_UNORM_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_SNORM_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_USCALED_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_SSCALED_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_UINT_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_UINT_PACK32"); break;
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: strcpy(szString, "VK_FORMAT_A2B10G10R10_SINT_PACK32"); break;
		case VK_FORMAT_R16_UNORM: strcpy(szString, "VK_FORMAT_R16_UNORM"); break;
		case VK_FORMAT_R16_SNORM: strcpy(szString, "VK_FORMAT_R16_SNORM"); break;
		case VK_FORMAT_R16_USCALED: strcpy(szString, "VK_FORMAT_R16_USCALED"); break;
		case VK_FORMAT_R16_SSCALED: strcpy(szString, "VK_FORMAT_R16_SSCALED"); break;
		case VK_FORMAT_R16_UINT: strcpy(szString, "VK_FORMAT_R16_UINT"); break;
		case VK_FORMAT_R16_SINT: strcpy(szString, "VK_FORMAT_R16_SINT"); break;
		case VK_FORMAT_R16_SFLOAT: strcpy(szString, "VK_FORMAT_R16_SFLOAT"); break;
		case VK_FORMAT_R16G16_UNORM: strcpy(szString, "VK_FORMAT_R16G16_UNORM"); break;
		case VK_FORMAT_R16G16_SNORM: strcpy(szString, "VK_FORMAT_R16G16_SNORM"); break;
		case VK_FORMAT_R16G16_USCALED: strcpy(szString, "VK_FORMAT_R16G16_USCALED"); break;
		case VK_FORMAT_R16G16_SSCALED: strcpy(szString, "VK_FORMAT_R16G16_SSCALED"); break;
		case VK_FORMAT_R16G16_UINT: strcpy(szString, "VK_FORMAT_R16G16_UINT"); break;
		case VK_FORMAT_R16G16_SINT: strcpy(szString, "VK_FORMAT_R16G16_SINT"); break;
		case VK_FORMAT_R16G16_SFLOAT: strcpy(szString, "VK_FORMAT_R16G16_SFLOAT"); break;
		case VK_FORMAT_R16G16B16_UNORM: strcpy(szString, "VK_FORMAT_R16G16B16_UNORM"); break;
		case VK_FORMAT_R16G16B16_SNORM: strcpy(szString, "VK_FORMAT_R16G16B16_SNORM"); break;
		case VK_FORMAT_R16G16B16_USCALED: strcpy(szString, "VK_FORMAT_R16G16B16_USCALED"); break;
		case VK_FORMAT_R16G16B16_SSCALED: strcpy(szString, "VK_FORMAT_R16G16B16_SSCALED"); break;
		case VK_FORMAT_R16G16B16_UINT: strcpy(szString, "VK_FORMAT_R16G16B16_UINT"); break;
		case VK_FORMAT_R16G16B16_SINT: strcpy(szString, "VK_FORMAT_R16G16B16_SINT"); break;
		case VK_FORMAT_R16G16B16_SFLOAT: strcpy(szString, "VK_FORMAT_R16G16B16_SFLOAT"); break;
		case VK_FORMAT_R16G16B16A16_UNORM: strcpy(szString, "VK_FORMAT_R16G16B16A16_UNORM"); break;
		case VK_FORMAT_R16G16B16A16_SNORM: strcpy(szString, "VK_FORMAT_R16G16B16A16_SNORM"); break;
		case VK_FORMAT_R16G16B16A16_USCALED: strcpy(szString, "VK_FORMAT_R16G16B16A16_USCALED"); break;
		case VK_FORMAT_R16G16B16A16_SSCALED: strcpy(szString, "VK_FORMAT_R16G16B16A16_SSCALED"); break;
		case VK_FORMAT_R16G16B16A16_UINT: strcpy(szString, "VK_FORMAT_R16G16B16A16_UINT"); break;
		case VK_FORMAT_R16G16B16A16_SINT: strcpy(szString, "VK_FORMAT_R16G16B16A16_SINT"); break;
		case VK_FORMAT_R16G16B16A16_SFLOAT: strcpy(szString, "VK_FORMAT_R16G16B16A16_SFLOAT"); break;
		case VK_FORMAT_R32_UINT: strcpy(szString, "VK_FORMAT_R32_UINT"); break;
		case VK_FORMAT_R32_SINT: strcpy(szString, "VK_FORMAT_R32_SINT"); break;
		case VK_FORMAT_R32_SFLOAT: strcpy(szString, "VK_FORMAT_R32_SFLOAT"); break;
		case VK_FORMAT_R32G32_UINT: strcpy(szString, "VK_FORMAT_R32G32_UINT"); break;
		case VK_FORMAT_R32G32_SINT: strcpy(szString, "VK_FORMAT_R32G32_SINT"); break;
		case VK_FORMAT_R32G32_SFLOAT: strcpy(szString, "VK_FORMAT_R32G32_SFLOAT"); break;
		case VK_FORMAT_R32G32B32_UINT: strcpy(szString, "VK_FORMAT_R32G32B32_UINT"); break;
		case VK_FORMAT_R32G32B32_SINT: strcpy(szString, "VK_FORMAT_R32G32B32_SINT"); break;
		case VK_FORMAT_R32G32B32_SFLOAT: strcpy(szString, "VK_FORMAT_R32G32B32_SFLOAT"); break;
		case VK_FORMAT_R32G32B32A32_UINT: strcpy(szString, "VK_FORMAT_R32G32B32A32_UINT"); break;
		case VK_FORMAT_R32G32B32A32_SINT: strcpy(szString, "VK_FORMAT_R32G32B32A32_SINT"); break;
		case VK_FORMAT_R32G32B32A32_SFLOAT: strcpy(szString, "VK_FORMAT_R32G32B32A32_SFLOAT"); break;
		case VK_FORMAT_R64_UINT: strcpy(szString, "VK_FORMAT_R64_UINT"); break;
		case VK_FORMAT_R64_SINT: strcpy(szString, "VK_FORMAT_R64_SINT"); break;
		case VK_FORMAT_R64_SFLOAT: strcpy(szString, "VK_FORMAT_R64_SFLOAT"); break;
		case VK_FORMAT_R64G64_UINT: strcpy(szString, "VK_FORMAT_R64G64_UINT"); break;
		case VK_FORMAT_R64G64_SINT: strcpy(szString, "VK_FORMAT_R64G64_SINT"); break;
		case VK_FORMAT_R64G64_SFLOAT: strcpy(szString, "VK_FORMAT_R64G64_SFLOAT"); break;
		case VK_FORMAT_R64G64B64_UINT: strcpy(szString, "VK_FORMAT_R64G64B64_UINT"); break;
		case VK_FORMAT_R64G64B64_SINT: strcpy(szString, "VK_FORMAT_R64G64B64_SINT"); break;
		case VK_FORMAT_R64G64B64_SFLOAT: strcpy(szString, "VK_FORMAT_R64G64B64_SFLOAT"); break;
		case VK_FORMAT_R64G64B64A64_UINT: strcpy(szString, "VK_FORMAT_R64G64B64A64_UINT"); break;
		case VK_FORMAT_R64G64B64A64_SINT: strcpy(szString, "VK_FORMAT_R64G64B64A64_SINT"); break;
		case VK_FORMAT_R64G64B64A64_SFLOAT: strcpy(szString, "VK_FORMAT_R64G64B64A64_SFLOAT"); break;
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32: strcpy(szString, "VK_FORMAT_B10G11R11_UFLOAT_PACK32"); break;
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: strcpy(szString, "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32"); break;
		case VK_FORMAT_D16_UNORM: strcpy(szString, "VK_FORMAT_D16_UNORM"); break;
		case VK_FORMAT_X8_D24_UNORM_PACK32: strcpy(szString, "VK_FORMAT_X8_D24_UNORM_PACK32"); break;
		case VK_FORMAT_D32_SFLOAT: strcpy(szString, "VK_FORMAT_D32_SFLOAT"); break;
		case VK_FORMAT_S8_UINT: strcpy(szString, "VK_FORMAT_S8_UINT"); break;
		case VK_FORMAT_D16_UNORM_S8_UINT: strcpy(szString, "VK_FORMAT_D16_UNORM_S8_UINT"); break;
		case VK_FORMAT_D24_UNORM_S8_UINT: strcpy(szString, "VK_FORMAT_D24_UNORM_S8_UINT"); break;
		case VK_FORMAT_D32_SFLOAT_S8_UINT: strcpy(szString, "VK_FORMAT_D32_SFLOAT_S8_UINT"); break;
		case VK_FORMAT_BC1_RGB_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC1_RGB_UNORM_BLOCK"); break;
		case VK_FORMAT_BC1_RGB_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_BC1_RGB_SRGB_BLOCK"); break;
		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC1_RGBA_UNORM_BLOCK"); break;
		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_BC1_RGBA_SRGB_BLOCK"); break;
		case VK_FORMAT_BC2_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC2_UNORM_BLOCK"); break;
		case VK_FORMAT_BC2_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_BC2_SRGB_BLOCK"); break;
		case VK_FORMAT_BC3_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC3_UNORM_BLOCK"); break;
		case VK_FORMAT_BC3_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_BC3_SRGB_BLOCK"); break;
		case VK_FORMAT_BC4_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC4_UNORM_BLOCK"); break;
		case VK_FORMAT_BC4_SNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC4_SNORM_BLOCK"); break;
		case VK_FORMAT_BC5_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC5_UNORM_BLOCK"); break;
		case VK_FORMAT_BC5_SNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC5_SNORM_BLOCK"); break;
		case VK_FORMAT_BC6H_UFLOAT_BLOCK: strcpy(szString, "VK_FORMAT_BC6H_UFLOAT_BLOCK"); break;
		case VK_FORMAT_BC6H_SFLOAT_BLOCK: strcpy(szString, "VK_FORMAT_BC6H_SFLOAT_BLOCK"); break;
		case VK_FORMAT_BC7_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_BC7_UNORM_BLOCK"); break;
		case VK_FORMAT_BC7_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_BC7_SRGB_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK"); break;
		case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK"); break;
		case VK_FORMAT_EAC_R11_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_EAC_R11_UNORM_BLOCK"); break;
		case VK_FORMAT_EAC_R11_SNORM_BLOCK: strcpy(szString, "VK_FORMAT_EAC_R11_SNORM_BLOCK"); break;
		case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_EAC_R11G11_UNORM_BLOCK"); break;
		case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: strcpy(szString, "VK_FORMAT_EAC_R11G11_SNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_4x4_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_4x4_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_5x4_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_5x4_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_5x5_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_5x5_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_6x5_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_6x5_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_6x6_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_6x6_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x5_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x5_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x6_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x6_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x8_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_8x8_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x5_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x5_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x6_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x6_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x8_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x8_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x10_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_10x10_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_12x10_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_12x10_SRGB_BLOCK"); break;
		case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_12x12_UNORM_BLOCK"); break;
		case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: strcpy(szString, "VK_FORMAT_ASTC_12x12_SRGB_BLOCK"); break;
		case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG"); break;
		case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: strcpy(szString, "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkImageTypeToString(VkImageType type)
	{
		static char szString[_MAX_STRING];

		switch (type) {
		case VK_IMAGE_TYPE_1D: strcpy(szString, "VK_IMAGE_TYPE_1D"); break;
		case VK_IMAGE_TYPE_2D: strcpy(szString, "VK_IMAGE_TYPE_2D"); break;
		case VK_IMAGE_TYPE_3D: strcpy(szString, "VK_IMAGE_TYPE_3D"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkImageTilingToString(VkImageTiling tiling)
	{
		static char szString[_MAX_STRING];

		switch (tiling) {
		case VK_IMAGE_TILING_OPTIMAL: strcpy(szString, "VK_IMAGE_TILING_OPTIMAL"); break;
		case VK_IMAGE_TILING_LINEAR: strcpy(szString, "VK_IMAGE_TILING_LINEAR"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkImageLayoutToString(VkImageLayout layout)
	{
		static char szString[_MAX_STRING];

		switch (layout) {
		case VK_IMAGE_LAYOUT_UNDEFINED: strcpy(szString, "VK_IMAGE_LAYOUT_UNDEFINED"); break;
		case VK_IMAGE_LAYOUT_GENERAL: strcpy(szString, "VK_IMAGE_LAYOUT_GENERAL"); break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: strcpy(szString, "VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL"); break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED: strcpy(szString, "VK_IMAGE_LAYOUT_PREINITIALIZED"); break;
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: strcpy(szString, "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkFilterToString(VkFilter filter)
	{
		static char szString[_MAX_STRING];

		switch (filter) {
		case VK_FILTER_NEAREST: strcpy(szString, "VK_FILTER_NEAREST"); break;
		case VK_FILTER_LINEAR: strcpy(szString, "VK_FILTER_LINEAR"); break;
		case VK_FILTER_CUBIC_IMG: strcpy(szString, "VK_FILTER_CUBIC_IMG"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkSamplerMipmapModeToString(VkSamplerMipmapMode mode)
	{
		static char szString[_MAX_STRING];

		switch (mode) {
		case VK_SAMPLER_MIPMAP_MODE_NEAREST: strcpy(szString, "VK_SAMPLER_MIPMAP_MODE_NEAREST"); break;
		case VK_SAMPLER_MIPMAP_MODE_LINEAR: strcpy(szString, "VK_SAMPLER_MIPMAP_MODE_LINEAR"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkSamplerAddressModeToString(VkSamplerAddressMode mode)
	{
		static char szString[_MAX_STRING];

		switch (mode) {
		case VK_SAMPLER_ADDRESS_MODE_REPEAT: strcpy(szString, "VK_SAMPLER_ADDRESS_MODE_REPEAT"); break;
		case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: strcpy(szString, "VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT"); break;
		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: strcpy(szString, "VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE"); break;
		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: strcpy(szString, "VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER"); break;
		case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: strcpy(szString, "VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkSampleCountFlagBitsToString(VkSampleCountFlagBits sample)
	{
		static char szString[_MAX_STRING];

		switch (sample) {
		case VK_SAMPLE_COUNT_1_BIT: strcpy(szString, "VK_SAMPLE_COUNT_1_BIT"); break;
		case VK_SAMPLE_COUNT_2_BIT: strcpy(szString, "VK_SAMPLE_COUNT_2_BIT"); break;
		case VK_SAMPLE_COUNT_4_BIT: strcpy(szString, "VK_SAMPLE_COUNT_4_BIT"); break;
		case VK_SAMPLE_COUNT_8_BIT: strcpy(szString, "VK_SAMPLE_COUNT_8_BIT"); break;
		case VK_SAMPLE_COUNT_16_BIT: strcpy(szString, "VK_SAMPLE_COUNT_16_BIT"); break;
		case VK_SAMPLE_COUNT_32_BIT: strcpy(szString, "VK_SAMPLE_COUNT_32_BIT"); break;
		case VK_SAMPLE_COUNT_64_BIT: strcpy(szString, "VK_SAMPLE_COUNT_64_BIT"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkBufferUsageFlagsToString(VkBufferUsageFlags usage)
	{
		static char szString[_MAX_STRING];
		BOOL or = FALSE;

		szString[0] = 0;
		if (usage&VK_BUFFER_USAGE_TRANSFER_SRC_BIT) strcat_or(szString, "VK_BUFFER_USAGE_TRANSFER_SRC_BIT", &or);
		if (usage&VK_BUFFER_USAGE_TRANSFER_DST_BIT) strcat_or(szString, "VK_BUFFER_USAGE_TRANSFER_DST_BIT", &or);
		if (usage&VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_STORAGE_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_INDEX_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_INDEX_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_VERTEX_BUFFER_BIT", &or);
		if (usage&VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT) strcat_or(szString, "VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT", &or);
		if (or == FALSE) strcpy(szString, "unknown");

		return szString;
	}

	const char* CVulkanHelper::vkAttachmentLoadOpToString(VkAttachmentLoadOp loadOp)
	{
		static char szString[_MAX_STRING];

		switch (loadOp) {
		case VK_ATTACHMENT_LOAD_OP_LOAD: strcpy(szString, "VK_ATTACHMENT_LOAD_OP_LOAD"); break;
		case VK_ATTACHMENT_LOAD_OP_CLEAR: strcpy(szString, "VK_ATTACHMENT_LOAD_OP_CLEAR"); break;
		case VK_ATTACHMENT_LOAD_OP_DONT_CARE: strcpy(szString, "VK_ATTACHMENT_LOAD_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkAttachmentStoreOpToString(VkAttachmentStoreOp storeOp)
	{
		static char szString[_MAX_STRING];

		switch (storeOp) {
		case VK_ATTACHMENT_STORE_OP_STORE: strcpy(szString, "VK_ATTACHMENT_STORE_OP_STORE"); break;
		case VK_ATTACHMENT_STORE_OP_DONT_CARE: strcpy(szString, "VK_ATTACHMENT_STORE_OP_DONT_CARE"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	const char* CVulkanHelper::vkPipelineStageFlagsToString(VkPipelineStageFlags flags)
	{
		static char szString[_MAX_STRING];
		BOOL or = FALSE;

		szString[0] = 0;
		if (flags&VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_VERTEX_INPUT_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_VERTEX_INPUT_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_VERTEX_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_VERTEX_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_TRANSFER_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_TRANSFER_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_HOST_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_HOST_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT", &or);
		if (flags&VK_PIPELINE_STAGE_ALL_COMMANDS_BIT) strcat_or(szString, "VK_PIPELINE_STAGE_ALL_COMMANDS_BIT", &or);
		if (or == FALSE) strcpy(szString, "unknown");

		return szString;
	}

	const char* CVulkanHelper::vkAccessFlagsToString(VkAccessFlags flags)
	{
		static char szString[_MAX_STRING];
		BOOL or = FALSE;

		szString[0] = 0;
		if (flags&VK_ACCESS_INDIRECT_COMMAND_READ_BIT) strcat_or(szString, "VK_ACCESS_INDIRECT_COMMAND_READ_BIT", &or);
		if (flags&VK_ACCESS_INDEX_READ_BIT) strcat_or(szString, "VK_ACCESS_INDEX_READ_BIT", &or);
		if (flags&VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT) strcat_or(szString, "VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT", &or);
		if (flags&VK_ACCESS_UNIFORM_READ_BIT) strcat_or(szString, "VK_ACCESS_UNIFORM_READ_BIT", &or);
		if (flags&VK_ACCESS_INPUT_ATTACHMENT_READ_BIT) strcat_or(szString, "VK_ACCESS_INPUT_ATTACHMENT_READ_BIT", &or);
		if (flags&VK_ACCESS_SHADER_READ_BIT) strcat_or(szString, "VK_ACCESS_SHADER_READ_BIT", &or);
		if (flags&VK_ACCESS_SHADER_WRITE_BIT) strcat_or(szString, "VK_ACCESS_SHADER_WRITE_BIT", &or);
		if (flags&VK_ACCESS_COLOR_ATTACHMENT_READ_BIT) strcat_or(szString, "VK_ACCESS_COLOR_ATTACHMENT_READ_BIT", &or);
		if (flags&VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT) strcat_or(szString, "VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT", &or);
		if (flags&VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT) strcat_or(szString, "VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT", &or);
		if (flags&VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT) strcat_or(szString, "VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT", &or);
		if (flags&VK_ACCESS_TRANSFER_READ_BIT) strcat_or(szString, "VK_ACCESS_TRANSFER_READ_BIT", &or);
		if (flags&VK_ACCESS_TRANSFER_WRITE_BIT) strcat_or(szString, "VK_ACCESS_TRANSFER_WRITE_BIT", &or);
		if (flags&VK_ACCESS_HOST_READ_BIT) strcat_or(szString, "VK_ACCESS_HOST_READ_BIT", &or);
		if (flags&VK_ACCESS_HOST_WRITE_BIT) strcat_or(szString, "VK_ACCESS_HOST_WRITE_BIT", &or);
		if (flags&VK_ACCESS_MEMORY_READ_BIT) strcat_or(szString, "VK_ACCESS_MEMORY_READ_BIT", &or);
		if (flags&VK_ACCESS_MEMORY_WRITE_BIT) strcat_or(szString, "VK_ACCESS_MEMORY_WRITE_BIT", &or);
		if (or == FALSE) strcpy(szString, "unknown");

		return szString;
	}

	const char* CVulkanHelper::vkDescriptorTypeToString(VkDescriptorType type)
	{
		static char szString[_MAX_STRING];

		switch (type)
		{
		case VK_DESCRIPTOR_TYPE_SAMPLER: strcpy(szString, "VK_DESCRIPTOR_TYPE_SAMPLER"); break;
		case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: strcpy(szString, "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"); break;
		case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE: strcpy(szString, "VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE"); break;
		case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: strcpy(szString, "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"); break;
		case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: strcpy(szString, "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"); break;
		case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: strcpy(szString, "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"); break;
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: strcpy(szString, "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"); break;
		case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: strcpy(szString, "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"); break;
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: strcpy(szString, "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC"); break;
		case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: strcpy(szString, "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC"); break;
		case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: strcpy(szString, "VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT"); break;
		default: strcpy(szString, "unknown"); break;
		}

		return szString;
	}

	static char* strcat_or(char *dst, char *src, BOOL *or)
	{
		if (*or == TRUE) {
			strcat(dst, "|");
		}

		*or = TRUE;
		return strcat(dst, src);
	}

}
