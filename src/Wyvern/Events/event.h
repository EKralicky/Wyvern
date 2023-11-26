#pragma once

#include <string>
#include <sstream>
#include <functional>

#include "Wyvern/Core.h"

namespace Wyvern {
	// Credit: Cherno

	// Blocking events
	// In future use event bus and run events during event stage in rendering

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// To be used in specific event classes for the event dispatcher & debugging.
	// getStaticType() - Simply returns the EventType associated with the class
	// getEventType() - Is used to get an event type given a generic event. The event dispatcher can check this type against a static event type
#define EVENT_TYPE(type) static EventType getStaticType() { return EventType::type; }\
						 virtual EventType getEventType() const override { return getStaticType(); }\
						 virtual const char* getEventName() const override { return #type; }

	// We use BIT() here so we can check for multiple categories if we want to filter events
	enum EventCategory
	{
		None = 0,
		CategoryApplication = BIT(1),
		CategoryInput		= BIT(2),
		CategoryKeyboard	= BIT(3),
		CategoryMouse		= BIT(4)
	};

	// To be used in specific event classes
	// Use bitwise or "|" to specify multiple categories
#define EVENT_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }


	class Event
	{
		// EventDispatcher can access private & protected variables from this class
		friend class EventDispatcher;
	protected:
		bool m_handled = false;
	public:
		virtual EventType getEventType()	const = 0; 
		virtual const char* getEventName()	const = 0; 
		virtual int getCategoryFlags()		const = 0; // Returns categories which this event is included in
		virtual std::string toString() const { return getEventName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}

	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	private:
		Event& m_Event;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (m_Event.getEventType() == T::getStaticType())
			{
				m_Event.m_handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	};

	// For printing events to ostream
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}


}