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
		ClearCamera();
	}

	void CCameraManager::ClearCamera(void)
	{
		for (auto &itCamera : m_pCamerasOrderByID) {
			SAFE_DELETE(itCamera.second);
		}

		m_pCamerasOrderByID.clear();
	}

	CCamera* CCameraManager::GetCamera(uint32_t id)
	{
		if (m_pCamerasOrderByID[id] == NULL) {
			m_pCamerasOrderByID[id] = SAFE_NEW CCamera;
		}

		return m_pCamerasOrderByID[id];
	}

	/*
	void CCameraManager::Render(void) const
	{
		CBatchPartical::ClearBuffer();
		CBatchSkinMesh::ClearBuffer();
		CBatchStaticMesh::ClearBuffer();
		{
			for (const auto &itCamera : m_pCamerasOrderByID) {
				itCamera.second->UpdateBatchBuffer();
			}
		}
		BatchBufferManager()->GetBuffer(CBatchBufferManager::BUFFER_TYPE_PARTICAL, GfxSwapChain()->GetImageIndex())->UpdateBuffer(CBatchPartical::GetBufferSize(), CBatchPartical::GetBuffer());
		BatchBufferManager()->GetBuffer(CBatchBufferManager::BUFFER_TYPE_SKIN_MESH, GfxSwapChain()->GetImageIndex())->UpdateBuffer(CBatchSkinMesh::GetBufferSize(), CBatchSkinMesh::GetBuffer());
		BatchBufferManager()->GetBuffer(CBatchBufferManager::BUFFER_TYPE_STATIC_MESH, GfxSwapChain()->GetImageIndex())->UpdateBuffer(CBatchStaticMesh::GetBufferSize(), CBatchStaticMesh::GetBuffer());

		for (const auto &itCamera : m_pCamerasOrderByID) {
			itCamera.second->Update();
			itCamera.second->Render();
		}
	}
	*/

}
