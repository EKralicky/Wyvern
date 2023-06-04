#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/CreateInfo/info.h"
#include "Wyvern/Renderer/API/Vulkan/wyvk_instance.h"
#include "Wyvern/window.h"

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
	WYVKSurface(WYVKInstance& instance, Window& window);
	void destroy();

	void createGLFWSurface();
	void createWin32Surface();
	inline VkSurfaceKHR getSurface() const { return m_surface; }
private:
	VkSurfaceKHR m_surface;

	// Handles
	WYVKInstance& m_instance;
	Window& m_window;
};




}