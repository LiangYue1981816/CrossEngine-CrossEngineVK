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

	static void SHRotationMatrix0(float rotation0[1][1])
	{
		rotation0[0][0] = 1.0f;
	}

	static void SHRotationMatrix1(float rotation1[3][3], float angle, const glm::vec3 &axis)
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(), angle, axis);

		rotation1[0][0] = rotation[2][2];
		rotation1[0][1] = -rotation[2][0];
		rotation1[0][2] = -rotation[2][1];

		rotation1[1][0] = -rotation[0][2];
		rotation1[1][1] = rotation[0][0];
		rotation1[1][2] = -rotation[0][1];

		rotation1[2][0] = -rotation[1][2];
		rotation1[2][1] = -rotation[1][0];
		rotation1[2][2] = rotation[1][1];
	}

	static void SHRotationMatrix2(float rotation2[5][5], const float rotation1[3][3])
	{
		const int l = 2;
		float P[3][5][5] = { 0.0f };

		for (int i = -1; i <= 1; i++) {
			for (int m = -1; m <= 1; m++) {
				for (int n = -l; n <= l; n++) {
					if (n == l) {
						P[i + 1][m + l][n + l] = rotation1[i + 1][2] * rotation1[m + 1][l - 1 + 1] - rotation1[i + 1][0] * rotation1[m + 1][-l + 1 + 1];
					}
					else if (n == -l) {
						P[i + 1][m + l][n + l] = rotation1[i + 1][2] * rotation1[m + 1][-l + 1 + 1] + rotation1[i + 1][0] * rotation1[m + 1][l - 1 + 1];
					}
					else {
						P[i + 1][m + l][n + l] = rotation1[i + 1][1] * rotation1[m + 1][n + 1];
					}
				}
			}
		}

		for (int m = -l; m <= l; m++) {
			for (int n = -l; n <= l; n++) {
				float d = (m == 0) ? 1.0f : 0.0f;
				float denom = (abs(n) == l) ? 2.0f * l * (2.0f * l - 1.0f) : (l + n) * (l - n);
				float U = 0.0f, u = 1.0f * sqrtf((l + m) * (l - m) / denom);
				float V = 0.0f, v = 0.5f * sqrtf((1 + d) * (l + abs(m) - 1) * (l + abs(m)) / denom) * (1 - 2 * d);
				float W = 0.0f, w = -0.5f * sqrtf((l - abs(m) - 1) * (l - abs(m)) / denom) * (1 - d);

				if (m == 0) {
					U = P[0 + 1][m + l][n + l];
					V = P[1 + 1][1 + l][n + l] + P[-1 + 1][-1 + l][n + l];
					W = P[1 + 1][m + 1 + l][n + l] + P[-1 + 1][-m - 1 + l][n + l];
				}

				if (m > 0) {
					float d = (m == 1) ? 1.0f : 0.0f;
					U = P[0 + 1][m + l][n + l];
					V = P[1 + 1][m - 1 + l][n + l] * sqrtf(1 + d) - P[-1 + 1][-m + 1 + l][n + l] * (1 - d);
					W = w != 0.0f ? P[1 + 1][m + 1 + l][n + l] + P[-1 + 1][-m - 1 + l][n + l] : 0.0f;
				}

				if (m < 0) {
					float d = (m == -1) ? 1.0f : 0.0f;
					U = P[0 + 1][m + l][n + l];
					V = P[1 + 1][m + 1 + l][n + l] * (1 - d) + P[-1 + 1][-m - 1 + l][n + l] * sqrtf(1 + d);
					W = w != 0.0f ? P[1 + 1][m - 1 + l][n + l] - P[-1 + 1][-m + 1 + l][n + l] : 0.0f;
				}

				rotation2[m + l][n + l] = u * U + v * V + w * W;
			}
		}
	}

	static void SHRotationMatrix(float rotation[9][9], float angle, const glm::vec3 &axis)
	{
		float rotation0[1][1] = { 0.0f };
		float rotation1[3][3] = { 0.0f };
		float rotation2[5][5] = { 0.0f };

		{
			SHRotationMatrix0(rotation0);
			SHRotationMatrix1(rotation1, angle, axis);
			SHRotationMatrix2(rotation2, rotation1);
		}

		{
			rotation[0][0] = rotation0[0][0];

			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					rotation[m + 1][n + 1] = rotation1[m][n];
				}
			}

			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					rotation[m + 4][n + 4] = rotation2[m][n];
				}
			}
		}
	}

	static void SHRotate(float shRedRotate[9], float shGreenRotate[9], float shBlueRotate[9], float shRedSrc[9], float shGreenSrc[9], float shBlueSrc[9], float angle, const glm::vec3 &axis)
	{
		float rotation[9][9] = { 0.0f };
		SHRotationMatrix(rotation, angle, axis);

		for (int m = 0; m < 9; m++) {
			shRedRotate[m] = 0.0f;
			shGreenRotate[m] = 0.0f;
			shBlueRotate[m] = 0.0f;

			for (int n = 0; n < 9; n++) {
				shRedRotate[m] += shRedSrc[n] * rotation[m][n];
				shGreenRotate[m] += shGreenSrc[n] * rotation[m][n];
				shBlueRotate[m] += shBlueSrc[n] * rotation[m][n];
			}
		}
	}


	CUniformEngine::CUniformEngine(void)
	{
		SetPointLightColor(0.0f, 0.0f, 0.0f);
		SetPointLightPosition(0.0f, 0.0f, 0.0f);
		SetPointLightAttenuation(0.0f, 0.0f, 1.0f);

		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_params), &m_params, TRUE);
	}

	CUniformEngine::~CUniformEngine(void)
	{

	}

	const CGfxUniformBufferPtr& CUniformEngine::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CUniformEngine::SetTime(float t, float dt)
	{
		m_bDirty = true;
		m_params.time = glm::vec4(t / 20.0f, t * 1.0f, t * 2.0f, t * 3.0f);
		m_params.sinTime = glm::vec4(sinf(t / 8.0f), sinf(t / 4.0f), sinf(t / 2.0f), t);
		m_params.cosTime = glm::vec4(cosf(t / 8.0f), cosf(t / 4.0f), cosf(t / 2.0f), t);
		m_params.deltaTime = glm::vec4(dt, 1.0f / dt, 1.0f, 1.0f);
	}

	void CUniformEngine::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_bDirty = true;
		m_params.shadowProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		m_params.shadowParams.x = zFar - zNear;
		m_params.shadowParams.y = 1.0f / (zFar - zNear);
	}

	void CUniformEngine::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
	{
		m_bDirty = true;
		m_params.shadowViewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
	}

	void CUniformEngine::SetShadowDistance(float distance)
	{
		m_bDirty = true;
		m_params.shadowParams.z = distance;
	}

	void CUniformEngine::SetShadowResolution(float resolution)
	{
		m_bDirty = true;
		m_params.shadowParams.w = resolution;
	}

	void CUniformEngine::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
	{
		m_bDirty = true;
		m_params.ambientLightRed0 = glm::vec4(shRed[0], shRed[1], shRed[2], 0.0);
		m_params.ambientLightRed1 = glm::vec4(shRed[3], shRed[4], shRed[5], 0.0);
		m_params.ambientLightRed2 = glm::vec4(shRed[6], shRed[7], shRed[8], 0.0);
		m_params.ambientLightGreen0 = glm::vec4(shGreen[0], shGreen[1], shGreen[2], 0.0);
		m_params.ambientLightGreen1 = glm::vec4(shGreen[3], shGreen[4], shGreen[5], 0.0);
		m_params.ambientLightGreen2 = glm::vec4(shGreen[6], shGreen[7], shGreen[8], 0.0);
		m_params.ambientLightBlue0 = glm::vec4(shBlue[0], shBlue[1], shBlue[2], 0.0);
		m_params.ambientLightBlue1 = glm::vec4(shBlue[3], shBlue[4], shBlue[5], 0.0);
		m_params.ambientLightBlue2 = glm::vec4(shBlue[6], shBlue[7], shBlue[8], 0.0);
	}

	void CUniformEngine::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
	{
		/*
		float shRedRotate[9] = { 0.0f };
		float shGreenRotate[9] = { 0.0f };
		float shBlueRotate[9] = { 0.0f };
		SHRotate(shRedRotate, shGreenRotate, shBlueRotate, shRed, shGreen, shBlue, angle, glm::vec3(axisx, axisy, axisz));
		SetAmbientLightSH(shRedRotate, shGreenRotate, shBlueRotate);
		*/
		m_bDirty = true;
		m_params.ambientLightRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(axisx, axisy, axisz));
	}

	void CUniformEngine::SetPointLightColor(float red, float green, float blue)
	{
		m_bDirty = true;
		m_params.pointLightColor = glm::vec4(red, green, blue, 0.0f);
	}

	void CUniformEngine::SetPointLightPosition(float x, float y, float z)
	{
		m_bDirty = true;
		m_params.pointLightPosition = glm::vec4(x, y, z, 0.0f);
	}

	void CUniformEngine::SetPointLightAttenuation(float linear, float square, float constant)
	{
		m_bDirty = true;
		m_params.pointLightAttenuation = glm::vec4(linear, square, constant, 0.0f);
	}

	void CUniformEngine::SetDirectLightColor(float red, float green, float blue)
	{
		m_bDirty = true;
		m_params.directLightColor = glm::vec4(red, green, blue, 0.0);
	}

	void CUniformEngine::SetDirectLightDirection(float x, float y, float z)
	{
		m_bDirty = true;
		m_params.directLightDirection = glm::normalize(glm::vec4(-x, -y, -z, 0.0));
	}

	void CUniformEngine::SetFogColor(float red, float green, float blue)
	{
		m_bDirty = true;
		m_params.fogColor = glm::vec4(red, green, blue, 0.0f);
	}

	void CUniformEngine::SetFogHeightDensity(float startHeight, float endHeight, float density)
	{
		m_bDirty = true;
		m_params.fogHeightDensity = glm::vec4(startHeight, endHeight, density, 0.0f);
	}

	void CUniformEngine::SetFogDistanceDensity(float startDistance, float endDistance, float density)
	{
		m_bDirty = true;
		m_params.fogDistanceDensity = glm::vec4(startDistance, endDistance, density, 0.0f);
	}

	void CUniformEngine::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = false;
			m_ptrUniformBuffer->SetData(0, sizeof(m_params), &m_params);
		}
	}

}
