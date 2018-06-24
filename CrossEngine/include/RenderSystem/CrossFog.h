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

	class CROSS_EXPORT CFog
	{
		friend class CRenderSystem;


	protected:
		typedef struct Param {
			glm::vec4 color;
			glm::vec4 heightDensity;
			glm::vec4 distanceDensity;
		} Param;


	protected:
		CFog(void);
		virtual ~CFog(void);


	public:
		const CGfxUniformBufferPtr GetUniformBuffer(void) const;

	public:
		void SetColor(float red, float green, float blue);
		void SetHeightDensity(float startHeight, float endHeight, float density);
		void SetDistanceDensity(float startDistance, float endDistance, float density);
		void Apply(void);


	protected:
		bool m_bDirty;
		Param m_param;
		CGfxUniformBufferPtr m_ptrUniformBuffer;
	};

}
