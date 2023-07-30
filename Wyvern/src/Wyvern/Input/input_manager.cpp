#include "input_manager.h"


namespace Wyvern {

void InputManager::bindKey(KeyRequirement requirement, InputAction&& action)
{
	m_keybinds.insert(std::make_pair(requirement, action));
}

void InputManager::unbindKey(int keycode)
{

}

void InputManager::processInput()
{

}

bool InputManager::onMouseMoved(MouseMovedEvent& e)
{
	WYVERN_LOG_INFO("Mouse Moved");
	return true;
}

bool InputManager::onMouseScrolled(MouseScrolledEvent& e)
{
	WYVERN_LOG_INFO("Mouse Scrolled");
	return true;
}

bool InputManager::onKeyPressed(KeyPressedEvent& e)
{
	WYVERN_LOG_INFO("Key Pressed");
	return true;
}

bool InputManager::onKeyReleased(KeyReleasedEvent& e)
{
	WYVERN_LOG_INFO("Key Released");
	return true;
}

InputAction& InputManager::getBoundAction(KeyRequirement& requirement)
{
	return m_keybinds.at(requirement);
}

}
