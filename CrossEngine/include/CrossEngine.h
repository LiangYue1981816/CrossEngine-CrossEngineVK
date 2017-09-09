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


#ifdef PLATFORM_WINDOWS

	#include <winsock2.h>
	#include <windows.h>
	#include <direct.h>

#elif PLATFORM_ANDROID

	#include <dirent.h>
	#include <unistd.h>
	#include <fnmatch.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>

	#include <jni.h>
	#include <string.h>
	#include <android/log.h>

#elif PLATFORM_IOS

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
#include <event.h>
#include <semaphore.h>
#include <tinyxml.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gli/gli.hpp>
#include <zzip/zzip.h>
#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>
#include <glslc/file_includer.h>
#include <spirv_cfg.hpp>
#include <spirv_cpp.hpp>
#include <spirv_msl.hpp>
#include <spirv_glsl.hpp>
#include <spirv_cross.hpp>


#include "CrossType.h"
#include "CrossMutex.h"
#include "CrossMemory.h"
#include "CrossDelegate.h"
#include "CrossC.h"
#include "CrossUtility.h"
#include "CrossLog.h"
#include "CrossStream.h"
#include "CrossSharedPtr.h"
#include "CrossTaskGraph.h"

#include "CrossGfxAPI.h"
#include "CrossGLES3API.h"
#include "CrossVulkanAPI.h"
#include "CrossRenderSystem.h"

#include "CrossResource.h"
#include "CrossResourceManager.h"
#include "CrossResourceSystem.h"
#include "CrossShader.h"
#include "CrossShaderManager.h"

#include "CrossEntityBase.h"
#include "CrossEntityMesh.h"


#define TAG_NAME "CrossEngine"


#define Engine()             CrossEngine::CEngine::GetEngine()

#define RenderSystem()       CrossEngine::CEngine::GetEngine()->GetRenderSystem()
#define GfxDevice()          CrossEngine::CEngine::GetEngine()->GetRenderSystem()->GetDevice()
#define GfxSwapChain()       CrossEngine::CEngine::GetEngine()->GetRenderSystem()->GetSwapchain()

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
		CRenderSystem* GetRenderSystem(void) const;
		CResourceSystem* GetResourceSystem(void) const;

	public:
		BOOL Init(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, RECT rcView, DWORD dwSoundMemPoolSize, INT maxChannels, const CHAR *szLogFileName, BOOL bEditorMode);
		void Exit(void);


	protected:
		CRenderSystem *m_pRenderSystem;
		CResourceSystem *m_pResourceSystem;

	protected:
		static CEngine sInstance;
	};

}
