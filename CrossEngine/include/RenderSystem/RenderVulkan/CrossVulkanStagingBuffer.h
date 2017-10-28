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

	class CROSS_EXPORT CVulkanStagingBuffer
	{
		friend class CVulkanBuffer;
		friend class CVulkanTexture;
		friend class CVulkanStagingBufferManager;


	protected:
		CVulkanStagingBuffer(CVulkanDevice *pDevice, VkCommandPool vkCommandPool, VkDeviceSize size);
		virtual ~CVulkanStagingBuffer(void);

	protected:
		void Release(void);


	protected:
		int TransferImage(VkImage vkImage, uint32_t mipLevels, uint32_t arrayLayers, uint32_t regionCount, const VkBufferImageCopy *pRegions, VkDeviceSize size, const void *pPixels) const;
		int TransferBuffer(VkBuffer vkBuffer, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const;
		int TransferIndexBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const;
		int TransferVertexBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const;
		int TransferUniformBuffer(VkBuffer vkBuffer, VkDeviceSize size, VkDeviceSize offset, const void *pBuffer) const;

	protected:
		size_t GetBufferSize(void) const;
		size_t GetMemorySize(void) const;

	protected:
		void DumpLog(void) const;


	protected:
		CVulkanMemory *m_pMemory;

	protected:
		VkDeviceSize m_size;
		VkBuffer m_vkBuffer;

	protected:
		VkCommandPool m_vkCommandPool;
		VkCommandBuffer m_vkCommandBuffer;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
