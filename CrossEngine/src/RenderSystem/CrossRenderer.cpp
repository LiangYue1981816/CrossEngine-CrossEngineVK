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

#include "_CrossEngine.h"


namespace CrossEngine {

	VkBool32 VKAPI_PTR DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
			LOGE("Vulkan [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
			return FALSE;
		}

		if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
			LOGW("Vulkan [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
			return FALSE;
		}

		if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
			LOGW("Vulkan [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
			return FALSE;
		}

		if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
			LOGI("Vulkan [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
			return FALSE;
		}

		LOGI("Vulkan [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return TRUE;
	}

	VkResult CRenderer::vkErrorCode = VK_SUCCESS;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback = NULL;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = NULL;

	CRenderer::CRenderer(void)
		: m_vkInstance(VK_NULL_HANDLE)
		, m_vkSurface(VK_NULL_HANDLE)

		, m_pAllocator(NULL)
		, m_pSwapchain(NULL)
		, m_pComputeDevice(NULL)
		, m_pGraphicsDevice(NULL)

#ifdef _DEBUG
		, m_vkDebugReportCallback(VK_NULL_HANDLE)
#endif
	{
		m_pAllocator = SAFE_NEW CRendererAllocator();
		m_pComputeDevice = SAFE_NEW CRendererDeviceCompute(this);
		m_pGraphicsDevice = SAFE_NEW CRendererDeviceGraphics(this);
		m_pSwapchain = SAFE_NEW CRendererSwapchain(m_pGraphicsDevice);
	}

	CRenderer::~CRenderer(void)
	{
		ASSERT(m_vkInstance == VK_NULL_HANDLE);
		ASSERT(m_vkSurface == VK_NULL_HANDLE);

		SAFE_DELETE(m_pSwapchain);
		SAFE_DELETE(m_pGraphicsDevice);
		SAFE_DELETE(m_pComputeDevice);
		SAFE_DELETE(m_pAllocator);
	}

	void CRenderer::SetLastError(VkResult err)
	{
		vkErrorCode = err;
	}

	VkResult CRenderer::GetLastError(void)
	{
		return vkErrorCode;
	}

	VkInstance CRenderer::GetInstance(void) const
	{
		return m_vkInstance;
	}

	VkSurfaceKHR CRenderer::GetSurface(void) const
	{
		return m_vkSurface;
	}

	CRendererAllocator* CRenderer::GetAllocator(void) const
	{
		return m_pAllocator;
	}

	CRendererSwapchain* CRenderer::GetSwapchain(void) const
	{
		return m_pSwapchain;
	}

	CRendererDeviceCompute* CRenderer::GetComputeDevice(void) const
	{
		return m_pComputeDevice;
	}

	CRendererDeviceGraphics* CRenderer::GetGraphicsDevice(void) const
	{
		return m_pGraphicsDevice;
	}

	BOOL CRenderer::Create(HINSTANCE hInstance, HWND hWnd)
	{
		try {
			std::vector<const char*> enabledInstanceLayers;
			std::vector<const char*> enabledInstanceExtensions;

			CALL_VK_FUNCTION_THROW(EnumerateInstanceLayerProperties(enabledInstanceLayers));
			CALL_VK_FUNCTION_THROW(EnumerateInstanceExtensionProperties(enabledInstanceExtensions));
			CALL_VK_FUNCTION_THROW(CreateInstance(enabledInstanceLayers, enabledInstanceExtensions));
			CALL_VK_FUNCTION_THROW(CreateDebugReportCallback());
			CALL_VK_FUNCTION_THROW(CreatePresentationSurface(hInstance, hWnd));
			CALL_VK_FUNCTION_THROW(CreateDevice());

			return TRUE;
		}
		catch (VkResult err) {
			CRenderer::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CRenderer::Destroy(void)
	{
		DestroyDevice();
		DestroyPresentationSurface();
		DestroyDebugReportCallback();
		DestroyInstance();
	}

	BOOL CRenderer::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		return m_pSwapchain->Create(width, height, transform);
	}

	void CRenderer::DestroySwapchain(void)
	{
		m_pSwapchain->Destroy();
	}

	VkResult CRenderer::EnumerateInstanceLayerProperties(std::vector<const char*> &enabledInstanceLayers) const
	{
		enabledInstanceLayers.clear();

		uint32_t numLayers;
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceLayerProperties(&numLayers, NULL));

		ASSERT(numLayers > 0);
		std::vector<VkLayerProperties> layers(numLayers);
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceLayerProperties(&numLayers, layers.data()));

		for (uint32_t index = 0; index < numLayers; index++) {
#ifdef _DEBUG
#define VK_LAYER_LUNARG_STANDARD_VALIDATION "VK_LAYER_LUNARG_standard_validation"
			if (stricmp(layers[index].layerName, VK_LAYER_LUNARG_STANDARD_VALIDATION) == 0) {
				enabledInstanceLayers.push_back(VK_LAYER_LUNARG_STANDARD_VALIDATION);
				continue;
			}
#endif
		}

		return VK_SUCCESS;
	}

	VkResult CRenderer::EnumerateInstanceExtensionProperties(std::vector<const char*> &enabledInstanceExtensions) const
	{
		enabledInstanceExtensions.clear();
		
		uint32_t numExtensions;
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceExtensionProperties(NULL, &numExtensions, NULL));

		ASSERT(numExtensions > 0);
		std::vector<VkExtensionProperties> extensions(numExtensions);
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceExtensionProperties(NULL, &numExtensions, extensions.data()));
		
		BOOL bSurfaceExtension = FALSE;
		BOOL bPlatformSurfaceExtension = FALSE;
		for (uint32_t index = 0; index < numExtensions; index++) {
			if (stricmp(extensions[index].extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0) {
				enabledInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
				bSurfaceExtension = TRUE;
				continue;
			}

#ifdef _PLATFORM_WINDOWS_
			if (stricmp(extensions[index].extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
				enabledInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
				bPlatformSurfaceExtension = TRUE;
				continue;
			}
#endif

#ifdef _PLATFORM_ANDROID_
			if (stricmp(extensions[index].extensionName, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) == 0) {
				enabledInstanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
				bPlatformSurfaceExtension = TRUE;
				continue;
			}
#endif

#ifdef _DEBUG
			if (stricmp(extensions[index].extensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0) {
				enabledInstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				continue;
			}
#endif
		}

		if (bSurfaceExtension == FALSE || bPlatformSurfaceExtension == FALSE) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		return VK_SUCCESS;
	}

	VkResult CRenderer::CreateInstance(const std::vector<const char*> &enabledInstanceLayers, const std::vector<const char*> &enabledInstanceExtensions)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = NULL;
		appInfo.pApplicationName = TAG_NAME;
		appInfo.applicationVersion = 1;
		appInfo.pEngineName = TAG_NAME;
		appInfo.engineVersion = 1;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = enabledInstanceLayers.size();
		createInfo.ppEnabledLayerNames = enabledInstanceLayers.data();
		createInfo.enabledExtensionCount = enabledInstanceExtensions.size();
		createInfo.ppEnabledExtensionNames = enabledInstanceExtensions.data();
		return vkCreateInstance(&createInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkInstance);
	}

	VkResult CRenderer::CreateDebugReportCallback(void)
	{
#ifdef _DEBUG
		vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugReportCallbackEXT");
		vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT");
		if (vkCreateDebugReportCallback == NULL) return VK_ERROR_INITIALIZATION_FAILED;
		if (vkDestroyDebugReportCallback == NULL) return VK_ERROR_INITIALIZATION_FAILED;

		VkDebugReportCallbackCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugInfo.pNext = NULL;
		debugInfo.pUserData = NULL;
		debugInfo.pfnCallback = DebugReportCallback;
		debugInfo.flags =
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
			VK_DEBUG_REPORT_DEBUG_BIT_EXT |
			VK_DEBUG_REPORT_ERROR_BIT_EXT |
			VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		return vkCreateDebugReportCallback(m_vkInstance, &debugInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkDebugReportCallback);
#else
		return VK_SUCCESS;
#endif
	}

	VkResult CRenderer::CreatePresentationSurface(HINSTANCE hInstance, HWND hWnd)
	{
#ifdef _PLATFORM_WINDOWS_
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = NULL;
		surfaceInfo.flags = 0;
		surfaceInfo.hinstance = hInstance;
		surfaceInfo.hwnd = hWnd;
		return vkCreateWin32SurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface);
#endif

#ifdef _PLATFORM_ANDROID_
		VkAndroidSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = NULL;
		surfaceInfo.flags = 0;
		surfaceInfo.window = (ANativeWindow *)hWnd;
		return vkCreateAndroidSurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface);
#endif
	}

	VkResult CRenderer::CreateDevice(void)
	{
		if (m_pComputeDevice->Create() == FALSE) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		if (m_pGraphicsDevice->Create() == FALSE) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		return VK_SUCCESS;
	}

	void CRenderer::DestroyInstance(void)
	{
		if (m_vkInstance) {
			vkDestroyInstance(m_vkInstance, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkInstance = VK_NULL_HANDLE;
	}

	void CRenderer::DestroyDebugReportCallback(void)
	{
#ifdef _DEBUG
		if (vkDestroyDebugReportCallback && m_vkDebugReportCallback) {
			vkDestroyDebugReportCallback(m_vkInstance, m_vkDebugReportCallback, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkDebugReportCallback = VK_NULL_HANDLE;
#endif
	}

	void CRenderer::DestroyPresentationSurface(void)
	{
		if (m_vkSurface) {
			vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkSurface = VK_NULL_HANDLE;
	}

	void CRenderer::DestroyDevice(void)
	{
		m_pComputeDevice->Destroy();
		m_pGraphicsDevice->Destroy();
	}

}
