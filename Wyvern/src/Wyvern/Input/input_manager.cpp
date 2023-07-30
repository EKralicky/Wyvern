#include "input_manager.h"


namespace Wyvern {

void InputManager::bindKey(KeyRequirement requirement, InputAction&& action)
{
	m_keybinds[requirement] = action;
}

void InputManager::unbindKey(int keycode)
{
}

void InputManager::getBoundAction(int keycode)
{
}

}


s