#pragma once

#include "event.h"

namespace Wyvern {

	// Abstract class for all key events (pressed or released)
	// Cannot be instantiated
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(int keycode)
			: m_Keycode(keycode) {}
		int m_Keycode;
	public:
		inline int getKeycode() const { return m_Keycode; }
		EVENT_CATEGORY(CategoryKeyboard | CategoryInput)
	};

	// When the user presses any key
	// repeated - Indicates that this event is a repeated event (key gets held down)
	class KeyPressedEvent : public KeyEvent
	{
	private:
		bool m_Repeated;
	public:
		KeyPressedEvent(int keycode, bool repeated)
			: KeyEvent(keycode), m_Repeated(repeated) {} // Creates an instance of KeyEvent with the keycode

		inline int getKeycode() const { return m_Keycode; }
		inline bool isRepeated() const { return m_Repeated; }


		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyPressedEvent ( Keycode: " << m_Keycode << ", Repeated: " << m_Repeated << " )";
			return ss.str();
		}

		EVENT_TYPE(KeyPressed)
	};
	
	// When the a key is released
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {} // Creates an instance of KeyEvent with the keycode

		inline int getKeycode() const { return m_Keycode; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyReleasedEvent ( Keycode: " << m_Keycode << " )";
			return ss.str();
		}

		EVENT_TYPE(KeyReleased)
	};






}
