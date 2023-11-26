#pragma once

#include "event.h"

namespace Wyvern {

	class WindowResizeEvent : public Event
	{
	private:
		int m_Width, m_Height;
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int geHeight() const { return m_Height; }

		std::string toString() const override // overrides pure virtual function
		{
			std::stringstream ss;
			ss << "Event: WindowResizeEvent ( Width: " << m_Width << ", Height: " << m_Height << ")";
			return ss.str();
		}

		EVENT_TYPE(WindowResize)
		EVENT_CATEGORY(CategoryApplication)
	};


	class WindowCloseEvent : public Event
	{
	private:
	public:
		WindowCloseEvent() {};

		std::string toString() const override // overrides pure virtual function
		{
			std::stringstream ss;
			ss << "Event: WindowCloseEvent";
			return ss.str();
		}

		EVENT_TYPE(WindowClose)
		EVENT_CATEGORY(CategoryApplication)
	};






}