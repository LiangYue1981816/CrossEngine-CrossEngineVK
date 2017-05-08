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
	#include <direct.h>

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


#define GLM_FORCE_DEPTH_ZERO_TO_ONE


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
#include <glm/gtc/matrix_transform.hpp>
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
#include "CrossSharedPtr.h"

#include "CrossVulkan.h"
#include "CrossVulkanHelper.h"
#include "CrossVulkanAllocator.h"
#include "CrossVulkanSwapchain.h"
#include "CrossVulkanQueue.h"
#include "CrossVulkanDevice.h"
#include "CrossVulkanDeviceCompute.h"
#include "CrossVulkanDeviceGraphics.h"
#include "CrossVulkanCommandPool.h"
#include "CrossVulkanCommandBuffer.h"
#include "CrossVulkanCommandBufferManager.h"
#include "CrossVulkanDescriptorPool.h"
#include "CrossVulkanDescriptorSet.h"
#include "CrossVulkanDescriptorSetManager.h"
#include "CrossVulkanMemory.h"
#include "CrossVulkanMemoryAllocator.h"
#include "CrossVulkanMemoryManager.h"
#include "CrossVulkanStagingBuffer.h"
#include "CrossVulkanStagingBufferManager.h"

#include "CrossVulkanResource.h"
#include "CrossVulkanResourceManager.h"
#include "CrossVulkanFence.h"
#include "CrossVulkanFenceManager.h"
#include "CrossVulkanSemaphore.h"
#include "CrossVulkanSemaphoreManager.h"
#include "CrossVulkanBuffer.h"
#include "CrossVulkanIndexBuffer.h"
#include "CrossVulkanVertexBuffer.h"
#include "CrossVulkanUniformBuffer.h"
#include "CrossVulkanBufferManager.h"
#include "CrossVulkanSampler.h"
#include "CrossVulkanSamplerManager.h"
#include "CrossVulkanImage.h"
#include "CrossVulkanTexture.h"
#include "CrossVulkanRenderTexture.h"
#include "CrossVulkanTextureManager.h"
#include "CrossVulkanShader.h"
#include "CrossVulkanShaderManager.h"
#include "CrossVulkanRenderPass.h"
#include "CrossVulkanRenderPassManager.h"
#include "CrossVulkanFrameBuffer.h"
#include "CrossVulkanFrameBufferManager.h"
#include "CrossVulkanDescriptorLayout.h"
#include "CrossVulkanDescriptorLayoutManager.h"
#include "CrossVulkanPipeline.h"
#include "CrossVulkanPipelineCompute.h"
#include "CrossVulkanPipelineGraphics.h"
#include "CrossVulkanPipelineManager.h"

#include "CrossResource.h"
#include "CrossResourceManager.h"
#include "CrossResourceSystem.h"


#define TAG_NAME "CrossEngine"


#define Engine()             CrossEngine::CEngine::GetEngine()

#define ResourceSystem()     CrossEngine::CEngine::GetEngine()->GetResourceSystem()
#define ShaderManager()      CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::SHADER)
#define TextureManager()     CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::TEXTURE)
#define MaterialManager()    CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::MATERIAL)
#define MeshManager()        CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::MESH)
#define SkeletonManager()    CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::SKELETON)
#define EffectManager()      CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::EFFECT)
#define SoundManager()       CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::RESOURCE_TYPE::SOUND)


namespace CrossEngine {

	class CROSS_EXPORT CEngine
	{
	protected:
		CEngine(void);
		virtual ~CEngine(void);


	public:
		static CEngine* GetEngine(void);

	public:
		CResourceSystem* GetResourceSystem(void) const;


	public:
		void Init(HINSTANCE hInstance, HWND hWnd, RECT rcView, DWORD dwSoundMemPoolSize, INT maxChannels, const CHAR *szLogFileName, BOOL bEditorMode);
		void Exit(void);


	protected:
		CResourceSystem *m_pResourceSystem;

	protected:
		static CEngine sInstance;
	};

}
