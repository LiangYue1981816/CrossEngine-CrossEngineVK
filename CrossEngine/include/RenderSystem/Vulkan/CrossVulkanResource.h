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

	class CROSS_EXPORT CVulkanResource
	{
		friend class CVulkanResourceManager;


	protected:
		CVulkanResource(CVulkanDevice *pDevice, CVulkanResourceManager *pManager);
		virtual ~CVulkanResource(void);


	public:
		virtual void Release(void);
		virtual void Destroy(void) {}
		virtual void DumpLog(void) const {}


	protected:
		CVulkanDevice *m_pDevice;
		CVulkanResourceManager *m_pManager;
	};

	class CROSS_EXPORT CVulkanResourcePtr : public CSharedPtr<CVulkanResource>
	{
	public:
		CVulkanResourcePtr(void) : CSharedPtr<CVulkanResource>() {}
		CVulkanResourcePtr(const CVulkanResource *pPointer) : CSharedPtr<CVulkanResource>(pPointer) {}
		CVulkanResourcePtr(const CVulkanResourcePtr &ptr) : CSharedPtr<CVulkanResource>(ptr) {}
		virtual ~CVulkanResourcePtr(void) {}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				m_pPointer->Release();
			}
		}
	};

}
