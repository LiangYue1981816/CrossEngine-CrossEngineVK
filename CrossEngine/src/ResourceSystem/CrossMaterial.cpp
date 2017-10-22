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

	CMaterial::CMaterial(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CMaterial::~CMaterial(void)
	{

	}

	RESOURCE_TYPE CMaterial::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_MATERIAL;
	}

	void CMaterial::Init(void)
	{
		m_passes.clear();
		CResource::Init();
	}

	void CMaterial::Free(void)
	{
		for (auto &itPass : m_passes) {
			SAFE_DELETE(itPass.second);
		}

		CResource::Free();
	}

	BOOL CMaterial::Load(BOOL bSync)
	{
		TiXmlDocument xmlDoc;

		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize()) == FALSE) {
			return FALSE;
		}

		if (TiXmlNode *pPassNode = xmlDoc.FirstChild("Pass")) {
			do {
				const char *szName = pPassNode->ToElement()->AttributeString("name");

				uint32_t dwName = HashValue(szName);
				m_passes[dwName] = SAFE_NEW CMaterialPass;

				if (m_passes[dwName]->Load(pPassNode, bSync) == FALSE) {
					return FALSE;
				}
			} while (pPassNode = pPassNode->IterateChildren("Pass", pPassNode));
		}

		return TRUE;
	}

	BOOL CMaterial::PostLoad(void)
	{
		for (const auto &itPass : m_passes) {
			if (itPass.second->IsReady() == FALSE) {
				return FALSE;
			}
		}

		for (auto &itPass : m_passes) {
			itPass.second->PostLoad();
		}

		return TRUE;
	}

	const CMaterialPass* CMaterial::GetPass(const char *szName) const
	{
		return GetPass(HashValue(szName));
	}

	const CMaterialPass* CMaterial::GetPass(uint32_t dwName) const
	{
		const auto &itPass = m_passes.find(dwName);
		return itPass != m_passes.end() ? itPass->second : NULL;
	}

}
