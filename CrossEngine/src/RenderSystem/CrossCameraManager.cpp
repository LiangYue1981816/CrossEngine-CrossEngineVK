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

	CCameraManager::CCameraManager(void)
	{

	}

	CCameraManager::~CCameraManager(void)
	{
		RemoveCameraAll();
	}

	CCamera* CCameraManager::GetCamera(uint32_t dwName)
	{
		if (m_pCameras[dwName] == NULL) {
			m_pCameras[dwName] = SAFE_NEW CCamera;
		}

		return m_pCameras[dwName];
	}

	void CCameraManager::RemoveCamera(uint32_t dwName)
	{
		if (m_pCameras[dwName] != NULL) {
			SAFE_DELETE(m_pCameras[dwName]);
		}

		m_pCameras.erase(dwName);
	}

	void CCameraManager::RemoveCameraAll(void)
	{
		for (auto &itCamera : m_pCameras) {
			SAFE_DELETE(itCamera.second);
		}

		m_pCameras.clear();
	}

	void CCameraManager::Update(void)
	{
		CBatchPartical::ClearBatchBuffer();
		CBatchSkinMesh::ClearBatchBuffer();
		CBatchStaticMesh::ClearBatchBuffer();
		{
			for (auto &itCamera : m_pCameras) {
				itCamera.second->Update();
			}
		}
		CBatchPartical::CreateBatchBuffer();
		CBatchSkinMesh::CreateBatchBuffer();
		CBatchStaticMesh::CreateBatchBuffer();
	}

	void CCameraManager::Render(uint32_t dwName, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		const auto &itCamera = m_pCameras.find(dwName);
		if (itCamera != m_pCameras.end()) {
			itCamera->second->Render(ptrFrameBuffer, ptrRenderPass);
		}
	}

}
