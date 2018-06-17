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

	CAmbientLight::CAmbientLight(void)
		: m_bDirty(FALSE)
	{
		memset(&m_params, sizeof(m_params), 0);

		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), NULL, TRUE);
	}

	CAmbientLight::~CAmbientLight(void)
	{
		m_ptrUniformBuffer.Release();
	}

	const CGfxUniformBufferPtr CAmbientLight::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CAmbientLight::SetAmbient(float shRed[9], float shGreen[9], float shBlue[9])
	{
		m_bDirty = TRUE;
		m_params.shRed[0] = glm::vec4(shRed[0], shRed[1], shRed[2], 0.0);
		m_params.shRed[1] = glm::vec4(shRed[3], shRed[4], shRed[5], 0.0);
		m_params.shRed[2] = glm::vec4(shRed[6], shRed[7], shRed[8], 0.0);
		m_params.shGreen[0] = glm::vec4(shGreen[0], shGreen[1], shGreen[2], 0.0);
		m_params.shGreen[1] = glm::vec4(shGreen[3], shGreen[4], shGreen[5], 0.0);
		m_params.shGreen[2] = glm::vec4(shGreen[6], shGreen[7], shGreen[8], 0.0);
		m_params.shBlue[0] = glm::vec4(shBlue[0], shBlue[1], shBlue[2], 0.0);
		m_params.shBlue[1] = glm::vec4(shBlue[3], shBlue[4], shBlue[5], 0.0);
		m_params.shBlue[2] = glm::vec4(shBlue[6], shBlue[7], shBlue[8], 0.0);
	}

	void CAmbientLight::SetRotation(const glm::mat4 &mtxRotation)
	{
		m_bDirty = TRUE;
		m_params.mtxRotation = mtxRotation;
	}

	void CAmbientLight::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = FALSE;
			m_ptrUniformBuffer->SetData(0, sizeof(m_params), &m_params);
		}
	}


	CDirectionLight::CDirectionLight(void)
		: m_bDirty(FALSE)
	{
		memset(&m_params, sizeof(m_params), 0);

		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), NULL, TRUE);
	}

	CDirectionLight::~CDirectionLight(void)
	{
		m_ptrUniformBuffer.Release();
	}

	const CGfxUniformBufferPtr CDirectionLight::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CDirectionLight::SetColor(float red, float green, float blue)
	{
		m_bDirty = TRUE;
		m_params.color = glm::vec4(red, green, blue, 0.0f);
	}

	void CDirectionLight::SetDirection(float x, float y, float z)
	{
		m_bDirty = TRUE;
		m_params.direction = glm::vec4(x, y, z, 0.0f);
	}

	void CDirectionLight::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = FALSE;
			m_ptrUniformBuffer->SetData(0, sizeof(m_params), &m_params);
		}
	}


	CPointLight::CPointLight(void)
	{

	}

	CPointLight::~CPointLight(void)
	{

	}

	const glm::sphere& CPointLight::GetSphere(void) const
	{
		return m_sphere;
	}

	void CPointLight::SetColor(float red, float green, float blue)
	{
		m_params.color = glm::vec4(red, green, blue, 0.0f);
	}

	void CPointLight::SetPosition(float x, float y, float z, float radius)
	{
		m_params.position = glm::vec4(x, y, z, radius);
	}

	void CPointLight::SetAttenuation(float linear, float square, float constant)
	{
		m_params.attenuation = glm::vec4(linear, square, constant, 0.0f);
	}

}
