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

	class CROSS_EXPORT CRenderResource
	{
		friend class CRenderResourceManager;
		template<class T> friend class CRenderResourcePtr;


	protected:
		CRenderResource(CRenderDevice *pDevice, CRenderResourceManager *pResourceManager);
		virtual ~CRenderResource(void);


	protected:
		virtual CRenderResourceManager* GetResourceManager(void) const;

	public:
		virtual void Destroy(void) {}
		virtual void DumpLog(void) const {}


	protected:
		CRenderDevice *m_pDevice;
		CRenderResourceManager *m_pResourceManager;
	};

	template<class T>
	class CROSS_EXPORT CRenderResourcePtr : public CSharedPtr<T>
	{
	public:
		CRenderResourcePtr(void) : CSharedPtr<T>() {}
		CRenderResourcePtr(const T *p) : CSharedPtr<T>(p) {}
		CRenderResourcePtr(const CRenderResourcePtr &ptr) : CSharedPtr<T>(ptr) {}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				((CRenderResource *)m_pPointer)->GetResourceManager()->Free((CRenderResource *)m_pPointer);
			}
		}
	};

}
