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

	class CROSS_EXPORT CRendererDescriptorPool
	{
		friend class CRendererDescriptorSetManager;


	protected:
		CRendererDescriptorPool(CRendererDevice *pDevice);
		virtual ~CRendererDescriptorPool(void);


	public:
		CRendererDescriptorSet* AllocDescriptorSet(VkDescriptorSetLayout vkSetLayout, const uint32_t *typesUsedCount);
		void FreeDescriptorSet(CRendererDescriptorSet *pDescriptorSet);
		void ResetDescriptorPool(void);

		uint32_t GetDescriptorSetCount(void) const;

	public:
		void DumpLog(void) const;


	protected:
		uint32_t m_numDescriptorSets;
		uint32_t m_maxDescriptorSets;
		uint32_t m_numAllocatedTypes[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
		uint32_t m_maxAllocatedTypes[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

	protected:
		VkDescriptorPool m_vkDescriptorPool;
		std::map<CRendererDescriptorSet*, CRendererDescriptorSet*> m_pDescriptorSets;

	protected:
		CRendererDevice *m_pDevice;
	};

}
