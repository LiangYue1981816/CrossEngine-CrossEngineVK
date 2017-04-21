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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CRendererShader : public CRendererResource
	{
		friend class CRendererShaderManager;


	protected:
		CRendererShader(CRendererDevice *pDevice, CRendererResourceManager *pManager);
		virtual ~CRendererShader(void);


	public:
		void AddMacroDefinition(const char *szName, const char *szValue);
		void DelMacroDefinition(const char *szName);
		void ClearMacroDefinitions(void);

	public:
		virtual BOOL Create(const char *szSource, size_t length, shaderc_shader_kind kind);
		virtual BOOL Create(const uint32_t *words, size_t numWords);
		virtual void Destroy(void);
		virtual void DumpLog(void) const;

	protected:
		shaderc::CompileOptions CreateCompileOptions(void);

	public:
		VkShaderModule GetShaderModule(void) const;
		const spirv::module_type& GetModule(void) const;


	protected:
		std::map<std::string, std::string> m_strMacroDefinitions;

	protected:
		spirv::module_type m_module;
		VkShaderModule m_vkShaderModule;
	};

}
