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

	CRendererDeviceCompute::CRendererDeviceCompute(CRenderer *pRenderer)
		: CRendererDevice(pRenderer)
	{

	}

	CRendererDeviceCompute::~CRendererDeviceCompute(void)
	{

	}

	VkPhysicalDevice CRendererDeviceCompute::SelectPhysicalDevices(std::vector<VkPhysicalDevice> &devices, uint32_t &queueFamilyIndex)
	{
		for (uint32_t index = 0; index < devices.size(); index++) {
			if (CheckDeviceCapabilities(devices[index]) != VK_SUCCESS) continue;
			if (CheckDeviceExtensionProperties(devices[index]) != VK_SUCCESS) continue;
			if (CheckDeviceQueueFamilyProperties(devices[index], queueFamilyIndex) != VK_SUCCESS) continue;

			return devices[index];
		}

		return VK_NULL_HANDLE;
	}

	VkResult CRendererDeviceCompute::CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex)
	{
		float queuePpriorities[1] = { 0.0f };
		VkDeviceQueueCreateInfo queueCreateInfo[1];
		queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo[0].pNext = NULL;
		queueCreateInfo[0].flags = 0;
		queueCreateInfo[0].queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo[0].queueCount = 1;
		queueCreateInfo[0].pQueuePriorities = queuePpriorities;

		VkDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = NULL;
		deviceCreateInfo.flags = 0;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = NULL;
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = NULL;
		deviceCreateInfo.pEnabledFeatures = NULL;

		m_vkPhysicalDevice = vkPhysicalDevice;
		vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &m_vkMemoryProperties);

		return vkCreateDevice(m_vkPhysicalDevice, &deviceCreateInfo, m_pRenderer->GetAllocator()->GetAllocationCallbacks(), &m_vkDevice);
	}

	VkResult CRendererDeviceCompute::CheckDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice)
	{
		vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &m_vkDeviceFeatures);
		vkGetPhysicalDeviceProperties(vkPhysicalDevice, &m_vkDeviceProperties);

		if (VK_VERSION_MAJOR(m_vkDeviceProperties.apiVersion) < 1) {
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		return VK_SUCCESS;
	}

	VkResult CRendererDeviceCompute::CheckDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const
	{
		return VK_SUCCESS;
	}

	VkResult CRendererDeviceCompute::CheckDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const
	{
		queueFamilyIndex = UINT32_MAX;

		uint32_t numQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, NULL);

		ASSERT(numQueueFamilies > 0);
		std::vector<VkQueueFamilyProperties> queueFamilies(numQueueFamilies);
		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, queueFamilies.data());

		for (uint32_t index = 0; index < numQueueFamilies; index++) {
			if ((queueFamilies[index].queueCount > 0) && (queueFamilies[index].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				if (queueFamilyIndex == UINT32_MAX) {
					queueFamilyIndex = index;
					return VK_SUCCESS;
				}
			}
		}

		return VK_ERROR_INITIALIZATION_FAILED;
	}

}
