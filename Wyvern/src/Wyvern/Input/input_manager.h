#pragma once
#include <functional>

#include "Wyvern/core.h"
#include "Wyvern/Input/input_action.h"

namespace Wyvern {

struct KeyRequirement
{
	int keycode;
	int modifiers;

	KeyRequirement(int keycode, int modifiers)
		: keycode(keycode), modifiers(modifiers)
	{
	}

	bool operator==(const KeyRequirement& other) const
	{
		return keycode == other.keycode && modifiers == other.modifiers;
	}

};

template <class T>
inline void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

struct KeyHash
{
	std::size_t operator()(const KeyRequirement& k) const
	{
		std::size_t res = 0;
		hash_combine(res, k.keycode);
		hash_combine(res, k.modifiers);
		return res;
	}
};

class InputManager
{
public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}

	// Delete because singleton lol
	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;
	
	/*
	* Initializes InputManager member variables and sets up proper functionality
	*/
	void initInputManager();

	/*
	* Iterate over all active keys in the activeKeys vector, find their input action in the keybinds map
	* and execute the action possibly with requirements if they are needed for the action.
	* Also checks the modifier bitmask for modifier keys.
	*/
	void processInput(int activeModifierMask);

	/*
	* 
	*/
	void updateKey();

	/*
	* Maps an integer keycode to and InputAction reference using the keybinds hashmap
	*/
	void bindKey(KeyRequirement requirement, InputAction&& action);

	/*
	* Removes the InputAction bound to a keycode
	*/
	void unbindKey(int keycode);

	/*
	* Returns an InputAction reference bound to a keycode
	*/
	InputAction& getBoundAction(int keycode);

private:
	InputManager() {}

	std::unordered_map<KeyRequirement, InputAction, KeyHash> m_keybinds;
	std::vector<int> m_activeKeys;
	int m_modifierMask = 0;

};





}

  