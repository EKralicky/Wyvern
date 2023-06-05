#include "wyvk_surface.h"

/*
* SOME COMMENTS IN THIS SECTION WERE EXCERPTED FROM https://www.glfw.org/docs/ AND https://registry.khronos.org/vulkan/specs/
*/

namespace Wyvern {

/*
	Platform dependent Vulkan surface initialization.
	Vulkan utilizes "surfaces" as a medium to interface with the window system. In this case, the function
	specifically creates a Win32 surface, which is the Windows-specific type of surface that Vulkan uses.
*/
WYVKSurface::WYVKSurface(WYVKInstance& instance, WYVKDevice& device, Window& window)
	: m_instance(instance), m_device(device), m_window(window)
{
	createWin32Surface();
	createGLFWSurface();
	querySupportDetails();
}

/*
	Destroying a VkSurfaceKHR merely severs the connection between Vulkan and the native surface, 
	and does not imply destroying the native surface, closing a window, or similar behavior.
*/
void WYVKSurface::destroy()
{
	vkDestroySurfaceKHR(m_instance.getInstance(), m_surface, nullptr);
}

/*
	This function creates a Vulkan surface for the specified window.
	The window surface cannot be shared with another API so the window must have been created with the client api hint set to GLFW_NO_API 
	otherwise it generates a GLFW_INVALID_VALUE error and returns VK_ERROR_NATIVE_WINDOW_IN_USE_KHR.

	The window surface must be destroyed before the specified Vulkan instance. 
	It is the responsibility of the caller to destroy the window surface. 
	GLFW does not destroy it for you. Call vkDestroySurfaceKHR to destroy the surface.
*/
void WYVKSurface::createGLFWSurface()
{
	VK_CALL(glfwCreateWindowSurface(m_instance.getInstance(), m_window.getNativeWindow(), nullptr, &m_surface), "Unable to create GLFW window surface!")
}

void WYVKSurface::createWin32Surface()
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	VKInfo::createWin32SurfaceInfo(createInfo, m_window.getNativeWindow());
	VK_CALL(vkCreateWin32SurfaceKHR(m_instance.getInstance(), &createInfo, nullptr, &m_surface), "Unable to create Vulkan Win32 surface!");
}

void WYVKSurface::querySupportDetails()
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.getPhysicalDevice(), m_surface, &m_supportDetails.capabilities);
	/*
	* Query supported surface formats
	*/
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &formatCount, nullptr);

	if (formatCount != 0) {
		m_supportDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.getPhysicalDevice(), m_surface, &formatCount, m_supportDetails.formats.data());
	}
	else {
		throw std::runtime_error("Format count is 0!");
	}

	/*
	* Check for present support (capability for a device to present to the surface)
	* Since the device is initialized before the surface and the device checks if the queue families are valid, we know presentFamily has a valid index
	* Queue families are checked and queried in the findQueueFamilies() function in the Device class
	*/ 
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(m_device.getPhysicalDevice(), m_device.getQueueFamilyIndices().presentFamily.value(), m_surface, &presentSupport);
	WYVERN_ASSERT(presentSupport, "The current present queue family does not have present support!")

	/*
	* Query supported presentation modes
	*/
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.getPhysicalDevice(), m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		m_supportDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.getPhysicalDevice(), m_surface, &presentModeCount, m_supportDetails.presentModes.data());
	}
	else {
		throw std::runtime_error("No present modes available! Present mode count is 0!");
	}
}

}

