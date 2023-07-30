#pragma once
#include "Wyvern/core.h"
#include "keycodes.h"
#include <ostream>

namespace Wyvern {

enum class InputType
{
	NONE,       // Does nothing
	CONTINUOUS, // Action will continuously be performed as long as the mapped key is held
	TOGGLE,		// Action will perform once after the key is pressed. If held, nothing will happen
	REPEAT		// Default key action. Performs once after the key is pressed, then if held, after some amount of time, will continuously perform the action until released
};

class InputAction
{
public:
	friend std::ostream& operator<<(std::ostream& os, InputAction const& action) { 	// Allow printing of action name
		return os << "Action Name: " << action.m_name << "\n";
	}

	InputAction(std::string name, InputType type, std::function<void()> func, int modifiers) 
		: m_name(name), m_type(type), m_func(func)
	{
	}

	inline void execute() const { m_func(); }
	inline InputType getType() const { return m_type; }

private:
	InputType m_type;
	std::string m_name;
	std::function<void()> m_func;
};

}