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

	class CROSS_EXPORT CGLES3ShaderManager : public CGfxResourceManager
	{
		friend class CGLES3Device;
		friend class CGLES3Shader;


	protected:
		CGLES3ShaderManager(CGLES3Device *pDevice);
		virtual ~CGLES3ShaderManager(void);


	protected:
		CGfxShaderPtr AllocShader(void);

		const char* GetCachePath(void) const;
		const shaderc::Compiler& GetCompiler(void) const;
		const shaderc::CompileOptions& GetCompileOptions(void) const;

	protected:
		void SetCachePath(const char *szPath);
		void AddIncludePath(const char *szPath);
		void AddMacroDefinition(const char *szName);
		void AddMacroDefinition(const char *szName, const char *szValue);
		BOOL Precompile(const char *szSource, size_t length, VkShaderStageFlagBits flags, std::vector<uint32_t> &words);


	protected:
		char m_szCachePath[_MAX_STRING];

		shaderc_util::FileFinder m_fileFinder;
		std::unique_ptr<glslc::FileIncluder> m_fileIncluder;

		shaderc::Compiler m_compiler;
		shaderc::CompileOptions m_options;
		std::vector<std::string> m_strMacroDefinitions;

	protected:
		CGLES3Device *m_pDevice;
	};

}
