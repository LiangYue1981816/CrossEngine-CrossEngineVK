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

	CResCompute::CResCompute(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResCompute::~CResCompute(void)
	{

	}

	const CGfxPipelineComputePtr& CResCompute::GetPipeline(void) const
	{
		return m_ptrPipeline;
	}

	RESOURCE_TYPE CResCompute::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_COMPUTE;
	}

	BOOL CResCompute::Load(BOOL bSync)
	{
		if (IsLoaded()) {
			return TRUE;
		}

		// ...

		return TRUE;
	}

	BOOL CResCompute::PostLoad(void)
	{
		if (IsLoaded()) {
			return TRUE;
		}

		m_stream.Free();
		m_bIsLoaded = TRUE;

		// ...

		return TRUE;
	}

	BOOL CResCompute::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrPipeline->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResCompute::IsLoaded(void) const
	{
		return m_bIsLoaded;
	}

}
