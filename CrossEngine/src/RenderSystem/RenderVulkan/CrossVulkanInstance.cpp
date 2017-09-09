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


	int CVulkanInstance::vkErrorCode = VK_SUCCESS;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback = NULL;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = NULL;

	void CVulkanInstance::SetLastError(int err)
	{
		vkErrorCode = err;
	}

	int CVulkanInstance::GetLastError(void)
	{
		return vkErrorCode;
	}


	CVulkanInstance::CVulkanInstance(void)
		: m_vkInstance(VK_NULL_HANDLE)
		, m_vkSurface(VK_NULL_HANDLE)

		, m_pDevice(NULL)
		, m_pSwapchain(NULL)
		, m_pAllocator(NULL)

#ifdef _DEBUG
		, m_vkDebugReportCallback(VK_NULL_HANDLE)
#endif
	{
		m_pDevice = SAFE_NEW CVulkanDevice(this);
		m_pSwapchain = SAFE_NEW CVulkanSwapchain(m_pDevice);
		m_pAllocator = SAFE_NEW CVulkanAllocator;
	}

	CVulkanInstance::~CVulkanInstance(void)
	{
		SAFE_DELETE(m_pDevice);
		SAFE_DELETE(m_pSwapchain);
		SAFE_DELETE(m_pAllocator);
	}

	BOOL CVulkanInstance::Create(HINSTANCE hInstance, HWND hWnd, HDC hDC, uint32_t width, uint32_t height)
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
			CALL_VK_FUNCTION_THROW(CreateSwapchain(width, height, VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR));

			return TRUE;
		}
		catch (int err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanInstance::Destroy(void)
	{
		m_pDevice->WaitIdle();

		DestroySwapchain();
		DestroyDevice();
		DestroyPresentationSurface();
		DestroyDebugReportCallback();
		DestroyInstance();
	}

	int CVulkanInstance::EnumerateInstanceLayerProperties(std::vector<const char*> &enabledInstanceLayers) const
	{
		enabledInstanceLayers.clear();

		uint32_t numLayers;
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceLayerProperties(&numLayers, NULL));
		if (numLayers == 0) return VK_ERROR_INITIALIZATION_FAILED;

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

	int CVulkanInstance::EnumerateInstanceExtensionProperties(std::vector<const char*> &enabledInstanceExtensions) const
	{
		enabledInstanceExtensions.clear();

		uint32_t numExtensions;
		CALL_VK_FUNCTION_RETURN(vkEnumerateInstanceExtensionProperties(NULL, &numExtensions, NULL));
		if (numExtensions == 0) return VK_ERROR_INITIALIZATION_FAILED;

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

#ifdef PLATFORM_WINDOWS
			if (stricmp(extensions[index].extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
				enabledInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
				bPlatformSurfaceExtension = TRUE;
				continue;
			}
#endif

#ifdef PLATFORM_ANDROID
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

	int CVulkanInstance::CreateInstance(const std::vector<const char*> &enabledInstanceLayers, const std::vector<const char*> &enabledInstanceExtensions)
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

	int CVulkanInstance::CreateDebugReportCallback(void)
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

	int CVulkanInstance::CreatePresentationSurface(HINSTANCE hInstance, HWND hWnd)
	{
#ifdef PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = NULL;
		surfaceInfo.flags = 0;
		surfaceInfo.hinstance = hInstance;
		surfaceInfo.hwnd = hWnd;
		return vkCreateWin32SurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface);
#endif

#ifdef PLATFORM_ANDROID
		VkAndroidSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = NULL;
		surfaceInfo.flags = 0;
		surfaceInfo.window = (ANativeWindow *)hWnd;
		return vkCreateAndroidSurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface);
#endif
	}

	int CVulkanInstance::CreateDevice(void)
	{
		return m_pDevice->Create();
	}

	int CVulkanInstance::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		return m_pSwapchain->Create(width, height, transform);
	}

	void CVulkanInstance::DestroyInstance(void)
	{
		if (m_vkInstance) {
			vkDestroyInstance(m_vkInstance, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkInstance = VK_NULL_HANDLE;
	}

	void CVulkanInstance::DestroyDebugReportCallback(void)
	{
#ifdef _DEBUG
		if (vkDestroyDebugReportCallback && m_vkDebugReportCallback) {
			vkDestroyDebugReportCallback(m_vkInstance, m_vkDebugReportCallback, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkDebugReportCallback = VK_NULL_HANDLE;
#endif
	}

	void CVulkanInstance::DestroyPresentationSurface(void)
	{
		if (m_vkSurface) {
			vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, m_pAllocator->GetAllocationCallbacks());
		}

		m_vkSurface = VK_NULL_HANDLE;
	}

	void CVulkanInstance::DestroyDevice(void)
	{
		m_pDevice->Destroy();
	}

	void CVulkanInstance::DestroySwapchain(void)
	{
		m_pSwapchain->Destroy();
	}

	VkInstance CVulkanInstance::GetInstance(void) const
	{
		return m_vkInstance;
	}

	VkSurfaceKHR CVulkanInstance::GetSurface(void) const
	{
		return m_vkSurface;
	}

	CGfxDevice* CVulkanInstance::GetDevice(void) const
	{
		return m_pDevice;
	}

	CGfxSwapchain* CVulkanInstance::GetSwapchain(void) const
	{
		return m_pSwapchain;
	}

	CVulkanAllocator* CVulkanInstance::GetAllocator(void) const
	{
		return m_pAllocator;
	}

}
