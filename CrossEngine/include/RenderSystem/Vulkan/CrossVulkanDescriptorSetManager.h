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

	class CROSS_EXPORT CVulkanDescriptorSetManager
	{
		friend class CVulkanDevice;


	protected:
		CVulkanDescriptorSetManager(CVulkanDevice *pDevice);
		virtual ~CVulkanDescriptorSetManager(void);


	protected:
		BOOL Create(void);
		void Destroy(void);

	public:
		CVulkanDescriptorSet* AllocDescriptorSet(uint32_t pool, VkDescriptorSetLayout vkSetLayout, const uint32_t *typesUsedCount);
		void FreeDescriptorSet(uint32_t pool, CVulkanDescriptorSet *pDescriptorSet);

	public:
		CVulkanDescriptorPool* GetDescriptorPool(uint32_t pool);

	public:
		void DumpLog(const char *szTitle) const;


	protected:
		std::map<uint32_t, CVulkanDescriptorPool*> m_pDescriptorPools;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
