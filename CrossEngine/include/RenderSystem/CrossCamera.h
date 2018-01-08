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

	class CROSS_EXPORT CCamera
	{
		friend class CCameraManager;


	protected:
		CCamera(void);
		virtual ~CCamera(void);


	public:
		void SetEnable(BOOL bEnable);
		BOOL IsEnable(void) const;

	public:
		void SetViewport(float x, float y, float width, float height);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetLookat(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up);

		const glm::vec3& GetPosition(void) const;
		const glm::vec3& GetDirection(void) const;

		const glm::mat4& GetProjectionMatrix(void) const;
		const glm::mat4& GetCameraToWorldMatrix(void) const;
		const glm::mat4& GetWorldToCameraMatrix(void) const;

	public:
		glm::vec3 WorldToScreen(const glm::vec3 &world);
		glm::vec3 ScreenToWorld(const glm::vec3 &screen);

	public:
		BOOL IsVisible(const glm::vec3 &vertex);
		BOOL IsVisible(const glm::aabb &aabb);

	public:
		BOOL SetFrameBuffer(const CGfxFrameBufferPtr &ptrFrameBuffer);

		BOOL AddRenderPass(uint32_t id, const CGfxRenderPassPtr &ptrRenderPass);
		BOOL RemoveRenderPass(uint32_t id);

	public:
		void ClearRenderQueue(uint32_t indexRenderQueue);
		void AddRenderQueue(uint32_t indexRenderQueue, const CDrawable *pDrawable);

	public:
		void Update(void);
		void UpdateBatchBuffer(uint32_t indexRenderQueue);
		void Render(uint32_t indexRenderQueue, int indexSwapchainImage);


	protected:
		BOOL m_bEnable;

	protected:
		glm::camera m_camera;
		glm::mat4 m_mtxProjection;
		glm::mat4 m_mtxCameraToWorld;
		glm::mat4 m_mtxWorldToCamera;

	protected:
		CRenderQueue m_renderQueue[2];
		std::map<uint32_t, CGfxCommandBufferPtr> m_ptrCommandBuffers[CGfxSwapchain::SWAPCHAIN_IMAGE_COUNT];

	protected:
		CGfxFrameBufferPtr m_ptrFrameBuffer;
		std::map<uint32_t, CGfxRenderPassPtr> m_ptrRenderPassesOrderByID;
		std::map<CGfxRenderPassPtr, CGfxRenderPassPtr> m_ptrRenderPasses;
	};

}
