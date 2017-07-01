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

	typedef struct {
		GLuint texture;
		CGfxRenderTexturePtr ptrRenderTexture;
	} GLAttachmentInformation;

	class CROSS_EXPORT CGLES3Instance;
	class CROSS_EXPORT CGLES3Queue;
	class CROSS_EXPORT CGLES3Device;
	class CROSS_EXPORT CGLES3DeviceFeatures;
	class CROSS_EXPORT CGLES3DeviceProperties;
	class CROSS_EXPORT CGLES3Swapchain;
	class CROSS_EXPORT CGLES3CommandBuffer;
	class CROSS_EXPORT CGLES3Buffer;
	class CROSS_EXPORT CGLES3IndexBuffer;
	class CROSS_EXPORT CGLES3VertexBuffer;
	class CROSS_EXPORT CGLES3UniformBuffer;
	class CROSS_EXPORT CGLES3BufferManager;
	class CROSS_EXPORT CGLES3Image;
	class CROSS_EXPORT CGLES3Texture;
	class CROSS_EXPORT CGLES3RenderTexture;
	class CROSS_EXPORT CGLES3TextureManager;
	class CROSS_EXPORT CGLES3FrameBuffer;

}
