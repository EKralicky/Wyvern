#pragma once
#include "core.h"

namespace Wyvern {

class Window
{
	static const uint32_t WINDOW_WIDTH = 1920;
	static const uint32_t WINDOW_HEIGHT = 1080;

public:

	Window(const char* title);
	void destroy();

	inline GLFWwindow* getNativeWindow() const { return m_nativeWindow; }
	inline bool isFramebufferResized() const { return m_framebufferResized; }
	void setFramebufferResized(bool flag) { m_framebufferResized = flag; }

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	bool m_framebufferResized = false;
	GLFWwindow* m_nativeWindow;
};

}