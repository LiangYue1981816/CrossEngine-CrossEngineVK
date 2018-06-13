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
#include "CrossRenderSystemDefinition.h"
#include "CrossDrawable.h"
#include "CrossDrawablePartical.h"
#include "CrossDrawableSkinMesh.h"
#include "CrossDrawableStaticMesh.h"
#include "CrossDrawableManager.h"
#include "CrossBatch.h"
#include "CrossBatchPartical.h"
#include "CrossBatchSkinMesh.h"
#include "CrossBatchStaticMesh.h"
#include "CrossRenderer.h"
#include "CrossCamera.h"
#include "CrossCameraManager.h"
#include "CrossScreen.h"


namespace CrossEngine {

	typedef enum {
		GFX_API_NONE = 0,
		GFX_API_GLES31, 
		GFX_API_VULKAN
	} GFX_API;

	typedef enum {
		DESCRIPTOR_SET_DRAW = 0,
		DESCRIPTOR_SET_PASS = 1,
		DESCRIPTOR_SET_FRAME = 2
	} DESCRIPTOR_SET_TYPE;

	typedef enum {
		DESCRIPTOR_BIND_CAMERA = 0,
		DESCRIPTOR_BIND_TRANSFORM = 1,
		DESCRIPTOR_BIND_AMBIENT_LIGHT = 2,
		DESCRIPTOR_BIND_POINT_LIGHT = 3,
		DESCRIPTOR_BIND_DIRECTION_LIGHT = 4,
		DESCRIPTOR_BIND_EXTERNAL_BASE = 16
	} DESCRIPTOR_BIND_TYPE;

	static const char *DESCRIPTOR_BIND_NAME[5] = {
		"engineCamera",
		"engineTransform",
		"engineAmbientLight",
		"enginePointLight",
		"engineDirectionLight"
	};


	class CROSS_EXPORT CRenderSystem
	{
		friend class CEngine;


	protected:
		CRenderSystem(void);
		virtual ~CRenderSystem(void);


	protected:
		BOOL Create(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		BOOL CreateGfx(GFX_API api, HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		BOOL CreateCameraManager(void);
		BOOL CreateDrawableManager(void);

		void Destroy(void);
		void DestroyGfx(void);
		void DestroyCameraManager(void);
		void DestroyDrawableManager(void);

	public:
		GFX_API GetAPI(void) const;
		CGfxDevice* GetDevice(void) const;
		CGfxSwapchain* GetSwapchain(void) const;

	public:
		CCamera* GetCamera(uint32_t dwName);
		void RemoveCamera(uint32_t dwName);
		void RemoveCameraAll(void);

	public:
		CDrawable* AllocDrawable(DRAWABLE_TYPE type);
		void FreeDrawable(CDrawable *pDrawable);
		void FreeDrawableAll(void);

	public:
		void Update(void);
		void Render(uint32_t dwName, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);


	protected:
		GFX_API m_api;

		CGfxInstance *m_pGfxInstance;
		CGfxDevice *m_pGfxDevice;
		CGfxSwapchain *m_pGfxSwapchain;

	protected:
		CCameraManager *m_pCameraManager;
		CDrawableManager *m_pDrawableManager;
	};

}
