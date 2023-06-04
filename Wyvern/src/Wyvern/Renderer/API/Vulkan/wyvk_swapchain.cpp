//#include "wyvk_swapchain.h"
//
//namespace Wyvern {
//
//void WYVKSwapchain::initialize(VkInstance instance, VkPhysicalDevice device, VkSurfaceKHR surface)
//{
//	m_surface = surface;
//	m_physicalDevice = device;
//}
//
//WYVKSwapchain::SwapChainSupportDetails WYVKSwapchain::querySwapChainSupport()
//{
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_supportDetails.capabilities);
//
//	/*
//	* Query supported surface formats
//	*/
//	uint32_t formatCount;
//	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
//
//	if (formatCount != 0) {
//		m_supportDetails.formats.resize(formatCount);
//		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, m_supportDetails.formats.data());
//	}
//
//	/*
//	* Query supported presentation modes
//	*/
//	uint32_t presentModeCount;
//	vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
//
//	if (presentModeCount != 0) {
//		m_supportDetails.presentModes.resize(presentModeCount);
//		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, m_supportDetails.presentModes.data());
//	}
//}
//
//}
//
