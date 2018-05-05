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
		typedef struct Params {
			glm::vec3 position;
			glm::mat4 mtxProjection;
			glm::mat4 mtxWorldToView;
			glm::mat4 mtxWorldToViewInverse;
		} Params;


	protected:
		CCamera(void);
		virtual ~CCamera(void);


	public:
		float GetViewportX(void) const;
		float GetViewportY(void) const;
		float GetViewportWidth(void) const;
		float GetViewportHeight(void) const;

		const CGfxDescriptorSetPtr& GetDescriptorSet(void) const;

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
		void ClearRenderQueue(void);
		void AddRenderQueue(const CDrawable *pDrawable);

	protected:
		void Update(void);
		void Render(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);


	protected:
		BOOL m_bEnable;

	protected:
		Params m_params;
		glm::camera m_camera;

	protected:
		CGfxUniformBufferPtr m_ptrUniformBuffer;
		CGfxDescriptorSetLayoutPtr m_ptrDescriptorSetLayout;
		std::map<uint32_t, CGfxDescriptorSetPtr> m_ptrDescriptorSets;

	protected:
		CRenderer m_renderer;
	};

}
