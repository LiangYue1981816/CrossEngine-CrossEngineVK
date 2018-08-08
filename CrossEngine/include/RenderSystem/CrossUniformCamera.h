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

	class CROSS_EXPORT CUniformCamera
	{
		friend class CRenderSystem;


	protected:
		typedef struct Params {
			glm::vec4 screen;
			glm::vec4 zbuffer;
			glm::vec4 projection;

			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 viewInverseMatrix;
			glm::mat4 viewInverseTransposeMatrix;
		} Params;


	protected:
		CUniformCamera(void);
		virtual ~CUniformCamera(void);


	protected:
		const CGfxUniformBufferPtr& GetUniformBuffer(void) const;

	protected:
		void SetScreen(float width, float height);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
		void Apply(void);


	protected:
		BOOL m_bDirty;
		Params m_params;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

}
