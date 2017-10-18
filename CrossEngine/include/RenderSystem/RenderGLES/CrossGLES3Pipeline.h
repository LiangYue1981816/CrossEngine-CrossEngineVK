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

	class CROSS_EXPORT CGLES3DescriptorSetLayout
	{
		friend class CGLES3Pipeline;


	protected:
		CGLES3DescriptorSetLayout(uint32_t set);
		virtual ~CGLES3DescriptorSetLayout(void);


	protected:
		BOOL SetUniformBlockBinding(const char *szName, uint32_t binding, GLuint program);
		BOOL SetSampledImageBinding(const char *szName, uint32_t binding, GLuint program);

	public:
		uint32_t GetSet(void) const;
		uint32_t GetBinding(const char *szName) const;

		const std::map<uint32_t, std::map<uint32_t, uint32_t>>& GetUniformBlockBindings(void) const;
		const std::map<uint32_t, std::map<uint32_t, uint32_t>>& GetSampledImageBindings(void) const;


	protected:
		uint32_t m_set;
		std::map<uint32_t, uint32_t> m_names;
		std::map<uint32_t, std::map<uint32_t, uint32_t>> m_uniformBlockBindings;
		std::map<uint32_t, std::map<uint32_t, uint32_t>> m_sampledImageBindings;
	};

	class CROSS_EXPORT CGLES3Pipeline
	{
	protected:
		CGLES3Pipeline(CGLES3Device *pDevice);
		virtual ~CGLES3Pipeline(void);


	protected:
		BOOL CreateDescriptorSetLayouts(void);
		void DestroyDescriptorSetLayouts(void);

	public:
		const CGLES3DescriptorSetLayout* GetDescriptorSetLayout(uint32_t set) const;


	protected:
		GLuint m_pipeline;
		std::map<VkShaderStageFlagBits, CGfxShaderPtr> m_ptrShaders;
		std::map<uint32_t, CGLES3DescriptorSetLayout*> m_pDescriptorSetLayouts;

	protected:
		CGLES3Device *m_pDevice;
	};

}
