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

	typedef struct VkAttachmentInformation {
		VkFormat foramt;
		VkImageView vkImageView;
		CGfxRenderTexturePtr ptrRenderTexture;
	} VkAttachmentInformation;

	typedef struct VkSubpassInformation {
		std::map<uint32_t, VkImageLayout> inputAttachments;
		std::map<uint32_t, VkImageLayout> colorAttachments;
		std::map<uint32_t, VkImageLayout> resolveAttachments;
		std::map<uint32_t, uint32_t> preserveAttachments;
		VkAttachmentReference depthStencilAttachment;
	} VkSubpassInformation;


	class CROSS_EXPORT CVulkanAllocator;
	class CROSS_EXPORT CVulkanInstance;
	class CROSS_EXPORT CVulkanQueue;
	class CROSS_EXPORT CVulkanDevice;
	class CROSS_EXPORT CVulkanSwapchain;
	class CROSS_EXPORT CVulkanMemory;
	class CROSS_EXPORT CVulkanMemoryAllocator;
	class CROSS_EXPORT CVulkanMemoryManager;
	class CROSS_EXPORT CVulkanStagingBuffer;
	class CROSS_EXPORT CVulkanStagingBufferManager;
	class CROSS_EXPORT CVulkanCommandBuffer;
	class CROSS_EXPORT CVulkanCommandPool;
	class CROSS_EXPORT CVulkanCommandPoolManager;
	class CROSS_EXPORT CVulkanDescriptorSet;
	class CROSS_EXPORT CVulkanDescriptorSetLayout;
	class CROSS_EXPORT CVulkanDescriptorPool;
	class CROSS_EXPORT CVulkanDescriptorPoolManager;
	class CROSS_EXPORT CVulkanBuffer;
	class CROSS_EXPORT CVulkanIndexBuffer;
	class CROSS_EXPORT CVulkanVertexBuffer;
	class CROSS_EXPORT CVulkanUniformBuffer;
	class CROSS_EXPORT CVulkanBufferManager;
	class CROSS_EXPORT CVulkanImage;
	class CROSS_EXPORT CVulkanTexture;
	class CROSS_EXPORT CVulkanRenderTexture;
	class CROSS_EXPORT CVulkanTextureManager;
	class CROSS_EXPORT CVulkanShader;
	class CROSS_EXPORT CVulkanShaderManager;
	class CROSS_EXPORT CVulkanPipeline;
	class CROSS_EXPORT CVulkanPipelineCompute;
	class CROSS_EXPORT CVulkanPipelineGraphics;
	class CROSS_EXPORT CVulkanPipelineManager;
	class CROSS_EXPORT CVulkanMaterial;
	class CROSS_EXPORT CVulkanMaterialPass;
	class CROSS_EXPORT CVulkanMaterialManager;
	class CROSS_EXPORT CVulkanRenderPass;
	class CROSS_EXPORT CVulkanRenderPassManager;
	class CROSS_EXPORT CVulkanFrameBuffer;
	class CROSS_EXPORT CVulkanFrameBufferManager;

}
