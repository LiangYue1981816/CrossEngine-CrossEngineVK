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

	CRendererResourceManager::CRendererResourceManager(CRendererDevice *pDevice)
		: m_pDevice(pDevice)
	{

	}

	CRendererResourceManager::~CRendererResourceManager(void)
	{

	}

	BOOL CRendererResourceManager::Create(void)
	{
		return m_pResources.empty() ? TRUE : FALSE;
	}

	void CRendererResourceManager::Destroy(void)
	{
		for (std::map<CRendererResource*, CRendererResource*>::const_iterator itResource = m_pResources.begin(); itResource != m_pResources.end(); ++itResource) {
			if (CRendererResource *pResource = itResource->second) {
				pResource->Destroy();
				SAFE_DELETE(pResource);
			}
		}

		m_pResources.clear();
	}

	void CRendererResourceManager::Free(CRendererResource *pResource)
	{
		if (pResource) {
			std::map<CRendererResource*, CRendererResource*>::const_iterator itResource = m_pResources.find(pResource);
			if (itResource != m_pResources.end()) m_pResources.erase(itResource);

			pResource->Destroy();
			SAFE_DELETE(pResource);
		}
	}

	void CRendererResourceManager::DumpLog(const char *szTitle) const
	{
		LOGI("\n");
		LOGI("%s\n", szTitle);
		uint32_t count = 0;
		for (std::map<CRendererResource*, CRendererResource*>::const_iterator itResource = m_pResources.begin(); itResource != m_pResources.end(); ++itResource) {
			if (const CRendererResource *pResource = itResource->second) {
				pResource->DumpLog();
				count++;
			}
		}
		LOGI("*** %d objects found\n", count);
		LOGI("\n");
	}

}
