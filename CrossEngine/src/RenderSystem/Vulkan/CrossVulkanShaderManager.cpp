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

	CVulkanShaderManager::CVulkanShaderManager(CVulkanDevice *pDevice)
		: CVulkanResourceManager(pDevice)
	{
		SetWarningsAsErrors();
		SetSourceLanguage(shaderc_source_language_glsl);
		SetForcedVersionProfile(310, shaderc_profile_es);
//		SetForcedVersionProfile(450, shaderc_profile_core);
	}

	CVulkanShaderManager::~CVulkanShaderManager(void)
	{

	}

	CVulkanShader* CVulkanShaderManager::AllocShader(void)
	{
		CVulkanShader *pShader = SAFE_NEW CVulkanShader(m_pDevice, this);
		m_pResources[pShader] = pShader;

		return pShader;
	}

	void CVulkanShaderManager::SetWarningsAsErrors(void)
	{
		m_options.SetWarningsAsErrors();
	}

	void CVulkanShaderManager::SetSourceLanguage(shaderc_source_language lang)
	{
		m_options.SetSourceLanguage(lang);
	}

	void CVulkanShaderManager::SetOptimizationLevel(shaderc_optimization_level level)
	{
		m_options.SetOptimizationLevel(level);
	}

	void CVulkanShaderManager::SetTargetEnvironment(uint32_t version, shaderc_target_env target)
	{
		m_options.SetTargetEnvironment(target, version);
	}

	void CVulkanShaderManager::SetForcedVersionProfile(uint32_t version, shaderc_profile profile)
	{
		m_options.SetForcedVersionProfile(version, profile);
	}

	void CVulkanShaderManager::SetMacroDefinition(const char *szName, const char *szValue)
	{
		m_options.AddMacroDefinition(szName, szValue);
	}

	const shaderc::CompileOptions& CVulkanShaderManager::GetCompileOptions(void) const
	{
		return m_options;
	}

}
