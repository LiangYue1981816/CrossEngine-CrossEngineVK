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
		for (auto &itCamera : m_cameras) {
			SAFE_DELETE(itCamera.second);
		}

		m_cameras.clear();
	}

	BOOL CCameraManager::AddCamera(uint32_t id, const char *szName)
	{
		const auto &itCamera = m_cameras.find(id);
		if (itCamera != m_cameras.end()) return FALSE;

		m_cameras[id] = SAFE_NEW CCamera;
		m_cameras[id]->SetName(szName);

		return TRUE;
	}

	BOOL CCameraManager::RemoveCamera(uint32_t id)
	{
		const auto &itCamera = m_cameras.find(id);
		if (itCamera == m_cameras.end()) return FALSE;

		SAFE_DELETE(itCamera->second);
		m_cameras.erase(itCamera);

		return TRUE;
	}

	CCamera* CCameraManager::GetCamera(uint32_t id) const
	{
		const auto &itCamera = m_cameras.find(id);
		return itCamera != m_cameras.end() ? itCamera->second : NULL;
	}

	void CCameraManager::Render(void) const
	{
		CBatchPartical::ClearBuffer();
		CBatchSkinMesh::ClearBuffer();
		CBatchStaticMesh::ClearBuffer();

		for (const auto &itCamera : m_cameras) {
			itCamera.second->Update();
		}

		for (const auto &itCamera : m_cameras) {
			itCamera.second->Render();
		}
	}

}
