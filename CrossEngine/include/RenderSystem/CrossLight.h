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

	class CROSS_EXPORT CAmbientLight
	{
		friend class CRenderSystem;


	protected:
		typedef struct Param {
			glm::vec4 shRed[3];
			glm::vec4 shGreen[3];
			glm::vec4 shBlue[3];
			glm::mat4 mtxRotation;
		} Param;


	protected:
		CAmbientLight(void);
		virtual ~CAmbientLight(void);


	public:
		const CGfxUniformBufferPtr GetUniformBuffer(void) const;

	public:
		void SetAmbient(float shRed[9], float shGreen[9], float shBlue[9]);
		void SetRotation(const glm::mat4 &mtxRotation);
		void Apply(void);


	protected:
		bool m_bDirty;
		Param m_param;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

	class CROSS_EXPORT CDirectLight
	{
		friend class CRenderSystem;


	protected:
		typedef struct Param {
			glm::vec4 color;
			glm::vec4 direction;
		} Param;


	protected:
		CDirectLight(void);
		virtual ~CDirectLight(void);


	public:
		const CGfxUniformBufferPtr GetUniformBuffer(void) const;

	public:
		void SetColor(float red, float green, float blue);
		void SetDirection(float x, float y, float z);
		void Apply(void);


	protected:
		bool m_bDirty;
		Param m_param;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

	class CROSS_EXPORT CPointLight
	{
		friend class CRenderSystem;


	protected:
		typedef struct Param {
			glm::vec4 color;
			glm::vec4 position;
			glm::vec4 attenuation;
		} Param;


	protected:
		CPointLight(void);
		virtual ~CPointLight(void);


	public:
		const CGfxUniformBufferPtr GetUniformBuffer(void) const;

	public:
		void SetColor(float red, float green, float blue);
		void SetPosition(float x, float y, float z, float radius);
		void SetAttenuation(float linear, float square, float constant);
		void Apply(void);


	protected:
		bool m_bDirty;
		Param m_param;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

	class CROSS_EXPORT CPointLightDeferred
	{
		friend class CRenderSystem;


	protected:
		typedef struct Param {
			glm::vec4 color;
			glm::vec4 position;
			glm::vec4 attenuation;
		} Param;


	protected:
		CPointLightDeferred(void);
		virtual ~CPointLightDeferred(void);


	public:
		const glm::sphere& GetSphere(void) const;

	public:
		void SetColor(float red, float green, float blue);
		void SetPosition(float x, float y, float z, float radius);
		void SetAttenuation(float linear, float square, float constant);


	protected:
		Param m_param;
		glm::sphere m_sphere;
	};

}
