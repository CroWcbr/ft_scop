#pragma once

#include "Keys.hpp"

namespace Scop
{
	class Input
	{
	private:
		static bool		m_keys_pressed[];
		static bool		m_key_event;
	// 	static bool		m_mouse_buttons_pressed[];
	// 	static bool		m_mouse_buttons_event;
	// public:
	// 	static int		m_x;
	// 	static int		m_y;

	public:
		static bool		IsKeyPressed(const KeyCode key_code);
		static bool		IsKeyEvent();
		static void		ClearKeyEvent();
		static void		PressKey(const int key_code);
		static void		ReleaseKey(const int key_code);

		// static bool		IsMouseButtonPressed(const MouseButton mouse_button);
		// static bool		IsMouseButtonEvent();
		// static void		ClearMouseButtonEvent();
		// static void		PressMouseButton(const int mouse_button);
		// static void		ReleaseMouseButton(const int mouse_button);
		// static void		SetMousePosition(const double x, const double y);
	};

}
