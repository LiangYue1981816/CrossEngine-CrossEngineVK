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

	class CROSS_EXPORT CVulkanHelper
	{
	public:
		static void SetupFormat(VkPhysicalDevice vkPhysicalDevice);
		static const VkComponentMapping& GetFormatComponentMapping(VkFormat format);

		static VkBool32 IsFormatSupported(VkFormat format);
		static VkBool32 IsFormatDepthOnly(VkFormat format);
		static VkBool32 IsFormatStencilOnly(VkFormat format);
		static VkBool32 IsFormatDepthStencil(VkFormat format);

		static VkAccessFlags GetAccessMask(VkImageLayout layout);
		static VkPipelineStageFlags GetPipelineStageFlags(VkAccessFlags access);

		static const char* FormatToString(VkFormat format);
		static const char* ImageTypeToString(VkImageType type);
		static const char* ImageTilingToString(VkImageTiling tiling);
		static const char* ImageLayoutToString(VkImageLayout layout);
		static const char* FilterToString(VkFilter filter);
		static const char* SamplerMipmapModeToString(VkSamplerMipmapMode mode);
		static const char* SamplerAddressModeToString(VkSamplerAddressMode mode);
		static const char* SampleCountFlagBitsToString(VkSampleCountFlagBits sample);
		static const char* BufferUsageFlagsToString(VkBufferUsageFlags usage, char *szString);
		static const char* AttachmentLoadOpToString(VkAttachmentLoadOp loadOp);
		static const char* AttachmentStoreOpToString(VkAttachmentStoreOp storeOp);
		static const char* PipelineStageFlagsToString(VkPipelineStageFlags flags, char *szString);
		static const char* AccessFlagsToString(VkAccessFlags flags, char *szString);
		static const char* DescriptorTypeToString(VkDescriptorType type);
		static const char* PrimitiveTopologyToString(VkPrimitiveTopology topology);
		static const char* PolygonModeToString(VkPolygonMode mode);
		static const char* CullModeFlagsToString(VkCullModeFlags mode);
		static const char* CompareOpToString(VkCompareOp compareOp);
		static const char* StencilOpToString(VkStencilOp stencilOp);
		static const char* BlendFactorToString(VkBlendFactor factor);
		static const char* BlendOpToString(VkBlendOp blendOp);

		static VkFormat StringToFormat(const char *szString);
		static VkImageType StringToImageType(const char *szString);
		static VkImageTiling StringToImageTiling(const char *szString);
		static VkImageLayout StringToImageLayout(const char *szString);
		static VkFilter StringToFilter(const char *szString);
		static VkSamplerMipmapMode StringToSamplerMipmapMode(const char *szString);
		static VkSamplerAddressMode StringToSamplerAddressMode(const char *szString);
		static VkSampleCountFlagBits StringToSampleCountFlagBits(const char *szString);
		static VkBufferUsageFlags StringToBufferUsageFlags(const char *szString);
		static VkAttachmentLoadOp StringToAttachmentLoadOp(const char *szString);
		static VkAttachmentStoreOp StringToAttachmentStoreOp(const char *szString);
		static VkPipelineStageFlags StringToPipelineStageFlags(const char *szString);
		static VkAccessFlags StringToAccessFlags(const char *szString);
		static VkDescriptorType StringToDescriptorType(const char *szString);
		static VkPrimitiveTopology StringToPrimitiveTopology(const char *szString);
		static VkPolygonMode StringToPolygonMode(const char *szString);
		static VkCullModeFlags StringToCullModeFlags(const char *szString);
		static VkCompareOp StringToCompareOp(const char *szString);
		static VkStencilOp StringToStencilOp(const char *szString);
		static VkBlendFactor StringToBlendFactor(const char *szString);
		static VkBlendOp StringToBlendOp(const char *szString);

	protected:
		static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
		static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
	};

}
