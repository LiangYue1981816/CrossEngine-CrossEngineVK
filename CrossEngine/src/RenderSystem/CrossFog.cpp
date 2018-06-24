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

	CFog::CFog(void)
		: m_bDirty(FALSE)
	{
		memset(&m_param, sizeof(m_param), 0);

		m_ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
		m_ptrUniformBuffer->Create(sizeof(m_param), &m_param, TRUE);
	}

	CFog::~CFog(void)
	{

	}

	const CGfxUniformBufferPtr CFog::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	void CFog::SetColor(float red, float green, float blue)
	{
		m_bDirty = TRUE;
		m_param.color = glm::vec4(red, green, blue, 0.0f);
	}

	void CFog::SetHeightDensity(float startHeight, float endHeight, float density)
	{
		m_bDirty = TRUE;
		m_param.heightDensity = glm::vec4(startHeight, endHeight, density, 0.0f);
	}

	void CFog::SetDistanceDensity(float startDistance, float endDistance, float density)
	{
		m_bDirty = TRUE;
		m_param.distanceDensity = glm::vec4(startDistance, endDistance, density, 0.0f);
	}

	void CFog::Apply(void)
	{
		if (m_bDirty) {
			m_bDirty = FALSE;
			m_ptrUniformBuffer->SetData(0, sizeof(m_param), &m_param);
		}
	}

}
