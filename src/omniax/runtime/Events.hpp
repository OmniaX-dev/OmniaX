#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <omniax/utils/Types.hpp>
#include <omniax/utils/BaseObject.hpp>

namespace ox
{
	struct tEventTypes
	{
		inline static constexpr uint8_t Invalid = 0;
		inline static constexpr uint8_t KeyPressed = 1;
		inline static constexpr uint8_t KeyReleased = 2;
		inline static constexpr uint8_t MousePressed = 3;
		inline static constexpr uint8_t MouseReleased = 4;
		inline static constexpr uint8_t MouseMoved = 5;
	}; 
	class Window;
	class Application;
	class Event : public BaseObject
	{
		public:
			Window& window;
			Application& app;
			const uint8_t type;

			inline Event(Window& _window, Application& _app, uint8_t _type) : window(_window), app(_app), type(_type) {  }
	};

	class KeyEvent : public Event
	{
		public:
			const int32_t key; //TODO: Maybe remap keys in a tKeys struct
			const bool pressed;

			inline KeyEvent(Window& _window, Application& _app, uint8_t _type, int32_t _key, bool _pressed) : Event(_window, _app, _type), key(_key), pressed(_pressed)
			{
				setTypeName("ox::KeyEvent");
				setValid(true);
			}
	};

	class MouseButtonEvent : public Event
	{
		public:
			const int32_t button;
			const bool pressed;
			const int32_t x;
			const int32_t y;

			inline MouseButtonEvent(Window& _window, Application& _app, uint8_t _type, uint8_t _btn, bool _pressed, uint32_t _x, uint32_t _y) : Event(_window, _app, _type), button(_btn), pressed(_pressed), x(_x), y(_y)
			{
				setTypeName("ox::MouseButtonEvent");
				setValid(true);
			}
	};

	class MouseMovedEvent : public Event
	{
		public:
			const int32_t x;
			const int32_t y;

			inline MouseMovedEvent(Window& _window, Application& _app, uint32_t _x, uint32_t _y) : Event(_window, _app, tEventTypes::MouseMoved), x(_x), y(_y)
			{
				setTypeName("ox::MouseMovedEvent");
				setValid(true);
			}
	};
}

#endif