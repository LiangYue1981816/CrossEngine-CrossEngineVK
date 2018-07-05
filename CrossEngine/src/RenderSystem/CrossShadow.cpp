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

	CShadow::CShadow(void)
		: m_bDirty(FALSE)
	{
		memset(&m_params, sizeof(m_params), 0);

		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), &m_params, TRUE);
	}

	CShadow::~CShadow(void)
	{

	}

	const CGfxUniformBufferPtr CShadow::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CShadow::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_bDirty = TRUE;
		m_params.params.x = zFar - zNear;
		m_params.params.y = 1.0f / (zFar - zNear);
		m_params.mtxProjection = glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	void CShadow::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
	{
		m_bDirty = TRUE;
		m_params.mtxView = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
	}

	void CShadow::SetDistance(float distance)
	{
		m_bDirty = TRUE;
		m_params.params.z = distance;
	}

	void CShadow::SetResolution(float resolution)
	{
		m_bDirty = TRUE;
		m_params.params.w = resolution;
	}

	void CShadow::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = FALSE;
			m_ptrUniformBuffer->SetData(0, sizeof(m_params), &m_params);
		}
	}

}
