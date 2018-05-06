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

	class CROSS_EXPORT CVulkanDescriptorPoolManager
	{
		friend class CVulkanDevice;


	protected:
		CVulkanDescriptorPoolManager(CVulkanDevice *pDevice);
		virtual ~CVulkanDescriptorPoolManager(void);


	protected:
		int Create(void);
		void Destroy(void);
		void DumpLog(const char *szTitle) const;

	protected:
		void ResetDescriptorSetPool(uint32_t pool);
		void AllocDescriptorSetPool(uint32_t pool);
		CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, const CGfxDescriptorSetLayoutPtr &ptrDescriptorSetLayout);


	protected:
		pthread_mutex_t m_mutex;
		std::map<uint32_t, CVulkanDescriptorSetPool*> m_pDescriptorPoolListHeads;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
