#pragma once
#include "Wyvern/core.h"
#include "Wyvern/Input/keycodes.h"

namespace Wyvern {

class Input 
{
public:
	static void initInput(GLFWwindow* window) { Input::s_window = window; }
	static bool isKeyPressed(int Keycode) { return glfwGetKey(Input::s_window, Keycode); }
private:
	inline static GLFWwindow* s_window = nullptr;
};

}