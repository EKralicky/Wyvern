#pragma once

#include "core.h"
#include "Wyvern/Events/application_event.h"
#include "Wyvern/Events/key_event.h"
#include "Wyvern/Events/mouse_event.h"

namespace Wyvern {

class Window
{

static const uint32_t WINDOW_WIDTH = 1920;
static const uint32_t WINDOW_HEIGHT = 1080;

using EventCallbackFn = std::function<void(Event&)>;

public:
	Window(const char* title);
	~Window();

	void pollEvents();
	void updateDeltaTime();
	void initCallbacks(const EventCallbackFn& callback);
	int shouldClose();

	inline GLFWwindow* getNativeWindow() const { return m_nativeWindow; }
	inline float deltaTime() const { return m_deltaTime; }
	inline bool isFramebufferResized() const { return m_framebufferResized; }
	void setFramebufferResized(bool flag) { m_framebufferResized = flag; }
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	int getWidth() { return m_windowData.windowWidth; }
	int getHeight() { return m_windowData.windowHeight; }


private:
	struct WindowData {
		const char* windowTitle;
		int windowWidth;
		int windowHeight;
		bool verticalSyncEnabled;
		EventCallbackFn eventCallbackFn;
	};

	bool m_framebufferResized = false;
	GLFWwindow* m_nativeWindow;
	WindowData m_windowData;
	float m_deltaTime = 0;
	float m_lastFrameTime = 0;
};

}