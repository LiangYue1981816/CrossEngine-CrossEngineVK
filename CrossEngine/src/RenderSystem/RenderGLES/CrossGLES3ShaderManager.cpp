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

	static BOOL SaveShaderBinary(const char *szFileName, const std::vector<uint32_t> &words)
	{
		FILE *pFile = fopen(szFileName, "wb");
		if (pFile == NULL) return FALSE;

		uint32_t dwHashValue = HashValue((uint8_t *)words.data(), sizeof(uint32_t) * words.size());

		fwrite(&dwHashValue, sizeof(dwHashValue), 1, pFile);
		fwrite(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return TRUE;
	}

	static BOOL LoadShaderBinary(const char *szFileName, std::vector<uint32_t> &words)
	{
		FILE *pFile = fopen(szFileName, "rb");
		if (pFile == NULL) return FALSE;

		uint32_t dwHashValue;

		words.clear();
		words.resize((fsize(pFile) - sizeof(dwHashValue)) / sizeof(uint32_t));

		fread(&dwHashValue, sizeof(dwHashValue), 1, pFile);
		fread(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return HashValue((uint8_t *)words.data(), sizeof(uint32_t) * words.size()) == dwHashValue ? TRUE : FALSE;
	}

	static shaderc_shader_kind GetShaderKind(VkShaderStageFlagBits flags)
	{
		switch (flags) {
		case VK_SHADER_STAGE_VERTEX_BIT: return shaderc_glsl_default_vertex_shader;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return shaderc_glsl_default_tess_control_shader;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return shaderc_glsl_default_tess_evaluation_shader;
		case VK_SHADER_STAGE_GEOMETRY_BIT: return shaderc_glsl_default_geometry_shader;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return shaderc_glsl_default_fragment_shader;
		case VK_SHADER_STAGE_COMPUTE_BIT: return shaderc_glsl_default_compute_shader;
		}

		return shaderc_glsl_infer_from_source;
	}

	static BOOL CompileShader(const char *source, size_t length, shaderc_shader_kind kind, const shaderc::Compiler &compiler, const shaderc::CompileOptions &options, std::vector<uint32_t> &words)
	{
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, length, kind, "SPIR-V Compiler", options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			LOGW(module.GetErrorMessage().c_str());
			return FALSE;
		}

		words = { module.cbegin(), module.cend() };
		return TRUE;
	}


	CGLES3ShaderManager::CGLES3ShaderManager(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_fileIncluder(new glslc::FileIncluder(&m_fileFinder))
		, m_szCachePath{ 0 }
	{
		m_options.SetIncluder(std::move(m_fileIncluder));
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

	const char* CGLES3ShaderManager::GetCachePath(void) const
	{
		return m_szCachePath;
	}

	const shaderc::Compiler& CGLES3ShaderManager::GetCompiler(void) const
	{
		return m_compiler;
	}

	const shaderc::CompileOptions& CGLES3ShaderManager::GetCompileOptions(void) const
	{
		return m_options;
	}

	void CGLES3ShaderManager::SetCachePath(const char *szPath)
	{
		strcpy(m_szCachePath, szPath);
	}

	void CGLES3ShaderManager::AddIncludePath(const char *szPath)
	{
		m_fileFinder.search_path().push_back(szPath);
	}

	void CGLES3ShaderManager::AddMacroDefinition(const char *szName)
	{
		m_options.AddMacroDefinition(szName);

		char szMacroDefinition[_MAX_STRING];
		sprintf(szMacroDefinition, "#define %s", szName);
		m_strMacroDefinitions.push_back(szMacroDefinition);
	}

	void CGLES3ShaderManager::AddMacroDefinition(const char *szName, const char *szValue)
	{
		m_options.AddMacroDefinition(szName, szValue);

		char szMacroDefinition[_MAX_STRING];
		sprintf(szMacroDefinition, "#define %s %s", szName, szValue);
		m_strMacroDefinitions.push_back(szMacroDefinition);
	}

	BOOL CGLES3ShaderManager::Precompile(const char *szSource, size_t length, VkShaderStageFlagBits flags, std::vector<uint32_t> &words)
	{
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", GetCachePath(), HashValue((const uint8_t *)szSource, length));

		if (LoadShaderBinary(szFileName, words) == FALSE) {
			if (CompileShader(szSource, length, GetShaderKind(flags), GetCompiler(), GetCompileOptions(), words) == FALSE) {
				return FALSE;
			}

			SaveShaderBinary(szFileName, words);
		}

		return TRUE;
	}

}
