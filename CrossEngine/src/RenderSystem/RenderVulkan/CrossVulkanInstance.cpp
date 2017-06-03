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


	VkResult CVulkanInstance::vkErrorCode = VK_SUCCESS;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback = NULL;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = NULL;

	void CVulkanInstance::SetLastError(VkResult err)
	{
		vkErrorCode = err;
	}

	VkResult CVulkanInstance::GetLastError(void)
	{
		return vkErrorCode;
	}


	CVulkanInstance::CVulkanInstance(const char *szCachePath)
		: m_vkInstance(VK_NULL_HANDLE)
		, m_vkSurface(VK_NULL_HANDLE)

		, m_pDevice(NULL)
		, m_pSwapchain(NULL)
		, m_pAllocator(NULL)

		, m_szCachePath{ 0 }

#ifdef _DEBUG
		, m_vkDebugReportCallback(VK_NULL_HANDLE)
#endif
	{
		if (szCachePath) {
			strcpy(m_szCachePath, szCachePath);
		}

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
		return TRUE;
	}

	void CVulkanInstance::Destroy(void)
	{

	}

	VkResult CVulkanInstance::EnumerateInstanceLayerProperties(std::vector<const char*> &enabledInstanceLayers) const
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::EnumerateInstanceExtensionProperties(std::vector<const char*> &enabledInstanceExtensions) const
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::CreateInstance(const std::vector<const char*> &enabledInstanceLayers, const std::vector<const char*> &enabledInstanceExtensions)
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::CreateDebugReportCallback(void)
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::CreatePresentationSurface(HINSTANCE hInstance, HWND hWnd)
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::CreateDevice(void)
	{
		return VK_SUCCESS;
	}

	VkResult CVulkanInstance::CreateSwapchain(uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		return VK_SUCCESS;
	}

	void CVulkanInstance::DestroyInstance(void)
	{

	}

	void CVulkanInstance::DestroyDebugReportCallback(void)
	{

	}

	void CVulkanInstance::DestroyPresentationSurface(void)
	{

	}

	void CVulkanInstance::DestroyDevice(void)
	{

	}

	void CVulkanInstance::DestroySwapchain(void)
	{

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

	const char* CVulkanInstance::GetCachePath(void) const
	{
		return m_szCachePath;
	}

}
