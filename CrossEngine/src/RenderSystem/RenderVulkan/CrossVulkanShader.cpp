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

	BOOL CVulkanShader::Create(const char *szSource, size_t length, VkShaderStageFlagBits flags)
	{
		std::vector<uint32_t> words;

		if (((CVulkanShaderManager *)m_pResourceManager)->Precompile(szSource, length, flags, words) == FALSE) {
			return FALSE;
		}

		return Create(words.data(), words.size(), flags);
	}

	BOOL CVulkanShader::Create(const uint32_t *words, size_t numWords, VkShaderStageFlagBits flags)
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
