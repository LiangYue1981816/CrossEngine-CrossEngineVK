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

	class CROSS_EXPORT CRendererShaderManager : public CRendererResourceManager
	{
		friend class CRendererDevice;


	protected:
		CRendererShaderManager(CRendererDevice *pDevice);
		virtual ~CRendererShaderManager(void);


	public:
		CRendererShader* AllocShader(void);

	public:
		void SetSourceLanguage(shaderc_source_language lang);
		void SetOptimizationLevel(shaderc_optimization_level level);
		void SetTargetEnvironment(uint32_t version, shaderc_target_env target);
		void SetForcedVersionProfile(uint32_t version, shaderc_profile profile);
		void SetMacroDefinition(const char *name, const char *value);

		const shaderc::CompileOptions& GetCompileOptions(void) const;


	protected:
		shaderc::CompileOptions m_options;
	};

}
