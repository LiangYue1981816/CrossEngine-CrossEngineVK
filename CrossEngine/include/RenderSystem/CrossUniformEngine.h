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

	class CROSS_EXPORT CUniformEngine
	{
		friend class CRenderSystem;


	protected:
		typedef struct Params {
			glm::vec4 time;
			glm::vec4 sinTime;
			glm::vec4 cosTime;
			glm::vec4 deltaTime;

			glm::mat4 shadowProjectionMatrix;
			glm::mat4 shadowViewMatrix;
			glm::vec4 shadowParams;

			glm::vec4 ambientLightRed0;
			glm::vec4 ambientLightRed1;
			glm::vec4 ambientLightRed2;
			glm::vec4 ambientLightGreen0;
			glm::vec4 ambientLightGreen1;
			glm::vec4 ambientLightGreen2;
			glm::vec4 ambientLightBlue0;
			glm::vec4 ambientLightBlue1;
			glm::vec4 ambientLightBlue2;
			glm::mat4 ambientLightRotationMatrix;

			glm::vec4 pointLightColor;
			glm::vec4 pointLightPosition;
			glm::vec4 pointLightAttenuation;

			glm::vec4 directLightColor;
			glm::vec4 directLightDirection;

			glm::vec4 fogColor;
			glm::vec4 fogHeightDensity;
			glm::vec4 fogDistanceDensity;
		} Params;


	protected:
		CUniformEngine(void);
		virtual ~CUniformEngine(void);


	protected:
		const CGfxUniformBufferPtr& GetUniformBuffer(void) const;

	protected:
		void SetTime(float t, float dt);

		void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
		void SetShadowDistance(float distance);
		void SetShadowResolution(float resolution);

		void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
		void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);

		void SetPointLightColor(float red, float green, float blue);
		void SetPointLightPosition(float x, float y, float z);
		void SetPointLightAttenuation(float linear, float square, float constant);

		void SetDirectLightColor(float red, float green, float blue);
		void SetDirectLightDirection(float x, float y, float z);

		void SetFogColor(float red, float green, float blue);
		void SetFogHeightDensity(float startHeight, float endHeight, float density);
		void SetFogDistanceDensity(float startDistance, float endDistance, float density);

		void Apply(void);


	protected:
		BOOL m_bDirty;
		Params m_params;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

}
