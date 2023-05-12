#pragma once

#include "Keys.hpp"

namespace Scop
{
	class Input
	{
	private:
		static bool		m_keys_pressed[];
		static bool		m_key_event;

	public:
		static bool		IsKeyPressed(const KeyCode key_code);
		static bool		IsKeyEvent();
		static void		ClearKeyEvent();
		static void		PressKey(const int key_code);
		static void		ReleaseKey(const int key_code);



	};

}
