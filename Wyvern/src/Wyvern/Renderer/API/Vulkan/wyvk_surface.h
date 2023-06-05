#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Renderer/API/Vulkan/CreateInfo/info.h"
#include "Wyvern/window.h"
#include "wyvk_instance.h"
#include "wyvk_device.h"

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
	WYVKSurface(WYVKInstance& instance, WYVKDevice& device, Window& window);
	void destroy();

	void createGLFWSurface();
	void createWin32Surface();
	void querySupportDetails(); // Queries and stores surface formats and present modes

	inline VkSurfaceKHR getSurface() const { return m_surface; }
	inline std::vector<VkSurfaceFormatKHR>& getSurfaceFormats() { return m_formats; }
	inline std::vector<VkPresentModeKHR>& getPresentModes() { return m_presentModes; }

private:
	VkSurfaceKHR m_surface;
	std::vector<VkSurfaceFormatKHR> m_formats;
	std::vector<VkPresentModeKHR> m_presentModes;

	// Handles
	WYVKInstance& m_instance;
	WYVKDevice& m_device;
	Window& m_window;
};




}