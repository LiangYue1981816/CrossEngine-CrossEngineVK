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
		, m_pUniformCamera(NULL)
	{
		m_pUniformCamera = SAFE_NEW CUniformCamera;

		m_ptrDescriptorSetLayout = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_CAMERA);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_CAMERA], DESCRIPTOR_BIND_CAMERA, VK_SHADER_STAGE_ALL);
		m_ptrDescriptorSetLayout->Create();

		m_ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(thread_id(), m_ptrDescriptorSetLayout);
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_CAMERA, m_pUniformCamera->GetUniformBuffer());
		m_ptrDescriptorSet->UpdateDescriptorSets();
	}

	CCamera::~CCamera(void)
	{
		ClearRenderQueue();
		SAFE_DELETE(m_pUniformCamera);
	}

	const RenderQueue& CCamera::GetRenderQueue(void) const
	{
		return m_queue;
	}

	const CGfxDescriptorSetPtr& CCamera::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
	}

	void CCamera::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	BOOL CCamera::IsEnable(void) const
	{
		return m_bEnable;
	}

	void CCamera::SetScissor(float x, float y, float width, float height)
	{
		m_camera.setScissor(x, y, width, height);
	}

	void CCamera::SetViewport(float x, float y, float width, float height)
	{
		m_camera.setViewport(x, y, width, height);
		m_pUniformCamera->SetScreen(width, height);
	}

	void CCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
	{
		m_camera.setPerspective(fovy, aspect, zNear, zFar);
		m_pUniformCamera->SetPerspective(fovy, aspect, zNear, zFar);
	}

	void CCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_camera.setOrtho(left, right, bottom, top, zNear, zFar);
		m_pUniformCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	}

	void CCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
	{
		m_camera.setLookat(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
		m_pUniformCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	}

	void CCamera::Apply(void)
	{
		m_pUniformCamera->Apply();
	}

	const glm::vec4& CCamera::GetScissor(void) const
	{
		return m_camera.scissor;
	}

	const glm::vec4& CCamera::GetViewport(void) const
	{
		return m_camera.viewport;
	}

	const glm::vec3& CCamera::GetPosition(void) const
	{
		return m_camera.position;
	}

	const glm::vec3& CCamera::GetForwardDirection(void) const
	{
		return m_camera.forward;
	}

	const glm::vec3& CCamera::GetUpDirection(void) const
	{
		return m_camera.up;
	}

	const glm::mat4& CCamera::GetProjectionMatrix(void) const
	{
		return m_camera.projectionMatrix;
	}

	const glm::mat4& CCamera::GetViewMatrix(void) const
	{
		return m_camera.viewMatrix;
	}

	const glm::mat4& CCamera::GetViewInverseMatrix(void) const
	{
		return m_camera.viewInverseMatrix;
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

	BOOL CCamera::IsVisible(const glm::sphere &sphere)
	{
		if (m_bEnable) {
			return m_camera.visible(sphere);
		}

		return FALSE;
	}

	void CCamera::ClearRenderQueue(void)
	{
		for (int indexPartical = 0; indexPartical < m_pBatchParticals.size(); indexPartical++) {
			SAFE_DELETE(m_pBatchParticals[indexPartical]);
		}

		for (int indexSkinMesh = 0; indexSkinMesh < m_pBatchSkinMeshs.size(); indexSkinMesh++) {
			SAFE_DELETE(m_pBatchSkinMeshs[indexSkinMesh]);
		}

		for (int indexStaticMesh = 0; indexStaticMesh < m_pBatchStaticMeshs.size(); indexStaticMesh++) {
			SAFE_DELETE(m_pBatchStaticMeshs[indexStaticMesh]);
		}

		m_queue.clear();
		m_pBatchParticals.clear();
		m_pBatchSkinMeshs.clear();
		m_pBatchStaticMeshs.clear();
	}

	void CCamera::AddRenderQueue(const CDrawable *pDrawable)
	{
		if (m_bEnable) {
			for (const auto &itMatPass : pDrawable->GetMaterial()->GetPasses()) {
				const uint32_t indexPass = itMatPass.second->GetIndexSubPass();
				const CGfxRenderPassPtr &ptrRenderPass = itMatPass.second->GetRenderPass();
				const CGfxPipelineGraphicsPtr &ptrMaterialPipeline = itMatPass.second->GetPipeline();
				const CGfxDescriptorSetPtr &ptrMaterialDescriptorSet = itMatPass.second->GetDescriptorSet();
				const CGfxVertexBufferPtr &ptrDrawableVertexBuffer = pDrawable->GetVertexBuffer();
				const CGfxIndexBufferPtr &ptrDrawableIndexBuffer = pDrawable->GetIndexBuffer();
				const CGfxDescriptorSetPtr &ptrDrawableDescriptorSet = pDrawable->GetDescriptorSet(itMatPass.first);
				const uint32_t indexCount = pDrawable->GetIndexCount();
				const uint32_t indexOffset = pDrawable->GetIndexOffset();
				const uint32_t vertexOffset = pDrawable->GetVertexOffset();

				if (m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] == NULL) {
					switch (pDrawable->GetType()) {
					case DRAWABLE_TYPE_PARTICAL:
						m_pBatchParticals.emplace_back(SAFE_NEW CBatchPartical);
						m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchParticals[m_pBatchParticals.size() - 1];
						break;

					case DRAWABLE_TYPE_SKIN_MESH:
						m_pBatchSkinMeshs.emplace_back(SAFE_NEW CBatchSkinMesh);
						m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchSkinMeshs[m_pBatchSkinMeshs.size() - 1];
						break;

					case DRAWABLE_TYPE_STATIC_MESH:
						m_pBatchStaticMeshs.emplace_back(SAFE_NEW CBatchStaticMesh);
						m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet] = m_pBatchStaticMeshs[m_pBatchStaticMeshs.size() - 1];
						break;

					default:
						LOGE("Not support drawable type = %d", pDrawable->GetType());
						return;
					}
				}

				m_queue[ptrRenderPass][indexPass][ptrMaterialPipeline][ptrMaterialDescriptorSet][ptrDrawableVertexBuffer][ptrDrawableIndexBuffer][indexCount][indexOffset][vertexOffset][ptrDrawableDescriptorSet]->AddDrawable(pDrawable, itMatPass.first);
			}
		}
	}

	void CCamera::Update(void)
	{
		if (m_bEnable) {
			for (int indexPartical = 0; indexPartical < m_pBatchParticals.size(); indexPartical++) {
				m_pBatchParticals[indexPartical]->UpdateBatchBuffer();
			}

			for (int indexSkinMesh = 0; indexSkinMesh < m_pBatchSkinMeshs.size(); indexSkinMesh++) {
				m_pBatchSkinMeshs[indexSkinMesh]->UpdateBatchBuffer();
			}

			for (int indexStaticMesh = 0; indexStaticMesh < m_pBatchStaticMeshs.size(); indexStaticMesh++) {
				m_pBatchStaticMeshs[indexStaticMesh]->UpdateBatchBuffer();
			}
		}
	}

}
