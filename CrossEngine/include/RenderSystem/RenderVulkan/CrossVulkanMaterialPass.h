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

	class CROSS_EXPORT CVulkanMaterialPass : public CGfxMaterialPass
	{
		friend class CVulkanMaterial;


	protected:
		CVulkanMaterialPass(CVulkanDevice *pDevice);
		virtual ~CVulkanMaterialPass(void);


	public:
		CVulkanDevice* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		void Destroy(void);
		void DumpLog(void) const;

	public:
		const uint32_t GetIndexSubPass(void) const;
		const CGfxRenderPassPtr& GetRenderPass(void) const;
		const CGfxPipelineGraphicsPtr& GetPipeline(void) const;
		const CGfxDescriptorSetPtr& GetDescriptorSet(void) const;

	public:
		void SetRenderPass(const CGfxRenderPassPtr &ptrRenderPass, uint32_t indexSubPass);
		void SetPipeline(const CGfxPipelineGraphicsPtr &ptrPipeline);

		void SetTexture(uint32_t dwName, const CGfxTexturePtr &ptrTexture);
		void SetUniformBuffer(uint32_t dwName, const CGfxUniformBufferPtr &ptrUniformBuffer);
		void UpdateDescriptorSet(void);


	protected:
		uint32_t m_indexSubPass;
		CGfxRenderPassPtr m_ptrRenderPass;
		CGfxPipelineGraphicsPtr m_ptrPipeline;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;

	protected:
		std::map<uint32_t, CGfxTexturePtr> m_ptrTextures;
		std::map<uint32_t, CGfxUniformBufferPtr> m_ptrUniformBuffers;

	protected:
		CVulkanDevice *m_pDevice;
	};

}
