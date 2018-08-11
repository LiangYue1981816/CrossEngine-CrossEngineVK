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

	CUniformCamera::CUniformCamera(void)
	{
		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), &m_params, TRUE);
	}

	CUniformCamera::~CUniformCamera(void)
	{

	}

	const CGfxUniformBufferPtr& CUniformCamera::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CUniformCamera::SetScreen(float width, float height)
	{
		m_bDirty = TRUE;
		m_params.screen = glm::vec4(width, height, 1.0f + 1.0f / width, 1.0f + 1.0f / height);
	}

	void CUniformCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
	{
		static const glm::mat4 LH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

		// [-1 1]
		//float x = (1.0f - zFar / zNear) / 2.0f;
		//float y = (1.0f + zFar / zNear) / 2.0f;

		// [0 1]
		float x = 1.0f - zFar / zNear;
		float y = zFar / zNear;

		m_bDirty = TRUE;
		m_params.projectionMatrix = RenderSystem()->GetAPI() == GFX_API_VULKAN ? LH2RH * glm::perspective(glm::radians(fovy), aspect, zNear, zFar) : glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
		m_params.projection = glm::vec4(1.0f, zNear, zFar, 1.0f / zFar);
		m_params.zbuffer = glm::vec4(x, y, x / zFar, y / zFar);
	}

	void CUniformCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		static const glm::mat4 LH2RH = glm::scale(glm::mat4(), glm::vec3(1.0f, -1.0f, 1.0f));

		// [-1 1]
		//float x = (1.0f - zFar / zNear) / 2.0f;
		//float y = (1.0f + zFar / zNear) / 2.0f;

		// [0 1]
		float x = 1.0f - zFar / zNear;
		float y = zFar / zNear;

		m_bDirty = TRUE;
		m_params.projectionMatrix = RenderSystem()->GetAPI() == GFX_API_VULKAN ? LH2RH * glm::ortho(left, right, bottom, top, zNear, zFar) : glm::ortho(left, right, bottom, top, zNear, zFar);
		m_params.projection = glm::vec4(1.0f, zNear, zFar, 1.0f / zFar);
		m_params.zbuffer = glm::vec4(x, y, x / zFar, y / zFar);
	}

	void CUniformCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
	{
		m_bDirty = TRUE;
		m_params.viewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
		m_params.viewInverseMatrix = glm::inverse(m_params.viewMatrix);
		m_params.viewInverseTransposeMatrix = glm::transpose(m_params.viewInverseMatrix);
	}

	void CUniformCamera::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = FALSE;
			m_ptrUniformBuffer->SetData(0, sizeof(m_params), &m_params);
		}
	}

}
