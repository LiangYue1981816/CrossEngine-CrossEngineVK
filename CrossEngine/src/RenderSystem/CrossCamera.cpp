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

	CCamera::CCamera(void)
		: m_szName{ 0 }
		, m_bEnable(TRUE)
	{

	}

	CCamera::~CCamera(void)
	{

	}

	void CCamera::SetName(const char *szName)
	{
		strcpy(m_szName, szName);
	}

	const char* CCamera::GetName(void) const
	{
		return m_szName;
	}

	void CCamera::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	BOOL CCamera::IsEnable(void) const
	{
		return m_bEnable;
	}

	void CCamera::SetViewport(float x, float y, float width, float height)
	{
		m_camera.setViewport(x, y, width, height);
	}

	void CCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
	{
		m_camera.setPerspective(fovy, aspect, zNear, zFar);
	}

	void CCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_camera.setOrtho(left, right, bottom, top, zNear, zFar);
	}

	void CCamera::SetLookat(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
	{
		m_camera.setLookat(eye, center, up);
	}

	glm::vec3 CCamera::GetPosition(void) const
	{
		return m_camera.position;
	}

	glm::vec3 CCamera::GetDirection(void) const
	{
		return m_camera.forward;
	}

	glm::mat4 CCamera::GetProjectionMatrix(void) const
	{
		return m_camera.mtxProjection;
	}

	glm::mat4 CCamera::GetCameraToWorldMatrix(void) const
	{
		return m_camera.mtxCameraToWorld;
	}

	glm::mat4 CCamera::GetWorldToCameraMatrix(void) const
	{
		return m_camera.mtxWorldToCamera;
	}

	glm::vec3 CCamera::WorldToScreen(const glm::vec3 &world)
	{
		return m_camera.worldToScreen(world);
	}

	glm::vec3 CCamera::ScreenToWorld(const glm::vec3 &screen)
	{
		return m_camera.screenToWorld(screen);
	}

	BOOL CCamera::IsVisible(const glm::vec3 &vertex)
	{
		return m_camera.visible(vertex) ? TRUE : FALSE;
	}

	BOOL CCamera::IsVisible(const glm::aabb &aabb)
	{
		return m_camera.visible(aabb) ? TRUE : FALSE;
	}

	void CCamera::ClearRenderQueue(void)
	{
		m_renderQueue.Clear();
	}

	void CCamera::AddRenderQueue(const CDrawable *pDrawable)
	{
		m_renderQueue.AddDrawable(pDrawable);
	}

	void CCamera::Update(void)
	{
		m_renderQueue.UpdateBuffer();
	}

	void CCamera::Render(void)
	{
		m_renderQueue.Render();
	}

}
