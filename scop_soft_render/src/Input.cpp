#include "Input.hpp"

#include <cstddef>

namespace Scop
{
	bool	Input::m_keys_pressed[static_cast<unsigned int>(KeyCode::KEY_LAST) + 1] = {};
	bool	Input::m_key_event = false;
	// bool	Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};
	// bool	Input::m_mouse_buttons_event = false;
	// int		Input::m_x = 0;
	// int		Input::m_y = 0;

	bool	Input::IsKeyPressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)];
	}

	void	Input::PressKey(const int key_code)
	{
		m_key_event = true;
		m_keys_pressed[key_code] = true;
	}

	void	Input::ReleaseKey(const int key_code)
	{
		m_keys_pressed[key_code] = false;
	}

	bool	Input::IsKeyEvent()
	{
		return m_key_event;
	}

	void	Input::ClearKeyEvent()
	{
		m_key_event = false;
	}

	// bool	Input::IsMouseButtonPressed(const MouseButton mouse_button)
	// {
	// 	return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
	// }

	// void	Input::PressMouseButton(const int mouse_button)
	// {
	// 	m_mouse_buttons_event = true;
	// 	m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
	// }

	// void	Input::ReleaseMouseButton(const int mouse_button)
	// {
	// 	m_mouse_buttons_event = false;
	// 	m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
	// }

	// bool	Input::IsMouseButtonEvent()
	// {
	// 	return m_mouse_buttons_event;
	// }

	// void	Input::ClearMouseButtonEvent()
	// {
	// 	m_mouse_buttons_event = false;
	// }

	// void	Input::SetMousePosition(const double x, const double y)
	// {

	// 		m_x = x;
	// 		m_y = y;
	// }

}
