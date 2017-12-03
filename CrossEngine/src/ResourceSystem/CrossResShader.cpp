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

	CResShader::CResShader(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		m_ptrShader = GfxDevice()->NewShader();
	}

	CResShader::~CResShader(void)
	{
		m_ptrShader.Release();
	}

	const CGfxShaderPtr& CResShader::GetShader(void) const
	{
		return m_ptrShader;
	}

	RESOURCE_TYPE CResShader::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_SHADER;
	}

	BOOL CResShader::Load(BOOL bSync)
	{
		if (IsLoaded()) {
			return TRUE;
		}

		char szExt[_MAX_EXT];
		splitfilename(m_stream.GetName(), NULL, szExt);

		if      (!stricmp(szExt, ".vert")) m_flags = VK_SHADER_STAGE_VERTEX_BIT;
		else if (!stricmp(szExt, ".frag")) m_flags = VK_SHADER_STAGE_FRAGMENT_BIT;
		else if (!stricmp(szExt, ".comp")) m_flags = VK_SHADER_STAGE_COMPUTE_BIT;
		else return FALSE;

		return m_ptrShader->Precompile((const char *)m_stream.GetAddress(), m_stream.GetFullSize(), m_flags, m_words);
	}

	BOOL CResShader::PostLoad(void)
	{
		if (IsLoaded()) {
			return TRUE;
		}

		BOOL rcode = m_ptrShader->Create(m_words.data(), m_words.size(), m_flags);

		m_stream.Free();
		m_words.clear();
		m_bIsLoaded = TRUE;

		return rcode;
	}

	BOOL CResShader::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrShader->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResShader::IsLoaded(void) const
	{
		return m_bIsLoaded;
	}

}
