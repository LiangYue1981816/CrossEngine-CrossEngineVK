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

	CGLES3PipelineCompute::CGLES3PipelineCompute(CGLES3Device *pDevice, CGfxResourceManager *pResourceManager)
		: CGfxPipelineCompute(pResourceManager)
		, CGLES3Pipeline(pDevice)
	{

	}

	CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
	{

	}

	CGLES3Device* CGLES3PipelineCompute::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CGLES3PipelineCompute::GetHandle(void) const
	{
		return (HANDLE)m_pipeline;
	}

	BOOL CGLES3PipelineCompute::Create(const CGfxShaderPtr &ptrShader)
	{
		if (ptrShader.IsNull()) {
			return FALSE;
		}

		m_ptrShaders[VK_SHADER_STAGE_COMPUTE_BIT] = ptrShader;

		glGenProgramPipelines(1, &m_pipeline);
		glBindProgramPipeline(m_pipeline);
		{
			glUseProgramStages(m_pipeline, GL_COMPUTE_SHADER_BIT, (GLuint)m_ptrShaders[VK_SHADER_STAGE_COMPUTE_BIT]->GetHandle());
		}
		glBindProgramPipeline(0);

		return CreateDescriptorSetLayouts();
	}

	void CGLES3PipelineCompute::Destroy(void)
	{
		DestroyDescriptorSetLayouts();
		glDeleteProgramPipelines(1, &m_pipeline);
		m_pipeline = 0;
	}

	void CGLES3PipelineCompute::DumpLog(void) const
	{
		if (m_pipeline) {
			LOGI("\t\tPipelineCompute 0x%x\n", m_pipeline);
		}
	}

}
