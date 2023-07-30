#pragma once

#include "event.h"

namespace Wyvern {

	// MOUSE MOVE
	class MouseMovedEvent : public Event
	{
	private:
		float m_XPos, m_YPos;
	public:
		MouseMovedEvent(float x, float y)
			: m_XPos(x), m_YPos(y) {}

		inline float getXPos() const { return m_XPos; }
		inline float getYPos() const { return m_YPos; }

		std::string toString() const override 
		{
			std::stringstream ss;
			ss << "Event: MouseMovedEvent ( X: " << m_XPos << ", Y: " << m_YPos << ")";
			return ss.str();
		}

		EVENT_TYPE(MouseMoved)
		EVENT_CATEGORY(CategoryInput | CategoryMouse) 
	};

	// MOUSE SCROLL
	class MouseScrolledEvent : public Event
	{
	private:
		float m_XOffset, m_YOffset;
	public:
		MouseScrolledEvent(float x, float y)
			: m_XOffset(x), m_YOffset(y) {}

		float getXOffset() const { return m_XOffset; }
		float getYOffset() const { return m_YOffset; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseScrolledEvent ( XOffset: " << m_XOffset << ", YOffset: " << m_YOffset << ")";
			return ss.str();
		}

		EVENT_TYPE(MouseScrolled)
		EVENT_CATEGORY(CategoryInput | CategoryMouse)
	};

	// MOUSE BUTTON
	class MouseButtonEvent : public Event
	{
	protected:
		MouseButtonEvent(int button)
			: m_button(button) {}
		int m_button;
	public:
		inline int getButton() const { return m_button; }
		EVENT_CATEGORY(CategoryInput | CategoryMouse)
	};

	// When the user presses any key
	// repeated - Indicates that this event is a repeated event (key gets held down)
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		inline int getButton() const { return m_button; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseButtonPressedEvent ( Button: " << m_button << ")";
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPressed)
	};

	// When the a key is released
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		inline int getButton() const { return m_button; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseButtonReleasedEvent ( Button: " << m_button << ")";
			return ss.str();
		}

		EVENT_TYPE(MouseButtonReleased)
	};






}