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

	CShader::CShader(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		m_ptrShader = GfxDevice()->NewShader();
	}

	CShader::~CShader(void)
	{
		m_ptrShader.Release();
	}

	RESOURCE_TYPE CShader::GetType(void) const
	{
		return RESOURCE_TYPE_SHADER;
	}

	const CGfxShaderPtr& CShader::GetShader(void) const
	{
		return m_ptrShader;
	}

	VkShaderStageFlagBits CShader::GetShaderFlags(void) const
	{
		return m_shaderFlags;
	}

	BOOL CShader::Load(void)
	{
		char szExt[_MAX_EXT];
		splitfilename(m_stream.GetName(), NULL, szExt);

		if      (!stricmp(szExt, ".vert")) m_shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
		else if (!stricmp(szExt, ".frag")) m_shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		else if (!stricmp(szExt, ".comp")) m_shaderFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		else return FALSE;

		return m_ptrShader->Precompile((const char *)m_stream.GetAddress(), m_stream.GetFullSize(), m_shaderFlags);
	}

	BOOL CShader::LoadPost(void)
	{
		BOOL rcode = m_ptrShader->Create((const char *)m_stream.GetAddress(), m_stream.GetFullSize(), m_shaderFlags);
		m_stream.Free();

		return rcode;
	}

}
