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

	CVulkanMaterial::CVulkanMaterial(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxMaterial(pResourceManager)
		, m_pDevice(pDevice)
	{

	}

	CVulkanMaterial::~CVulkanMaterial(void)
	{

	}

	CVulkanDevice* CVulkanMaterial::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanMaterial::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	void CVulkanMaterial::Destroy(void)
	{
		m_ptrPasses.clear();
	}

	CGfxMaterialPassPtr& CVulkanMaterial::AddPass(uint32_t dwName)
	{
		if (m_ptrPasses.find(dwName) == m_ptrPasses.end()) {
			m_ptrPasses[dwName] = CGfxMaterialPassPtr(SAFE_NEW CVulkanMaterialPass);
		}

		return m_ptrPasses[dwName];
	}

	const CGfxMaterialPassPtr& CVulkanMaterial::GetPass(uint32_t dwName) const
	{
		static const CGfxMaterialPassPtr ptrMaterialPassNull;
		const auto &itMaterialPass = m_ptrPasses.find(dwName);
		return itMaterialPass != m_ptrPasses.end() ? itMaterialPass->second : ptrMaterialPassNull;
	}

	const std::map<uint32_t, CGfxMaterialPassPtr>& CVulkanMaterial::GetPasses(void) const
	{
		return m_ptrPasses;
	}

	void CVulkanMaterial::DumpLog(void) const
	{

	}

}
