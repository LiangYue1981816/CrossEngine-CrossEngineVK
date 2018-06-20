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

	class CROSS_EXPORT CGfxDescriptorSetLayout
	{
		friend class CGfxDescriptorSetLayoutPtr;


	protected:
		CGfxDescriptorSetLayout(void)
		{

		}
		virtual ~CGfxDescriptorSetLayout(void)
		{

		}

		virtual void Release(void) = 0;


	public:
		virtual HANDLE GetHandle(void) const = 0;

	public:
		virtual BOOL Create(void) = 0;
		virtual void Destroy(void) = 0;

	public:
		virtual BOOL SetUniformBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags) = 0;
		virtual BOOL SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags) = 0;
		virtual BOOL SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags) = 0;

	public:
		virtual const uint32_t GetSet(void) const = 0;
		virtual const uint32_t GetBinding(uint32_t dwName) const = 0;
		virtual const uint32_t* GetTypesUsedCount(void) const = 0;

	public:
		virtual BOOL IsCompatible(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics) const = 0;
	};

	class CROSS_EXPORT CGfxDescriptorSetLayoutPtr : public CSharedPtr<CGfxDescriptorSetLayout>
	{
	public:
		CGfxDescriptorSetLayoutPtr(void) : CSharedPtr<CGfxDescriptorSetLayout>()
		{

		}
		CGfxDescriptorSetLayoutPtr(const CGfxDescriptorSetLayout *p) : CSharedPtr<CGfxDescriptorSetLayout>(p)
		{

		}
		CGfxDescriptorSetLayoutPtr(const CGfxDescriptorSetLayoutPtr &ptr) : CSharedPtr<CGfxDescriptorSetLayout>(ptr)
		{

		}
		virtual ~CGfxDescriptorSetLayoutPtr(void)
		{
			Release();
		}


	protected:
		virtual void FreePointer(void)
		{
			if (m_pPointer) {
				m_pPointer->Destroy();
				m_pPointer->Release();
			}
		}
	};

}
