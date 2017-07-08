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

	CGLES3ShaderManager::CGLES3ShaderManager(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
	{
		m_options.SetWarningsAsErrors();
		m_options.SetSourceLanguage(shaderc_source_language_glsl);
		m_options.SetForcedVersionProfile(310, shaderc_profile_es);
	}

	CGLES3ShaderManager::~CGLES3ShaderManager(void)
	{

	}

	CGfxShaderPtr CGLES3ShaderManager::AllocShader(void)
	{
		CGLES3Shader *pShader = SAFE_NEW CGLES3Shader(m_pDevice, this);
		{
			mutex_autolock mutex(m_mutex);
			m_pResources[pShader] = pShader;
		}
		return CGfxShaderPtr(pShader);
	}

	const shaderc::Compiler& CGLES3ShaderManager::GetCompiler(void) const
	{
		return m_compiler;
	}

	const shaderc::CompileOptions& CGLES3ShaderManager::GetCompileOptions(void) const
	{
		return m_options;
	}

	void CGLES3ShaderManager::AddMacroDefinition(const char *szName)
	{
		m_options.AddMacroDefinition(szName);

		char szMacroDefinition[_MAX_STRING];
		sprintf(szMacroDefinition, "#define %s\n", szName);
		m_strMacroDefinitions.push_back(szMacroDefinition);
	}

	void CGLES3ShaderManager::AddMacroDefinition(const char *szName, const char *szValue)
	{
		m_options.AddMacroDefinition(szName, szValue);

		char szMacroDefinition[_MAX_STRING];
		sprintf(szMacroDefinition, "#define %s %s\n", szName, szValue);
		m_strMacroDefinitions.push_back(szMacroDefinition);
	}

	const std::vector<std::string>& CGLES3ShaderManager::GetMacroDefinitions(void) const
	{
		return m_strMacroDefinitions;
	}

}
