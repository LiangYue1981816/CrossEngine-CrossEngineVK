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

	RESOURCE_TYPE CResCompute::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_COMPUTE;
	}

	const CGfxPipelineComputePtr& CResCompute::GetPipeline(void) const
	{
		return m_ptrPipelineCompute;
	}

	BOOL CResCompute::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrPipelineCompute.IsNull() || m_ptrPipelineCompute->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResCompute::InternalLoad(BOOL bSyncPostLoad)
	{
		// ...

		return TRUE;
	}

	BOOL CResCompute::InternalPostLoad(void)
	{
		// ...

		return TRUE;
	}

	void CResCompute::InternalLoadFail(void)
	{
		m_ptrPipelineCompute.Release();
		CResource::InternalLoadFail();
	}

	void CResCompute::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

}
