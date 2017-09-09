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

	}

	CShader::~CShader(void)
	{

	}

	const CGfxShaderPtr& CShader::GetShader(void) const
	{
		return m_ptrShader;
	}

	BOOL CShader::IsValid(void) const
	{
		return m_ptrShader.IsNull() ? FALSE : TRUE;
	}

	RESOURCE_TYPE CShader::GetType(void) const
	{
		return SHADER;
	}

	BOOL CShader::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL CShader::LoadFromFile(const char *szFileName)
	{
		BOOL rcode = CResource::LoadFromFile(szFileName);
		{
			m_stream.Free();
		}
		return rcode;
	}

	BOOL CShader::LoadFromPack(const char *szPackName, const char *szFileName)
	{
		BOOL rcode = CResource::LoadFromPack(szPackName, szFileName);
		{
			m_stream.Free();
		}
		return rcode;
	}

	BOOL CShader::LoadFromPack(ZZIP_DIR *pPack, const char *szFileName)
	{
		BOOL rcode = CResource::LoadFromPack(pPack, szFileName);
		{
			m_stream.Free();
		}
		return rcode;
	}

	BOOL CShader::LoadFromStream(CStream *pStream)
	{
		char szExt[_MAX_EXT];
		splitfilename(pStream->GetName(), NULL, szExt);

		VkShaderStageFlagBits shaderFlags;
		if      (!stricmp(szExt, ".vert")) shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
		else if (!stricmp(szExt, ".frag")) shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		else if (!stricmp(szExt, ".comp")) shaderFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		else return FALSE;

		m_ptrShader = GfxDevice()->NewShader();
		return m_ptrShader->Create((const char *)pStream->GetAddress(), pStream->GetFullSize(), shaderFlags);
	}

}
