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

	static BOOL SaveShader(const char *szFileName, const std::vector<uint32_t> &words)
	{
		FILE *pFile = fopen(szFileName, "wb");  
		if (pFile == NULL) return FALSE;
		
		fwrite(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return TRUE;
	}

	static BOOL LoadShader(const char *szFileName, std::vector<uint32_t> &words)
	{
		FILE *pFile = fopen(szFileName, "rb");
		if (pFile == NULL) return FALSE;

		words.clear();
		words.resize(fsize(pFile) / sizeof(uint32_t));

		fread(words.data(), sizeof(uint32_t), words.size(), pFile);
		fclose(pFile);

		return TRUE;
	}

	static BOOL CompileShader(const char *source, size_t length, shaderc_shader_kind kind, const shaderc::CompileOptions &options, std::vector<uint32_t> &words)
	{
		shaderc::Compiler compiler;
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, length, kind, "SPIR-V Compiler", options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			LOGW(module.GetErrorMessage().c_str());
			return FALSE;
		}

		words = { module.cbegin(), module.cend() };
		return TRUE;
	}

	CVulkanShader::CVulkanShader(CVulkanDevice *pDevice, CVulkanResourceManager *pManager)
		: CVulkanResource(pDevice, pManager)
		, m_vkShaderModule(VK_NULL_HANDLE)
	{

	}

	CVulkanShader::~CVulkanShader(void)
	{
		ASSERT(m_vkShaderModule == VK_NULL_HANDLE);
	}

	BOOL CVulkanShader::Create(const char *szSource, size_t length, shaderc_shader_kind kind)
	{
		char szFileName[_MAX_PATH];
		sprintf(szFileName, "%s/%x", m_pDevice->GetVulkan()->GetCachePath(), HashValue(szSource));

		std::vector<uint32_t> words;
		if (LoadShader(szFileName, words) == FALSE) {
			if (CompileShader(szSource, length, kind, CreateCompileOptions(), words) == FALSE) {
				return FALSE;
			}

			SaveShader(szFileName, words);
		}

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
			CALL_VK_FUNCTION_THROW(vkCreateShaderModule(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks(), &m_vkShaderModule));

			m_module = spirv::parse(words, numWords);

			return TRUE;
		}
		catch (VkResult err) {
			CVulkan::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	shaderc::CompileOptions CVulkanShader::CreateCompileOptions(void)
	{
		shaderc::CompileOptions options(((CVulkanShaderManager *)m_pManager)->GetCompileOptions());

		for (const auto &itMacroDefinition : m_strMacroDefinitions) {
			if (itMacroDefinition.second.empty()) {
				options.AddMacroDefinition(itMacroDefinition.first.c_str());
			}
			else {
				options.AddMacroDefinition(itMacroDefinition.first.c_str(), itMacroDefinition.second.c_str());
			}
		}

		return options;
	}

	void CVulkanShader::Destroy(void)
	{
		if (m_vkShaderModule) {
			vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShaderModule, m_pDevice->GetVulkan()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkShaderModule = VK_NULL_HANDLE;
	}

	void CVulkanShader::AddMacroDefinition(const char *szName, const char *szValue)
	{
		m_strMacroDefinitions[szName] = szValue;
	}

	void CVulkanShader::DelMacroDefinition(const char *szName)
	{
		const auto &itMacroDefinition = m_strMacroDefinitions.find(szName);
		if (itMacroDefinition != m_strMacroDefinitions.end()) m_strMacroDefinitions.erase(itMacroDefinition);
	}

	void CVulkanShader::ClearMacroDefinitions(void)
	{
		m_strMacroDefinitions.clear();
	}

	VkShaderModule CVulkanShader::GetShaderModule(void) const
	{
		return m_vkShaderModule;
	}

	const spirv::module_type& CVulkanShader::GetModule(void) const
	{
		return m_module;
	}

	void CVulkanShader::DumpLog(void) const
	{
		if (m_vkShaderModule) {
			LOGI("\t\tShader 0x%x\n", m_vkShaderModule);
		}
	}

}
