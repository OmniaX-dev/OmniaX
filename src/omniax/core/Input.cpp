#include "Input.hpp"

namespace ox
{
	bool Input::init(GLFWwindow* window)
	{
		Input::m_window = window;

		for (uint8_t i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++)
		{
			if (glfwJoystickIsGamepad(i))
			{
				Input::s_connectedJoysticks.push_back({ (JoystickID)i, glfwGetGamepadName(i) });
			}
		}
		return true;
	}

	uint16_t Input::getKeyState(int32_t keyCode)
	{
		if (!Input::isInitialized()) return tInputStates::Failed;
		int32_t state = glfwGetKey(Input::m_window, keyCode);
		if (state == GLFW_PRESS) return tInputStates::KeyPressed;
		return tInputStates::None;
	}

	uint16_t Input::getMouseButtonState(uint8_t mouseButton)
	{
		if (!Input::isInitialized()) return tInputStates::Failed;
		int32_t state = glfwGetMouseButton(Input::m_window, mouseButton);
		if (state == GLFW_PRESS) return tInputStates::MousePressed;
		return tInputStates::None;
	}

	uint16_t Input::getJoystickButtonState(JoystickID joystick, uint8_t button)
	{
		if (!Input::isInitialized()) return tInputStates::Failed;
		GLFWgamepadstate state;
		if (glfwGetGamepadState(joystick, &state))
		{
			if (state.buttons[button])
				return tInputStates::JoystickPressed;
		}
		return tInputStates::None;
	}

	float Input::getJoystickAxisValue(JoystickID joystick, uint8_t axis)
	{
		if (!Input::isInitialized()) return 0.0f;
		GLFWgamepadstate state;
		if (glfwGetGamepadState(joystick, &state))
			return state.axes[axis];
		return 0.0f;
	}

	Vec2 Input::getMousePosition(bool zero_if_out_of_window)
	{
		if (!Input::isInitialized()) return { 0.0f, 0.0f };
		int32_t ww, wh;
		glfwGetWindowSize(Input::m_window, &ww, &wh);
		double xpos, ypos;
		glfwGetCursorPos(Input::m_window, &xpos, &ypos);
		if (zero_if_out_of_window && (xpos < 0 || ypos < 0 || xpos >= ww || ypos >= wh))
		{
			xpos = 0;
			ypos = 0;
		}
		return { (float)xpos, (float)ypos };
	}

	tJoystickInfo Input::getJoystick(uint8_t index)
	{
		if (Input::s_connectedJoysticks.size() == 0 || index >= Input::s_connectedJoysticks.size())
			return { tInputStates::JoystickNotFound, "NOT_FOUND" };
		return Input::s_connectedJoysticks[index];
	}

	tJoystickInfo Input::getFirstJoystick(void)
	{
		return Input::getJoystick(0);
	}
}