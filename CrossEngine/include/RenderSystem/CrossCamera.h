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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	// [RenderPass][IndexPass][MaterialPipeline][MaterialDescriptorSet][VertexBuffer][IndexBuffer][indexCount][indexOffset][vertexOffset][DrawDescriptorSet] = Batch
	typedef
		std::map<CGfxRenderPassPtr,
		std::map<uint32_t,
		std::map<CGfxPipelineGraphicsPtr,
		std::map<CGfxDescriptorSetPtr,
		std::map<CGfxVertexBufferPtr,
		std::map<CGfxIndexBufferPtr,
		std::map<uint32_t,
		std::map<uint32_t,
		std::map<uint32_t,
		std::map<CGfxDescriptorSetPtr,
		CBatch*>>>>>>>>>> RenderQueue;

	class CROSS_EXPORT CCamera
	{
		friend class CCameraManager;


	protected:
		typedef struct Param {
			glm::mat4 mtxProjection;
			glm::mat4 mtxView;
			glm::mat4 mtxViewInverse;
			glm::mat4 mtxViewInverseTranspose;
		} Param;


	protected:
		CCamera(void);
		virtual ~CCamera(void);


	public:
		const RenderQueue& GetRenderQueue(void) const;
		const CGfxDescriptorSetPtr& GetDescriptorSet(void) const;

	public:
		void SetEnable(BOOL bEnable);
		BOOL IsEnable(void) const;

	public:
		void SetViewport(float x, float y, float width, float height);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetLookat(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up);
		void Apply(void);

		float GetViewportX(void) const;
		float GetViewportY(void) const;
		float GetViewportWidth(void) const;
		float GetViewportHeight(void) const;

		const glm::vec3& GetPosition(void) const;
		const glm::vec3& GetDirection(void) const;

		const glm::mat4& GetProjectionMatrix(void) const;
		const glm::mat4& GetWorldToCameraMatrix(void) const;
		const glm::mat4& GetCameraToWorldMatrix(void) const;

	public:
		glm::vec3 WorldToScreen(const glm::vec3 &world);
		glm::vec3 ScreenToWorld(const glm::vec3 &screen);

	public:
		BOOL IsVisible(const glm::vec3 &vertex);
		BOOL IsVisible(const glm::aabb &aabb);
		BOOL IsVisible(const glm::sphere &sphere);

	public:
		void ClearRenderQueue(void);
		void AddRenderQueue(const CDrawable *pDrawable);

	protected:
		void Update(void);


	protected:
		BOOL m_bEnable;

	protected:
		BOOL m_bDirty;
		Param m_param;
		glm::camera m_camera;

	protected:
		CGfxUniformBufferPtr m_ptrUniformBuffer;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
		CGfxDescriptorSetLayoutPtr m_ptrDescriptorSetLayout;

	protected:
		RenderQueue m_queue;
		std::vector<CBatchPartical*> m_pBatchParticals;
		std::vector<CBatchSkinMesh*> m_pBatchSkinMeshs;
		std::vector<CBatchStaticMesh*> m_pBatchStaticMeshs;
	};

}
