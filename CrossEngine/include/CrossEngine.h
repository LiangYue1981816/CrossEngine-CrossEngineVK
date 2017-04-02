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


#if defined _PLATFORM_WINDOWS_

	#include <winsock2.h>
	#include <windows.h>

#elif defined _PLATFORM_ANDROID_

	#include <dirent.h>
	#include <unistd.h>
	#include <fnmatch.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>

	#include <jni.h>
	#include <string.h>
	#include <android/log.h>

#elif defined _PLATFORM_IOS_

	#include <dirent.h>
	#include <unistd.h>
	#include <fnmatch.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>

#else

	#pragma message("Not support platform !!!")

#endif


#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <rbtree.h>
#include <pthread.h>
#include <semaphore.h>
#include <glm/glm.hpp>
#include <gli/gli.hpp>
#include <zzip/zzip.h>
#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>
#include <reflection/analyzer.h>


#include "CrossType.h"
#include "CrossMemory.h"
#include "CrossDelegate.h"
#include "CrossC.h"
#include "CrossUtility.h"
#include "CrossLog.h"
#include "CrossStream.h"

#include "CrossRenderer.h"
#include "CrossRendererHelper.h"
#include "CrossRendererVertex.h"
#include "CrossRendererAllocator.h"
#include "CrossRendererSwapchain.h"
#include "CrossRendererQueue.h"
#include "CrossRendererDevice.h"
#include "CrossRendererDeviceCompute.h"
#include "CrossRendererDeviceGraphics.h"
#include "CrossRendererCommandPool.h"
#include "CrossRendererCommandBuffer.h"
#include "CrossRendererCommandBufferManager.h"
#include "CrossRendererDescriptorPool.h"
#include "CrossRendererDescriptorSet.h"
#include "CrossRendererDescriptorSetManager.h"
#include "CrossRendererMemory.h"
#include "CrossRendererMemoryAllocator.h"
#include "CrossRendererMemoryManager.h"
#include "CrossRendererStagingBuffer.h"
#include "CrossRendererStagingBufferManager.h"

#include "CrossRendererResource.h"
#include "CrossRendererResourceManager.h"
#include "CrossRendererFence.h"
#include "CrossRendererFenceManager.h"
#include "CrossRendererBuffer.h"
#include "CrossRendererIndexBuffer.h"
#include "CrossRendererVertexBuffer.h"
#include "CrossRendererUniformBuffer.h"
#include "CrossRendererBufferManager.h"
#include "CrossRendererImage.h"
#include "CrossRendererTexture.h"
#include "CrossRendererRenderTexture.h"
#include "CrossRendererTextureManager.h"
#include "CrossRendererSampler.h"
#include "CrossRendererSamplerManager.h"
#include "CrossRendererShader.h"
#include "CrossRendererShaderManager.h"
#include "CrossRendererRenderPass.h"
#include "CrossRendererRenderPassManager.h"
#include "CrossRendererFrameBuffer.h"
#include "CrossRendererFrameBufferManager.h"
#include "CrossRendererDescriptorLayout.h"
#include "CrossRendererDescriptorLayoutManager.h"
#include "CrossRendererPipeline.h"
#include "CrossRendererPipelineCompute.h"
#include "CrossRendererPipelineGraphics.h"
#include "CrossRendererPipelineManager.h"


#define TAG_NAME "CrossEngine"


namespace CrossEngine {

}
