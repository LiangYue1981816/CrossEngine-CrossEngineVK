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

	class CROSS_EXPORT CVulkanDescriptorSetLayout : public CGfxDescriptorSetLayout
	{
		friend class CVulkanDevice;


	protected:
		CVulkanDescriptorSetLayout(CVulkanDevice *pDevice, uint32_t set);
		virtual ~CVulkanDescriptorSetLayout(void);


	protected:
		void Release(void);


	public:
		BOOL Create(void);
		void Destroy(void);

	public:
		BOOL SetUniformBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
		BOOL SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
		BOOL SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);

	public:
		uint32_t GetSet(void) const;
		uint32_t GetBinding(uint32_t dwName) const;

	public:
		VkDescriptorSetLayout GetLayout(void) const;
		const uint32_t* GetTypesUsedCount(void) const;


	protected:
		uint32_t m_numTypesUsedCount[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

	protected:
		uint32_t m_set;
		std::map<uint32_t, uint32_t> m_nameBindings;
		std::map<uint32_t, VkDescriptorSetLayoutBinding> m_layoutBindings;

	protected:
		VkDescriptorSetLayout m_vkDescriptorSetLayout;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
