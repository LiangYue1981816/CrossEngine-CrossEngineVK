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
		: m_bEnable(TRUE)
		, m_renderer(this)
	{
		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), NULL, TRUE);

		m_ptrDescriptorSetLayout = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_FRAME);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_CAMERA], DESCRIPTOR_BIND_CAMERA, VK_SHADER_STAGE_VERTEX_BIT);
		m_ptrDescriptorSetLayout->Create();

		for (uint32_t frame = 0; frame < GfxSwapChain()->GetImageCount(); frame++) {
			m_ptrDescriptorSets[frame] = GfxDevice()->AllocDescriptorSet(thread_id(), m_ptrDescriptorSetLayout);
			m_ptrDescriptorSets[frame]->SetUniformBuffer(DESCRIPTOR_BIND_CAMERA, m_ptrUniformBuffer);
			m_ptrDescriptorSets[frame]->UpdateDescriptorSets();
		}
	}

	CCamera::~CCamera(void)
	{
		ClearRenderQueue();
	}

	float CCamera::GetViewportX(void) const
	{
		return m_camera.viewport.x;
	}

	float CCamera::GetViewportY(void) const
	{
		return m_camera.viewport.y;
	}

	float CCamera::GetViewportWidth(void) const
	{
		return m_camera.viewport.z;
	}

	float CCamera::GetViewportHeight(void) const
	{
		return m_camera.viewport.w;
	}

	const CGfxDescriptorSetPtr& CCamera::GetDescriptorSet(void) const
	{
		static const CGfxDescriptorSetPtr ptrDescriptorSetNull;
		const auto &itDescriptorSet = m_ptrDescriptorSets.find(GfxSwapChain()->GetImageIndex());
		return itDescriptorSet != m_ptrDescriptorSets.end() ? itDescriptorSet->second : ptrDescriptorSetNull;
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
		static const glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

		m_camera.setPerspective(fovy, aspect, zNear, zFar);
		m_params.mtxProjection = RenderSystem()->GetAPI() == GFX_API_VULKAN ? mtxLH2RH * m_camera.mtxProjection : m_camera.mtxProjection;
		m_ptrDescriptorSets[GfxSwapChain()->GetImageIndex()]->SetUniformBufferData(DESCRIPTOR_BIND_CAMERA, 0, sizeof(m_params), &m_params);
	}

	void CCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		static const glm::mat4 mtxLH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

		m_camera.setOrtho(left, right, bottom, top, zNear, zFar);
		m_params.mtxProjection = RenderSystem()->GetAPI() == GFX_API_VULKAN ? mtxLH2RH * m_camera.mtxProjection : m_camera.mtxProjection;
		m_ptrDescriptorSets[GfxSwapChain()->GetImageIndex()]->SetUniformBufferData(DESCRIPTOR_BIND_CAMERA, 0, sizeof(m_params), &m_params);
	}

	void CCamera::SetLookat(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up)
	{
		m_camera.setLookat(position, position + direction, up);
		m_params.mtxView = m_camera.mtxView;
		m_params.mtxViewInverse = m_camera.mtxViewInverse;
		m_params.mtxViewInverseTranspose = m_camera.mtxViewInverseTranspose;
		m_ptrDescriptorSets[GfxSwapChain()->GetImageIndex()]->SetUniformBufferData(DESCRIPTOR_BIND_CAMERA, 0, sizeof(m_params), &m_params);
	}

	const glm::vec3& CCamera::GetPosition(void) const
	{
		return m_camera.position;
	}

	const glm::vec3& CCamera::GetDirection(void) const
	{
		return m_camera.forward;
	}

	const glm::mat4& CCamera::GetProjectionMatrix(void) const
	{
		return m_camera.mtxProjection;
	}

	const glm::mat4& CCamera::GetWorldToCameraMatrix(void) const
	{
		return m_camera.mtxView;
	}

	const glm::mat4& CCamera::GetCameraToWorldMatrix(void) const
	{
		return m_camera.mtxViewInverse;
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
		if (m_bEnable) {
			return m_camera.visible(vertex);
		}

		return FALSE;
	}

	BOOL CCamera::IsVisible(const glm::aabb &aabb)
	{
		if (m_bEnable) {
			return m_camera.visible(aabb);
		}

		return FALSE;
	}

	void CCamera::ClearRenderQueue(void)
	{
		m_renderer.Clear();
	}

	void CCamera::AddRenderQueue(const CDrawable *pDrawable)
	{
		if (m_bEnable) {
			m_renderer.AddDrawable(pDrawable);
		}
	}

	void CCamera::Update(void)
	{
		if (m_bEnable) {
			m_renderer.UpdateBatchBuffer();
		}
	}

	void CCamera::Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
	{
		if (m_bEnable) {
			m_renderer.BuildCommandBuffer(ptrFrameBuffer, ptrRenderPass);
			m_renderer.Render(ptrFrameBuffer, ptrRenderPass);
		}
	}

}
