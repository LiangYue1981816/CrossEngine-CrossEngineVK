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

	class CROSS_EXPORT CGfxInstance;
	class CROSS_EXPORT CGfxQueue;
	class CROSS_EXPORT CGfxDevice;
	class CROSS_EXPORT CGfxSwapchain;

	template<class T>
	class CROSS_EXPORT CGfxResourcePtr;
	class CROSS_EXPORT CGfxResource;
	class CROSS_EXPORT CGfxResourceManager;
	class CROSS_EXPORT CGfxBuffer;
	class CROSS_EXPORT CGfxIndexBuffer;
	class CROSS_EXPORT CGfxVertexBuffer;
	class CROSS_EXPORT CGfxUniformBuffer;
	class CROSS_EXPORT CGfxImage;
	class CROSS_EXPORT CGfxTexture;
	class CROSS_EXPORT CGfxRenderTexture;
	class CROSS_EXPORT CGfxShader;
	class CROSS_EXPORT CGfxPipelineCompute;
	class CROSS_EXPORT CGfxPipelineGraphics;
	class CROSS_EXPORT CGfxRenderPass;
	class CROSS_EXPORT CGfxFrameBuffer;
	class CROSS_EXPORT CGfxDescriptorSet;
	class CROSS_EXPORT CGfxCommandBuffer;
	class CROSS_EXPORT CGfxDescriptorSetPtr;
	class CROSS_EXPORT CGfxCommandBufferPtr;

	typedef CGfxResourcePtr<CGfxIndexBuffer> CGfxIndexBufferPtr;
	typedef CGfxResourcePtr<CGfxVertexBuffer> CGfxVertexBufferPtr;
	typedef CGfxResourcePtr<CGfxUniformBuffer> CGfxUniformBufferPtr;
	typedef CGfxResourcePtr<CGfxTexture> CGfxTexturePtr;
	typedef CGfxResourcePtr<CGfxRenderTexture> CGfxRenderTexturePtr;
	typedef CGfxResourcePtr<CGfxShader> CGfxShaderPtr;
	typedef CGfxResourcePtr<CGfxPipelineCompute> CGfxPipelineComputePtr;
	typedef CGfxResourcePtr<CGfxPipelineGraphics> CGfxPipelineGraphicsPtr;
	typedef CGfxResourcePtr<CGfxRenderPass> CGfxRenderPassPtr;
	typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;

	typedef void* CGfxFence;
	typedef void* CGfxSemaphore;

}
