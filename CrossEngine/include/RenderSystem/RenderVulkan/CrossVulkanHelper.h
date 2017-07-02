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
		static void vkSetupFormat(VkPhysicalDevice vkPhysicalDevice);
		static const VkComponentMapping& vkGetFormatComponentMapping(VkFormat format);

		static VkBool32 vkIsFormatSupported(VkFormat format);
		static VkBool32 vkIsFormatDepthOnly(VkFormat format);
		static VkBool32 vkIsFormatStencilOnly(VkFormat format);
		static VkBool32 vkIsFormatDepthStencil(VkFormat format);

		static VkAccessFlags vkGetAccessMask(VkImageLayout layout);

		static const char* vkFormatToString(VkFormat format);
		static const char* vkImageTypeToString(VkImageType type);
		static const char* vkImageTilingToString(VkImageTiling tiling);
		static const char* vkImageLayoutToString(VkImageLayout layout);
		static const char* vkFilterToString(VkFilter filter);
		static const char* vkSamplerMipmapModeToString(VkSamplerMipmapMode mode);
		static const char* vkSamplerAddressModeToString(VkSamplerAddressMode mode);
		static const char* vkSampleCountFlagBitsToString(VkSampleCountFlagBits sample);
		static const char* vkBufferUsageFlagsToString(VkBufferUsageFlags usage);
		static const char* vkAttachmentLoadOpToString(VkAttachmentLoadOp loadOp);
		static const char* vkAttachmentStoreOpToString(VkAttachmentStoreOp storeOp);
		static const char* vkPipelineStageFlagsToString(VkPipelineStageFlags flags);
		static const char* vkAccessFlagsToString(VkAccessFlags flags);
		static const char* vkDescriptorTypeToString(VkDescriptorType type);

	protected:
		static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
		static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
	};

}
