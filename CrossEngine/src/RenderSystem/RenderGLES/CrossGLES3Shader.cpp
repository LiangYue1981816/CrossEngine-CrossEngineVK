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

		DWORD dwHashValue = HashValue((BYTE *)words.data(), sizeof(uint32_t) * words.size());

		fwrite(&dwHashValue, sizeof(dwHashValue), 1, pFile);
		fwrite(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return TRUE;
	}

	static BOOL LoadShaderBinary(const char *szFileName, std::vector<uint32_t> &words)
	{
		FILE *pFile = fopen(szFileName, "rb");
		if (pFile == NULL) return FALSE;

		DWORD dwHashValue;

		words.clear();
		words.resize((fsize(pFile) - sizeof(dwHashValue)) / sizeof(uint32_t));

		fread(&dwHashValue, sizeof(dwHashValue), 1, pFile);
		fread(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return HashValue((BYTE *)words.data(), sizeof(uint32_t) * words.size()) == dwHashValue ? TRUE : FALSE;
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
		, m_shader(0)
	{

	}

	CGLES3Shader::~CGLES3Shader(void)
	{

	}

	HANDLE CGLES3Shader::GetHandle(void) const
	{
		return (HANDLE)m_shader;
	}

	const spirv::module_type& CGLES3Shader::GetMoudleType(void) const
	{
		return m_moduleType;
	}

	BOOL CGLES3Shader::Precompile(const char *szSource, size_t length, VkShaderStageFlagBits flags)
	{
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", m_pDevice->GetInstance()->GetCachePath(), HashValue(szSource));

		std::vector<uint32_t> words;
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
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", m_pDevice->GetInstance()->GetCachePath(), HashValue(szSource));

		std::vector<uint32_t> words;
		Precompile(szSource, length, flags);
		LoadShaderBinary(szFileName, words);
		m_moduleType = spirv::parse(words.data(), words.size());

		GLint compiled = GL_FALSE;
		GLenum shaderType = glGetShaderKind(flags);

		m_shader = glCreateShader(shaderType);
		glShaderSource(m_shader, 1, &szSource, NULL);
		glCompileShader(m_shader);
		glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compiled);
		return compiled;
	}

	void CGLES3Shader::Destroy(void)
	{
		if (m_shader) {
			glDeleteShader(m_shader);
		}

		m_shader = 0;
	}

	void CGLES3Shader::DumpLog(void) const
	{
		if (m_shader) {
			LOGI("\t\tShader 0x%x\n", m_shader);
		}
	}

}
