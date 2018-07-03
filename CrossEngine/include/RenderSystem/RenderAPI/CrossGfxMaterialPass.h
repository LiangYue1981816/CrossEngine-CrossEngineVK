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

	class CROSS_EXPORT CGfxMaterialPass : public CGfxResource
	{
		friend class CGfxMaterial;


	protected:
		CGfxMaterialPass(void)
		{

		}
		virtual ~CGfxMaterialPass(void)
		{

		}


	public:
		virtual const uint32_t GetIndexSubPass(void) const = 0;
		virtual const CGfxRenderPassPtr& GetRenderPass(void) const = 0;
		virtual const CGfxPipelineGraphicsPtr& GetPipeline(void) const = 0;
		virtual const CGfxDescriptorSetPtr& GetDescriptorSet(void) const = 0;

	public:
		virtual void SetRenderPass(const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass) = 0;
		virtual void SetPipeline(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics) = 0;

		virtual void SetTexture(uint32_t dwName, const CGfxTexturePtr &ptrTexture) = 0;
		virtual void SetUniformBuffer(uint32_t dwName, const CGfxUniformBufferPtr &ptrUniformBuffer) = 0;
		virtual void UpdateDescriptorSet(void) = 0;
	};

}
