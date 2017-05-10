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

	class CROSS_EXPORT CVulkanSampler : public CVulkanResource
	{
		friend class CVulkanSamplerManager;


	protected:
		CVulkanSampler(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager);
		virtual ~CVulkanSampler(void);


	public:
		virtual BOOL Create(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);
		virtual void Destroy(void);

	public:
		VkSampler GetSampler(void) const;

	public:
		virtual void DumpLog(void) const;


	protected:
		VkSampler m_vkSampler;

	protected:
		VkFilter m_minFilter;
		VkFilter m_magFilter;
		VkSamplerMipmapMode m_mipmapMode;
		VkSamplerAddressMode m_addressMode;
	};

	class CROSS_EXPORT CVulkanSamplerPtr : public CVulkanResourcePtr<CVulkanSampler>
	{
	public:
		CVulkanSamplerPtr(void) : CVulkanResourcePtr<CVulkanSampler>() {}
		CVulkanSamplerPtr(const CVulkanSampler *p) : CVulkanResourcePtr<CVulkanSampler>(p) {}
		CVulkanSamplerPtr(const CVulkanSamplerPtr &ptr) : CVulkanResourcePtr<CVulkanSampler>(ptr) {}
	};

}
