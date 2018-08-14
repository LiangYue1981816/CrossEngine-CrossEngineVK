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

	class CROSS_EXPORT CVulkanInstance : public CGfxInstance
	{
		friend class CRenderSystem;


	protected:
		CVulkanInstance(void);
		virtual ~CVulkanInstance(void);


	public:
		static void SetLastError(int err);
		static int GetLastError(void);

	public:
		BOOL Create(HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		void Destroy(void);

	protected:
		int EnumerateInstanceLayerProperties(eastl::vector<const char*> &enabledInstanceLayers) const;
		int EnumerateInstanceExtensionProperties(eastl::vector<const char*> &enabledInstanceExtensions) const;
		int CreateInstance(const eastl::vector<const char*> &enabledInstanceLayers, const eastl::vector<const char*> &enabledInstanceExtensions);
		int CreateDebugReportCallback(void);
		int CreatePresentationSurface(HINSTANCE hInstance, HWND hWnd);
		int CreateDevice(void);
		int CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);

	protected:
		void DestroyInstance(void);
		void DestroyDebugReportCallback(void);
		void DestroyPresentationSurface(void);
		void DestroyDevice(void);
		void DestroySwapchain(void);

	public:
		VkInstance GetInstance(void) const;
		VkSurfaceKHR GetSurface(void) const;

	public:
		CGfxDevice* GetDevice(void) const;
		CGfxSwapchain* GetSwapchain(void) const;

	public:
		CVulkanAllocator* GetAllocator(void) const;


	protected:
		VkInstance m_vkInstance;
		VkSurfaceKHR m_vkSurface;

#ifdef _DEBUG
		VkDebugReportCallbackEXT m_vkDebugReportCallback;
#endif

	protected:
		CVulkanDevice *m_pDevice;
		CVulkanSwapchain *m_pSwapchain;
		CVulkanAllocator *m_pAllocator;

	protected:
		static int vkErrorCode;
	};

}
