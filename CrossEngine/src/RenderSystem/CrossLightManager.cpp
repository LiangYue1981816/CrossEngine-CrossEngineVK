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

	CLightManager::CLightManager(void)
	{
		m_ptrDescriptorSetLayout = GfxDevice()->AllocDescriptorSetLayout(DESCRIPTOR_SET_FRAME);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_AMBIENT_LIGHT], DESCRIPTOR_BIND_AMBIENT_LIGHT, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
		m_ptrDescriptorSetLayout->SetUniformBinding(DESCRIPTOR_BIND_NAME[DESCRIPTOR_BIND_DIRECTION_LIGHT], DESCRIPTOR_BIND_DIRECTION_LIGHT, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
		m_ptrDescriptorSetLayout->Create();

		m_ptrDescriptorSet = GfxDevice()->AllocDescriptorSet(thread_id(), m_ptrDescriptorSetLayout);
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_AMBIENT_LIGHT, m_ambientLight.GetUniformBuffer());
		m_ptrDescriptorSet->SetUniformBuffer(DESCRIPTOR_BIND_DIRECTION_LIGHT, m_mainDirectionLight.GetUniformBuffer());
		m_ptrDescriptorSet->UpdateDescriptorSets();
	}

	CLightManager::~CLightManager(void)
	{

	}

	const CGfxDescriptorSetPtr& CLightManager::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
	}

	void CLightManager::SetAmbientColor(float shRed[9], float shGreen[9], float shBlue[9])
	{
		m_ambientLight.SetAmbient(shRed, shGreen, shBlue);
		m_ambientLight.Apply();
	}

	void CLightManager::SetAmbientRotation(float angle, float axisx, float axisy, float axisz)
	{
		m_ambientLight.SetRotation(angle, axisx, axisy, axisz);
		m_ambientLight.Apply();
	}

	void CLightManager::SetMainLightColor(float red, float green, float blue)
	{
		m_mainDirectionLight.SetColor(red, green, blue);
		m_mainDirectionLight.Apply();
	}

	void CLightManager::SetMainLightDirection(float x, float y, float z)
	{
		m_mainDirectionLight.SetDirection(x, y, z);
		m_mainDirectionLight.Apply();
	}

}
