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

	CRendererPipelineManager::CRendererPipelineManager(CRendererDevice *pDevice)
		: CRendererResourceManager(pDevice)
	{

	}

	CRendererPipelineManager::~CRendererPipelineManager(void)
	{

	}

	CRendererPipelineCompute* CRendererPipelineManager::AllocPipelineCompute(void)
	{
		CRendererPipelineCompute *pPipeline = SAFE_NEW CRendererPipelineCompute(m_pDevice, this);
		m_pResources[pPipeline] = pPipeline;

		return pPipeline;
	}

	CRendererPipelineGraphics* CRendererPipelineManager::AllocPipelineGraphics(void)
	{
		CRendererPipelineGraphics *pPipeline = SAFE_NEW CRendererPipelineGraphics(m_pDevice, this);
		m_pResources[pPipeline] = pPipeline;

		return pPipeline;
	}

	void CRendererPipelineManager::DumpLog(const char *szTitle) const
	{
		uint32_t count = 0;

		LOGI("\n");
		LOGI("%s\n", szTitle);
		{
			for (const auto &itResource : m_pResources) {
				if (const CRendererPipeline *pResource = (CRendererPipeline *)itResource.second) {
					pResource->DumpLog();
					count++;
				}
			}
		}
		LOGI("*** %d objects found\n", count);
		LOGI("\n");
	}

}
