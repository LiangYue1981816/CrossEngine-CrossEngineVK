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
	
	class CROSS_EXPORT CVulkanDescriptorPool
	{
		friend class CVulkanDescriptorSet;
		friend class CVulkanDescriptorPoolManager;


	protected:
		CVulkanDescriptorPool(CVulkanDevice *pDevice);
		virtual ~CVulkanDescriptorPool(void);


	protected:
		CVulkanDescriptorSet* AllocDescriptorSet(const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout);
		void FreeDescriptorSet(CVulkanDescriptorSet *pDescriptorSet);
		void ResetDescriptorSetPool(void);

	protected:
		uint32_t GetDescriptorSetCount(void) const;
		VkDescriptorPool GetDescriptorPool(void) const;

	protected:
		void DumpLog(void) const;


	protected:
		uint32_t m_numDescriptorSets;
		uint32_t m_maxDescriptorSets;
		uint32_t m_numAllocatedTypes[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
		uint32_t m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

	protected:
		VkDescriptorPool m_vkDescriptorPool;
		std::map<CVulkanDescriptorSet*, CVulkanDescriptorSet*> m_pDescriptorSets;

	protected:
		CVulkanDevice *m_pDevice;

	protected:
		CVulkanDescriptorPool *pNext;
	};

}
