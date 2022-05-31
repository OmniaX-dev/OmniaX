#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <GLFW/glfw3.h>
#include <omniax/utils/Geometry.hpp>

namespace ox
{
	struct tInputStates
	{
		inline static constexpr uint16_t None = 0x0000;
		inline static constexpr uint16_t Failed = 0x0001;
		inline static constexpr uint16_t JoystickNotFound = 0x00FF;

		inline static constexpr uint16_t KeyPressed = 0x0100;
		inline static constexpr uint16_t MousePressed = 0x0101;
		inline static constexpr uint16_t JoystickPressed = 0x0102;
	};
	
	struct tMouseButtons
	{
		inline static constexpr uint8_t Button1 = 0;
		inline static constexpr uint8_t Button2 = 1;
		inline static constexpr uint8_t Button3 = 2;
		inline static constexpr uint8_t Button4 = 3;
		inline static constexpr uint8_t Button5 = 4;
		inline static constexpr uint8_t Button6 = 5;
		inline static constexpr uint8_t Button7 = 6;
		inline static constexpr uint8_t Button8 = 7;
		inline static constexpr uint8_t Left = tMouseButtons::Button1;
		inline static constexpr uint8_t Right = tMouseButtons::Button2;
		inline static constexpr uint8_t Middle = tMouseButtons::Button3;
	};

	struct tJoystickButtons
	{
		inline static constexpr uint8_t A = GLFW_GAMEPAD_BUTTON_A;
		inline static constexpr uint8_t B = GLFW_GAMEPAD_BUTTON_B;
		inline static constexpr uint8_t X = GLFW_GAMEPAD_BUTTON_X;
		inline static constexpr uint8_t Y = GLFW_GAMEPAD_BUTTON_Y;

		inline static constexpr uint8_t Cross = GLFW_GAMEPAD_BUTTON_CROSS;
		inline static constexpr uint8_t Circle = GLFW_GAMEPAD_BUTTON_CIRCLE;
		inline static constexpr uint8_t Square = GLFW_GAMEPAD_BUTTON_SQUARE;
		inline static constexpr uint8_t Triangle = GLFW_GAMEPAD_BUTTON_TRIANGLE;

		inline static constexpr uint8_t Back = GLFW_GAMEPAD_BUTTON_BACK;
		inline static constexpr uint8_t Start = GLFW_GAMEPAD_BUTTON_START;
		inline static constexpr uint8_t Guide = GLFW_GAMEPAD_BUTTON_GUIDE;

		inline static constexpr uint8_t RT = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
		inline static constexpr uint8_t RB = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
		inline static constexpr uint8_t LT = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
		inline static constexpr uint8_t LB = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;

		inline static constexpr uint8_t DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP;
		inline static constexpr uint8_t DPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
		inline static constexpr uint8_t DPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
		inline static constexpr uint8_t DPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;

		inline static constexpr uint8_t LastButton = GLFW_GAMEPAD_BUTTON_LAST;

		inline static constexpr uint8_t AxisLeftX = GLFW_GAMEPAD_AXIS_LEFT_X;
		inline static constexpr uint8_t AxisLeftY = GLFW_GAMEPAD_AXIS_LEFT_Y;
		inline static constexpr uint8_t AxisLeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
		inline static constexpr uint8_t AxisRightX = GLFW_GAMEPAD_AXIS_RIGHT_X;
		inline static constexpr uint8_t AxisRightY = GLFW_GAMEPAD_AXIS_RIGHT_Y;
		inline static constexpr uint8_t AxisRightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;

		inline static constexpr uint8_t LastAxis = GLFW_GAMEPAD_AXIS_LAST;
	};

	struct tJoystickInfo
	{
		JoystickID id;
		String name;
	};

	class Input
	{
		public:
			static bool init(GLFWwindow* window);
			static uint16_t getKeyState(int32_t keyCode);
			static uint16_t getMouseButtonState(uint8_t mouseButton);
			static uint16_t getJoystickButtonState(JoystickID joystick, uint8_t button);
			static float getJoystickAxisValue(JoystickID joystick, uint8_t axis);
			static Vec2 getMousePosition(bool zero_if_out_of_window = true);
			static tJoystickInfo getJoystick(uint8_t index = 0);
			static tJoystickInfo getFirstJoystick(void);

			inline static bool isKeyPressed(int32_t key) { return Input::getKeyState(key) == tInputStates::KeyPressed; }
			inline static bool isMousePressed(uint8_t button) { return Input::getMouseButtonState(button) == tInputStates::MousePressed; }
			inline static bool isJoystickPressed(JoystickID joystick, uint8_t button) { return Input::getJoystickButtonState(joystick, button) == tInputStates::JoystickPressed; }
			
			static inline bool isInitialized(void) { return m_window != nullptr; }

		private:
			inline static GLFWwindow* m_window { nullptr };
			inline static std::vector<tJoystickInfo> s_connectedJoysticks;
	};
}

#endif