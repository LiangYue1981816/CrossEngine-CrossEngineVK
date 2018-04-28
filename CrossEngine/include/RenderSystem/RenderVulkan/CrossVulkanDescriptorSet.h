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

	class CROSS_EXPORT CVulkanDescriptorSet : public CGfxDescriptorSet
	{
		friend class CVulkanDescriptorPool;
		friend class CVulkanDescriptorSetManager;


	protected:
		CVulkanDescriptorSet(CVulkanDescriptorPool *pDescriptorPool, CVulkanDevice *pDevice, VkDescriptorSet vkDescriptorSet, uint32_t set, const uint32_t *typesUsedCount);
		virtual ~CVulkanDescriptorSet(void);


	protected:
		void Release(void);

	public:
		CVulkanDevice* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		void SetTexture(uint32_t binding, const CGfxTexturePtr &ptrTexture);
		void SetRenderTexture(uint32_t binding, const CGfxRenderTexturePtr &ptrRenderTexture);
		void SetUniformBuffer(uint32_t binding, const CGfxUniformBufferPtr &ptrUniformBuffer);
		void SetUniformBufferData(uint32_t binding, uint32_t offset, uint32_t size, const void *pBuffer);
		void UpdateDescriptorSets(void);

	public:
		const uint32_t GetSet(void) const;
		const uint32_t* GetTypesUsedCount(void) const;
		const std::vector<uint32_t> GetUniformBufferOffsets(void) const;


	protected:
		CVulkanDescriptorPool *m_pDescriptorPool;

	protected:
		uint32_t m_set;
		uint32_t m_typesUsedCount[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

	protected:
		BOOL m_bNeedUpdate;
		std::map<uint32_t, CGfxTexturePtr> m_ptrTextures;
		std::map<uint32_t, CGfxRenderTexturePtr> m_ptrRenderTextures;
		std::map<uint32_t, CGfxUniformBufferPtr> m_ptrUniformBuffers;
		std::map<uint32_t, uint32_t> m_uniformBufferOffsets;

	protected:
		VkDescriptorSet m_vkDescriptorSet;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
