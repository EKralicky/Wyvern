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
void Wyvern::WYVKSurface::initialize(VkInstance instance, GLFWwindow* window)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	VKInfo::createWin32SurfaceInfo(createInfo, window);
	VK_CALL(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &m_surface), "Unable to create Vulkan Win32 surface!")
}

/*
	Destroying a VkSurfaceKHR merely severs the connection between Vulkan and the native surface, 
	and does not imply destroying the native surface, closing a window, or similar behavior.
*/
void WYVKSurface::destroy(VkInstance instance)
{
	vkDestroySurfaceKHR(instance, m_surface, nullptr);
}

/*
	This function creates a Vulkan surface for the specified window.
	The window surface cannot be shared with another API so the window must have been created with the client api hint set to GLFW_NO_API 
	otherwise it generates a GLFW_INVALID_VALUE error and returns VK_ERROR_NATIVE_WINDOW_IN_USE_KHR.

	The window surface must be destroyed before the specified Vulkan instance. 
	It is the responsibility of the caller to destroy the window surface. 
	GLFW does not destroy it for you. Call vkDestroySurfaceKHR to destroy the surface.
*/
void WYVKSurface::createGLFWSurface(VkInstance instance, GLFWwindow* window)
{
	VK_CALL(glfwCreateWindowSurface(instance, window, nullptr, &m_surface), "Unable to create GLFW window surface!")
}



}

