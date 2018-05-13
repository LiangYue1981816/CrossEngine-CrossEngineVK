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

	CGLES3Material::CGLES3Material(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxMaterial(pResourceManager)
		, m_pDevice(pDevice)
	{

	}

	CGLES3Material::~CGLES3Material(void)
	{

	}

	CGLES3Device* CGLES3Material::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3Material::GetHandle(void) const
	{
		return (HANDLE)this;
	}

	void CGLES3Material::Destroy(void)
	{
		m_ptrPasses.clear();
	}

	CGfxMaterialPassPtr& CGLES3Material::AddPass(uint32_t dwName)
	{
		if (m_ptrPasses.find(dwName) == m_ptrPasses.end()) {
			m_ptrPasses[dwName] = CGfxMaterialPassPtr(SAFE_NEW CGLES3MaterialPass(m_pDevice));
		}

		return m_ptrPasses[dwName];
	}

	const CGfxMaterialPassPtr& CGLES3Material::GetPass(uint32_t dwName) const
	{
		static const CGfxMaterialPassPtr ptrMaterialPassNull;
		const auto &itMaterialPass = m_ptrPasses.find(dwName);
		return itMaterialPass != m_ptrPasses.end() ? itMaterialPass->second : ptrMaterialPassNull;
	}

	const std::map<uint32_t, CGfxMaterialPassPtr>& CGLES3Material::GetPasses(void) const
	{
		return m_ptrPasses;
	}

	void CGLES3Material::DumpLog(void) const
	{
		LOGI("\t\tMaterial\n");
		for (const auto &itPass : m_ptrPasses) {
			itPass.second->DumpLog();
		}
	}

}
