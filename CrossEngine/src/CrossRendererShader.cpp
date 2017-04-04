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

	static std::vector<uint32_t> CompileShader(const char *source, size_t length, shaderc_shader_kind kind, const shaderc::CompileOptions &options)
	{
		shaderc::Compiler compiler;
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, length, kind, "SPIR-V Compiler", options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			LOGW(module.GetErrorMessage().c_str());
			return std::vector<uint32_t>();
		}

		return { module.cbegin(), module.cend() };
	}

	CRendererShader::CRendererShader(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererResource(pDevice, pManager)
		, m_vkShaderModule(VK_NULL_HANDLE)
	{

	}

	CRendererShader::~CRendererShader(void)
	{
		ASSERT(m_vkShaderModule == VK_NULL_HANDLE);
	}

	void CRendererShader::AddMacroDefinition(const char *szName, const char *szValue)
	{
		m_strMacroDefinitions[szName] = szValue;
	}

	void CRendererShader::DelMacroDefinition(const char *szName)
	{
		std::map<std::string, std::string>::const_iterator itMacroDefinition = m_strMacroDefinitions.find(szName);
		if (itMacroDefinition != m_strMacroDefinitions.end()) m_strMacroDefinitions.erase(itMacroDefinition);
	}

	void CRendererShader::ClearMacroDefinitions(void)
	{
		m_strMacroDefinitions.clear();
	}

	BOOL CRendererShader::Create(const char *szSource, size_t length, shaderc_shader_kind kind)
	{
		shaderc::CompileOptions options = CreateCompileOptions();
		std::vector<uint32_t> words = CompileShader(szSource, length, kind, options);
		return Create(words.data(), words.size());
	}

	shaderc::CompileOptions CRendererShader::CreateCompileOptions(void)
	{
		shaderc::CompileOptions options(((CRendererShaderManager *)m_pManager)->GetCompileOptions());

		for (std::map<std::string, std::string>::const_iterator itMacroDefinition = m_strMacroDefinitions.begin(); itMacroDefinition != m_strMacroDefinitions.end(); ++itMacroDefinition) {
			if (itMacroDefinition->second.empty()) {
				options.AddMacroDefinition(itMacroDefinition->first.c_str());
			}
			else {
				options.AddMacroDefinition(itMacroDefinition->first.c_str(), itMacroDefinition->second.c_str());
			}
		}

		return options;
	}

	BOOL CRendererShader::Create(const uint32_t *words, size_t numWords)
	{
		try {
			VkShaderModuleCreateInfo moduleCreateInfo;
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = numWords * 4;
			moduleCreateInfo.pCode = words;
			CALL_VK_FUNCTION_THROW(vkCreateShaderModule(m_pDevice->GetDevice(), &moduleCreateInfo, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks(), &m_vkShaderModule));

			m_module = spirv::parse(words, numWords);

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRendererShader::Destroy(void)
	{
		if (m_vkShaderModule) {
			vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShaderModule, m_pDevice->GetRenderer()->GetAllocator()->GetAllocationCallbacks());
		}

		m_vkShaderModule = VK_NULL_HANDLE;
	}

	void CRendererShader::DumpLog(void) const
	{
		if (m_vkShaderModule) {
			LOGI("\t\tShader 0x%x\n", m_vkShaderModule);
		}
	}

	VkShaderModule CRendererShader::GetShaderModule(void) const
	{
		return m_vkShaderModule;
	}

	const spirv::module_type& CRendererShader::GetModule(void) const
	{
		return m_module;
	}

}
