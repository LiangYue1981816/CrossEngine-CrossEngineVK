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

	class CROSS_EXPORT CVulkanUniformBuffer : public CVulkanBuffer
	{
		friend class CVulkanBufferManager;


	protected:
		CVulkanUniformBuffer(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager);
		virtual ~CVulkanUniformBuffer(void);


	public:
		virtual BOOL Create(VkDeviceSize size, const void *pBuffer);

	public:
		BOOL SetDescriptorBufferInfo(uint32_t set, uint32_t binding, VkDeviceSize offset, VkDeviceSize size);
		const VkDescriptorBufferInfo& GetDescriptorBufferInfo(uint32_t set, uint32_t binding);


	protected:
		std::map<uint32_t, std::map<uint32_t, VkDescriptorBufferInfo>> m_vkDescriptorBufferInfos;
	};

	class CROSS_EXPORT CVulkanUniformBufferPtr : public CVulkanResourcePtr<CVulkanUniformBuffer>
	{
	public:
		CVulkanUniformBufferPtr(void) : CVulkanResourcePtr<CVulkanUniformBuffer>() {}
		CVulkanUniformBufferPtr(const CVulkanUniformBuffer *p) : CVulkanResourcePtr<CVulkanUniformBuffer>(p) {}
		CVulkanUniformBufferPtr(const CVulkanUniformBufferPtr &ptr) : CVulkanResourcePtr<CVulkanUniformBuffer>(ptr) {}
	};

}
