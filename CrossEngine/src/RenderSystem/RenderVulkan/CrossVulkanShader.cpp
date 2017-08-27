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


	CVulkanShader::CVulkanShader(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxShader(pResourceManager)
		, m_pDevice(pDevice)
		, m_pShaderCompiler(NULL)
		, m_vkShaderModule(VK_NULL_HANDLE)
	{

	}

	CVulkanShader::~CVulkanShader(void)
	{

	}

	CVulkanDevice* CVulkanShader::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanShader::GetHandle(void) const
	{
		return m_vkShaderModule;
	}

	const spirv_cross::Compiler* CVulkanShader::GetShaderCompiler(void) const
	{
		return m_pShaderCompiler;
	}

	BOOL CVulkanShader::Precompile(const char *szSource, size_t length, VkShaderStageFlagBits flags)
	{
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", ((CVulkanShaderManager *)m_pResourceManager)->GetCachePath(), HashValue(szSource));

		std::vector<uint32_t> words;
		if (LoadShaderBinary(szFileName, words) == FALSE) {
			if (CompileShader(szSource, length, GetShaderKind(flags), ((CVulkanShaderManager *)m_pResourceManager)->GetCompiler(), ((CVulkanShaderManager *)m_pResourceManager)->GetCompileOptions(), words) == FALSE) {
				return FALSE;
			}

			SaveShaderBinary(szFileName, words);
		}

		return TRUE;
	}

	BOOL CVulkanShader::Create(const char *szSource, size_t length, VkShaderStageFlagBits flags)
	{
		char szFileName[_MAX_STRING];
		sprintf(szFileName, "%s/%x", ((CVulkanShaderManager *)m_pResourceManager)->GetCachePath(), HashValue(szSource));

		std::vector<uint32_t> words;
		Precompile(szSource, length, flags);
		LoadShaderBinary(szFileName, words);

		return Create(words.data(), words.size());
	}

	BOOL CVulkanShader::Create(const uint32_t *words, size_t numWords)
	{
		try {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.codeSize = numWords * 4;
			createInfo.pCode = words;
			CALL_VK_FUNCTION_THROW(vkCreateShaderModule(m_pDevice->GetDevice(), &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkShaderModule));

			m_pShaderCompiler = SAFE_NEW spirv_cross::Compiler(words, numWords);

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanShader::Destroy(void)
	{
		SAFE_DELETE(m_pShaderCompiler);

		if (m_vkShaderModule) {
			vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShaderModule, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkShaderModule = VK_NULL_HANDLE;
	}

	void CVulkanShader::DumpLog(void) const
	{
		if (m_vkShaderModule) {
			LOGI("\t\tShader 0x%x\n", m_vkShaderModule);
		}
	}

}
