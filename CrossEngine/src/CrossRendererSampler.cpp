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

	CRendererSampler::CRendererSampler(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkSampler(VK_NULL_HANDLE)

		, m_minFilter(VK_FILTER_NEAREST)
		, m_magFilter(VK_FILTER_NEAREST)
		, m_mipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST)
		, m_addressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
	{

	}

	CRendererSampler::~CRendererSampler(void)
	{
		ASSERT(m_vkSampler == VK_NULL_HANDLE);
	}

	BOOL CRendererSampler::Create(VkFilter minFilter, VkFilter magFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode)
	{
		try {
			VkSamplerCreateInfo samplerInfo;
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.pNext = NULL;
			samplerInfo.flags = 0;
			samplerInfo.magFilter = magFilter;
			samplerInfo.minFilter = minFilter;
			samplerInfo.mipmapMode = mipmapMode;
			samplerInfo.addressModeU = addressMode;
			samplerInfo.addressModeV = addressMode;
			samplerInfo.addressModeW = addressMode;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			CALL_VK_FUNCTION_THROW(vkCreateSampler(m_pDevice->GetDevice(), &samplerInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererSampler::Destroy(void)
	{
		if (m_vkSampler) {
			vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkSampler = VK_NULL_HANDLE;
	}

	void CRendererSampler::DumpLog(void) const
	{
		if (m_vkSampler) {
			LOGI("\t\tSampler 0x%x: min filter = %s mag filter = %s mipmap mode = %s address mode = %s\n",
				m_vkSampler,
				CRendererHelper::vkFilterToString(m_minFilter),
				CRendererHelper::vkFilterToString(m_magFilter),
				CRendererHelper::vkSamplerMipmapModeToString(m_mipmapMode),
				CRendererHelper::vkSamplerAddressModeToString(m_addressMode));
		}
	}

	VkSampler CRendererSampler::GetSampler(void) const
	{
		return m_vkSampler;
	}

}
