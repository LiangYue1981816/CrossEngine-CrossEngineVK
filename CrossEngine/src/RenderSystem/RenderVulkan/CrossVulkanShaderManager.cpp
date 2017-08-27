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
		: m_pDevice(pDevice)
		, m_fileIncluder(new glslc::FileIncluder(&m_fileFinder))
	{
		m_options.SetIncluder(std::move(m_fileIncluder));
		m_options.SetWarningsAsErrors();
		m_options.SetSourceLanguage(shaderc_source_language_glsl);
		m_options.SetForcedVersionProfile(310, shaderc_profile_es);
	}

	CVulkanShaderManager::~CVulkanShaderManager(void)
	{

	}

	CGfxShaderPtr CVulkanShaderManager::AllocShader(void)
	{
		CVulkanShader *pShader = SAFE_NEW CVulkanShader(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pShader] = pShader;
		}
		return CGfxShaderPtr(pShader);
	}

	const shaderc::Compiler& CVulkanShaderManager::GetCompiler(void) const
	{
		return m_compiler;
	}

	const shaderc::CompileOptions& CVulkanShaderManager::GetCompileOptions(void) const
	{
		return m_options;
	}

	void CVulkanShaderManager::AddIncludePath(const char *szPath)
	{
		m_fileFinder.search_path().push_back(szPath);
	}

	void CVulkanShaderManager::AddMacroDefinition(const char *szName)
	{
		m_options.AddMacroDefinition(szName);
	}

	void CVulkanShaderManager::AddMacroDefinition(const char *szName, const char *szValue)
	{
		m_options.AddMacroDefinition(szName, szValue);
	}

}
