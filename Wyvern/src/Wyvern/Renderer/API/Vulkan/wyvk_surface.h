#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/CreateInfo/info.h"

namespace Wyvern {
/*
	Since Vulkan is a platform agnostic API, it can not interface directly with the window system on its own. 
	To establish the connection between Vulkan and the window system to present results to the screen, we need to use the WSI (Window System Integration) extensions. 
	The surface extension needed is VK_KHR_surface. It exposes a VkSurfaceKHR object that represents an abstract type of surface to present rendered images to. 
	The surface in the program will be backed by the window that we've already opened with GLFW.
	Red: https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface
*/
class WYVKSurface
{
public:
	void initialize(VkInstance instance, GLFWwindow* window);
	void destroy(VkInstance instance);
	void createGLFWSurface(VkInstance instance, GLFWwindow* window);
private:
	VkSurfaceKHR m_surface;
};




}