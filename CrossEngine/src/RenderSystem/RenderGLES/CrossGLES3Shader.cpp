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

	static GLenum glGetShaderKind(VkShaderStageFlagBits flags)
	{
		switch (flags) {
		case VK_SHADER_STAGE_VERTEX_BIT: return GL_VERTEX_SHADER;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return GL_FRAGMENT_SHADER;
		case VK_SHADER_STAGE_COMPUTE_BIT: return GL_COMPUTE_SHADER;
		}

		return GL_INVALID_ENUM;
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


	CGLES3Shader::CGLES3Shader(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxShader(pResourceManager)
		, m_pDevice(pDevice)
		, m_pShaderCompiler(NULL)
		, m_program(0)
	{

	}

	CGLES3Shader::~CGLES3Shader(void)
	{

	}

	CGLES3Device* CGLES3Shader::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3Shader::GetHandle(void) const
	{
		return (HANDLE)m_program;
	}

	const spirv_cross::Compiler* CGLES3Shader::GetShaderCompiler(void) const
	{
		return m_pShaderCompiler;
	}

	BOOL CGLES3Shader::Precompile(const char *szSource, size_t length, VkShaderStageFlagBits flags, std::vector<uint32_t> &words)
	{
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", ((CGLES3ShaderManager *)m_pResourceManager)->GetCachePath(), HashValue((const uint8_t *)szSource, length));

		if (LoadShaderBinary(szFileName, words) == FALSE) {
			if (CompileShader(szSource, length, GetShaderKind(flags), ((CGLES3ShaderManager *)m_pResourceManager)->GetCompiler(), ((CGLES3ShaderManager *)m_pResourceManager)->GetCompileOptions(), words) == FALSE) {
				return FALSE;
			}

			SaveShaderBinary(szFileName, words);
		}

		return TRUE;
	}

	BOOL CGLES3Shader::Create(const char *szSource, size_t length, VkShaderStageFlagBits flags)
	{
		std::vector<uint32_t> words;
		Precompile(szSource, length, flags, words);

		return Create(words.data(), words.size(), flags);
	}

	BOOL CGLES3Shader::Create(const uint32_t *words, size_t numWords, VkShaderStageFlagBits flags)
	{
		m_pShaderCompiler = SAFE_NEW spirv_cross::CompilerGLSL(words, numWords);

		spirv_cross::CompilerGLSL::Options options;
		options.version = 310;
		options.es = true;
		((spirv_cross::CompilerGLSL *)m_pShaderCompiler)->set_options(options);

		const std::vector<std::string>& strMacroDefinitions = ((CGLES3ShaderManager *)m_pResourceManager)->GetMacroDefinitions();
		for (const auto &itMacroDefinition : strMacroDefinitions) {
			((spirv_cross::CompilerGLSL *)m_pShaderCompiler)->add_header_line(itMacroDefinition);
		}

		const std::string strSource = m_pShaderCompiler->compile();
		const char *szSource = strSource.c_str();

#ifdef _DEBUG
		switch (flags) {
		case VK_SHADER_STAGE_VERTEX_BIT:
			LOGI("Vertex Shader:\n");
			LOGI("%s\n", szSource);
			break;

		case VK_SHADER_STAGE_FRAGMENT_BIT:
			LOGI("Fragment Shader:\n");
			LOGI("%s\n", szSource);
			break;

		case VK_SHADER_STAGE_COMPUTE_BIT:
			LOGI("Compute Shader:\n");
			LOGI("%s\n", szSource);
			break;
		}
#endif

		m_program = glCreateShaderProgramv(glGetShaderKind(flags), 1, &szSource);
		return m_program != 0 ? TRUE : FALSE;
	}

	void CGLES3Shader::Destroy(void)
	{
		SAFE_DELETE(m_pShaderCompiler);
		glDeleteProgram(m_program);
		m_program = 0;
	}

	void CGLES3Shader::DumpLog(void) const
	{
		if (m_program) {
			LOGI("\t\tShader 0x%x\n", m_program);
		}
	}

}
