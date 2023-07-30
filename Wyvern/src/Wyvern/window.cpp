#include "window.h"

namespace Wyvern {

Window::Window(const char* title)
{
    m_windowData.windowTitle = title;
    m_windowData.windowWidth = WINDOW_WIDTH;
    m_windowData.windowHeight = WINDOW_HEIGHT;

	WYVERN_LOG_INFO("Initializing Native window");
	glfwInit();										// Initialize GLFW library
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Do not create an opengl context since we are using VK. (GLFW was originally dessigned to do this)
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);		// Disable resizing

	m_nativeWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr); // Create window instance

    glfwSetWindowUserPointer(m_nativeWindow, &m_windowData);
	glfwSetFramebufferSizeCallback(m_nativeWindow, Window::framebufferResizeCallback);
}

Window::~Window()
{
	glfwDestroyWindow(m_nativeWindow);
	glfwTerminate();
}

void Window::pollEvents()
{
    glfwPollEvents();			// Poll for events e.g. Button presses, mouse movements, window close
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->m_framebufferResized = true;
}

void Window::initCallbacks(const EventCallbackFn& callback)
{
    // Add callback function to window data user pointer
    m_windowData.eventCallbackFn = callback;

    // Setup GLFW callbacks
    glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallbackFn(event);
        });

    glfwSetWindowSizeCallback(m_nativeWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            data.windowWidth = width;
            data.windowHeight = height;
            data.eventCallbackFn(event);
        });

    glfwSetCursorPosCallback(m_nativeWindow, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event(xPos, yPos);
            data.eventCallbackFn(event);
        });

    glfwSetScrollCallback(m_nativeWindow, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event(xOffset, yOffset);
            data.eventCallbackFn(event);
        });

    glfwSetKeyCallback(m_nativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {

            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, false); // not repeated by default
                data.eventCallbackFn(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.eventCallbackFn(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, true); // repeating
                data.eventCallbackFn(event);
                break;
            }
            default:
                throw std::runtime_error("Invalid Key Action: " + action);
            }
        });
}

int Window::shouldClose()
{
    return glfwWindowShouldClose(m_nativeWindow);
}

}


