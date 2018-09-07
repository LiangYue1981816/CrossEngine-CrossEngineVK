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

	class CROSS_EXPORT CGfxDescriptorSet
	{
		friend class CGfxDescriptorSetPtr;


	protected:
		CGfxDescriptorSet(void)
		{

		}
		virtual ~CGfxDescriptorSet(void)
		{

		}

		virtual void Release(void) = 0;


	public:
		virtual HANDLE GetHandle(void) const = 0;
		virtual const CGfxDescriptorSetLayoutPtr& GetDescriptorSetLayoutPtr(void) const = 0;

	public:
		virtual void SetTexture(uint32_t binding, const CGfxTexturePtr &ptrTexture) = 0;
		virtual void SetRenderTexture(uint32_t binding, const CGfxRenderTexturePtr &ptrRenderTexture) = 0;
		virtual void SetUniformBuffer(uint32_t binding, const CGfxUniformBufferPtr &ptrUniformBuffer) = 0;
		virtual void UpdateDescriptorSets(void) = 0;

	public:
		virtual void SetUniformBufferData(uint32_t binding, size_t offset, size_t size, const void *pBuffer) = 0;
	};

	class CROSS_EXPORT CGfxDescriptorSetPtr : public CSharedPtr<CGfxDescriptorSet>
	{
	public:
		CGfxDescriptorSetPtr(void) : CSharedPtr<CGfxDescriptorSet>()
		{

		}
		CGfxDescriptorSetPtr(const CGfxDescriptorSet *p) : CSharedPtr<CGfxDescriptorSet>(p)
		{

		}
		CGfxDescriptorSetPtr(const CGfxDescriptorSetPtr &ptr) : CSharedPtr<CGfxDescriptorSet>(ptr)
		{

		}
		virtual ~CGfxDescriptorSetPtr(void)
		{

		}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				m_pPointer->Release();
			}
		}
	};

}
