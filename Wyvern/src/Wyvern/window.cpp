#include "window.h"

namespace Wyvern {

Window::Window(const char* title)
{
	WYVERN_LOG_INFO("Initializing Native window");
	glfwInit();										// Initialize GLFW library
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Do not create an opengl context since we are using VK. (GLFW was originally dessigned to do this)
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// Disable resizing

	m_nativeWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr); // Create window instance
}

void Window::destroy()
{
	glfwDestroyWindow(m_nativeWindow);
	glfwTerminate();
}

}

