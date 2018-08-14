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
		eastl::map<CGfxRenderPassPtr,
		eastl::map<uint32_t,
		eastl::map<CGfxPipelineGraphicsPtr,
		eastl::map<CGfxDescriptorSetPtr,
		eastl::map<CGfxVertexBufferPtr,
		eastl::map<CGfxIndexBufferPtr,
		eastl::map<uint32_t,
		eastl::map<uint32_t,
		eastl::map<uint32_t,
		eastl::map<CGfxDescriptorSetPtr,
		CBatch*>>>>>>>>>> RenderQueue;

	class CROSS_EXPORT CCamera
	{
		friend class CCameraManager;


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
		void SetScissor(float x, float y, float width, float height);
		void SetViewport(float x, float y, float width, float height);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
		void Apply(void);

		const glm::vec4& GetScissor(void) const;
		const glm::vec4& GetViewport(void) const;

		const glm::vec3& GetPosition(void) const;
		const glm::vec3& GetForwardDirection(void) const;
		const glm::vec3& GetUpDirection(void) const;

		const glm::mat4& GetProjectionMatrix(void) const;
		const glm::mat4& GetViewMatrix(void) const;
		const glm::mat4& GetViewInverseMatrix(void) const;

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
		glm::camera m_camera;

	protected:
		CUniformCamera *m_pUniformCamera;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
		CGfxDescriptorSetLayoutPtr m_ptrDescriptorSetLayout;

	protected:
		RenderQueue m_queue;
		eastl::vector<CBatchPartical*> m_pBatchParticals;
		eastl::vector<CBatchSkinMesh*> m_pBatchSkinMeshs;
		eastl::vector<CBatchStaticMesh*> m_pBatchStaticMeshs;
	};

}
