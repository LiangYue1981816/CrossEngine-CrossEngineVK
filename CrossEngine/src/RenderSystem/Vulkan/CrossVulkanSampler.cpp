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

#include "_CrossEngine.h"


namespace CrossEngine {

	CVulkanSampler::CVulkanSampler(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager)
		: CVulkanResource(pDevice, pResourceManager)
		, m_vkSampler(VK_NULL_HANDLE)

		, m_minFilter(VK_FILTER_NEAREST)
		, m_magFilter(VK_FILTER_NEAREST)
		, m_mipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST)
		, m_addressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
	{

	}

	CVulkanSampler::~CVulkanSampler(void)
	{

	}

	BOOL CVulkanSampler::Create(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		try {
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.magFilter = magFilter;
			createInfo.minFilter = minFilter;
			createInfo.mipmapMode = mipmapMode;
			createInfo.addressModeU = addressMode;
			createInfo.addressModeV = addressMode;
			createInfo.addressModeW = addressMode;
			createInfo.mipLodBias = 0.0f;
			createInfo.anisotropyEnable = VK_FALSE;
			createInfo.maxAnisotropy = 1.0f;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = 0.0f;
			createInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			CALL_VK_FUNCTION_THROW(vkCreateSampler(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanSampler::Destroy(void)
	{
		if (m_vkSampler) {
			vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSampler = VK_NULL_HANDLE;
	}

	VkSampler CVulkanSampler::GetSampler(void) const
	{
		return m_vkSampler;
	}

	void CVulkanSampler::DumpLog(void) const
	{
		if (m_vkSampler) {
			LOGI("\t\tSampler 0x%x: min filter = %s mag filter = %s mipmap mode = %s address mode = %s\n",
				m_vkSampler,
				CVulkanHelper::vkFilterToString(m_minFilter),
				CVulkanHelper::vkFilterToString(m_magFilter),
				CVulkanHelper::vkSamplerMipmapModeToString(m_mipmapMode),
				CVulkanHelper::vkSamplerAddressModeToString(m_addressMode));
		}
	}

}
