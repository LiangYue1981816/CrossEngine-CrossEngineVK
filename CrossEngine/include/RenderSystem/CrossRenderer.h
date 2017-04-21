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

	#define CALL_VK_FUNCTION_RETURN(func)   \
	{                                       \
		VkResult err = func;                \
		if (err != VK_SUCCESS) return err;  \
	}

	#define CALL_VK_FUNCTION_THROW(func)    \
	{                                       \
		VkResult err = func;                \
		if (err != VK_SUCCESS) throw err;   \
	}

	#define CALL_BOOL_FUNCTION_RETURN(func) \
	{                                       \
		BOOL err = (BOOL)func;              \
		if (err != TRUE) return err;        \
	}

	#define CALL_BOOL_FUNCTION_THROW(func)  \
	{                                       \
		BOOL err = (BOOL)func;              \
		if (err != TRUE) throw err;         \
	}


	class CROSS_EXPORT CRendererAllocator;
	class CROSS_EXPORT CRendererSwapchain;
	class CROSS_EXPORT CRendererDevice;
	class CROSS_EXPORT CRendererDeviceCompute;
	class CROSS_EXPORT CRendererDeviceGraphics;

	class CROSS_EXPORT CRenderer
	{
	public:
		CRenderer(void);
		virtual ~CRenderer(void);
	
	
	public:
		static void SetLastError(VkResult err);
		static VkResult GetLastError(void);

	public:
		BOOL Create(HINSTANCE hInstance, HWND hWnd);
		void Destroy(void);

		BOOL CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform);
		void DestroySwapchain(void);

	protected:
		VkResult EnumerateInstanceLayerProperties(std::vector<const char*> &enabledInstanceLayers) const;
		VkResult EnumerateInstanceExtensionProperties(std::vector<const char*> &enabledInstanceExtensions) const;
		VkResult CreateInstance(const std::vector<const char*> &enabledInstanceLayers, const std::vector<const char*> &enabledInstanceExtensions);
		VkResult CreateDebugReportCallback(void);
		VkResult CreatePresentationSurface(HINSTANCE hInstance, HWND hWnd);
		VkResult CreateDevice(void);

	protected:
		void DestroyInstance(void);
		void DestroyDebugReportCallback(void);
		void DestroyPresentationSurface(void);
		void DestroyDevice(void);

	public:
		VkInstance GetInstance(void) const;
		VkSurfaceKHR GetSurface(void) const;

	public:
		CRendererAllocator* GetAllocator(void) const;
		CRendererSwapchain* GetSwapchain(void) const;
		CRendererDeviceCompute* GetComputeDevice(void) const;
		CRendererDeviceGraphics* GetGraphicsDevice(void) const;


	protected:
		VkInstance m_vkInstance;
		VkSurfaceKHR m_vkSurface;

	protected:
		CRendererAllocator *m_pAllocator;
		CRendererSwapchain *m_pSwapchain;
		CRendererDeviceCompute *m_pComputeDevice;
		CRendererDeviceGraphics *m_pGraphicsDevice;

#ifdef _DEBUG
		VkDebugReportCallbackEXT m_vkDebugReportCallback;
#endif

	protected:
		static VkResult vkErrorCode;
	};

}
