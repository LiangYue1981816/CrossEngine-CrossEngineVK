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

	typedef enum VertexAttributeFlagBits {
		VERTEX_ATTRIBUTE_FLAG_POSITION = 0x00000001,
		VERTEX_ATTRIBUTE_FLAG_NORMAL = 0x00000002,
		VERTEX_ATTRIBUTE_FLAG_BINORMAL = 0x00000004,
		VERTEX_ATTRIBUTE_FLAG_COLOR = 0x00000008,
		VERTEX_ATTRIBUTE_FLAG_SKIN_INDEX = 0x00000010,
		VERTEX_ATTRIBUTE_FLAG_SKIN_WEIGHT = 0x00000020,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD0 = 0x00000040,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD1 = 0x00000080,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD2 = 0x00000100,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD3 = 0x00000200,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD4 = 0x00000400,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD5 = 0x00000800,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD6 = 0x00001000,
		VERTEX_ATTRIBUTE_FLAG_TEXCOORD7 = 0x00002000,
		VERTEX_ATTRIBUTE_FLAG_COUNT = 14
	} VertexAttributeFlagBits;


	class CROSS_EXPORT CGfxDevice
	{
	protected:
		CGfxDevice(void)
		{

		}
		virtual ~CGfxDevice(void)
		{

		}


	public:
		uint32_t GetVertexStride(uint32_t format) const;
		uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute) const;
		uint32_t GetVertexAttributeFlag(const char *szName) const;
		uint32_t GetVertexAttributeSize(uint32_t attribute) const;
		uint32_t GetVertexAttributeLocation(uint32_t attribute) const;
		VkFormat GetVertexAttributeFormat(uint32_t attribute) const;

	public:
		virtual CGfxInstance* GetInstance(void) const = 0;
		virtual CGfxQueue* GetComputeQueue(void) const = 0;
		virtual CGfxQueue* GetGraphicsQueue(void) const = 0;
		virtual CGfxQueue* GetTransferQueue(void) const = 0;

	public:
		virtual CGfxCommandBufferPtr AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level) = 0;
		virtual CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineComputePtr &ptrPipeline) = 0;
		virtual CGfxDescriptorSetPtr AllocDescriptorSet(uint32_t pool, uint32_t set, const CGfxPipelineGraphicsPtr &ptrPipeline) = 0;

		virtual CGfxIndexBufferPtr NewIndexBuffer(void) = 0;
		virtual CGfxVertexBufferPtr NewVertexBuffer(void) = 0;
		virtual CGfxUniformBufferPtr NewUniformBuffer(void) = 0;
		virtual CGfxTexturePtr NewTexture(void) = 0;
		virtual CGfxRenderTexturePtr NewRenderTexture(void) = 0;
		virtual CGfxShaderPtr NewShader(void) = 0;
		virtual CGfxPipelineComputePtr NewPipelineCompute(void) = 0;
		virtual CGfxPipelineGraphicsPtr NewPipelineGraphics(void) = 0;
		virtual CGfxMaterialPtr NewMaterial(void) = 0;
		virtual CGfxRenderPassPtr NewRenderPass(void) = 0;
		virtual CGfxFrameBufferPtr NewFrameBuffer(void) = 0;

	public:
		virtual void SetShaderCachePath(const char *szPath) = 0;
		virtual void AddShaderIncludePath(const char *szPath) = 0;
		virtual void AddShaderMacroDefinition(const char *szName) = 0;
		virtual void AddShaderMacroDefinition(const char *szName, const char *szValue) = 0;

	public:
		virtual int WaitIdle(void) const = 0;

	public:
		virtual void DumpLog(void) const = 0;
	};

}
