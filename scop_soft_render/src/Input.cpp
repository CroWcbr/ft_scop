#include "Input.hpp"

#include <cstddef>

namespace Scop
{
	bool	Input::m_keys_pressed[static_cast<unsigned int>(KeyCode::KEY_LAST) + 1] = {};
	bool	Input::m_key_event = false;

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

}
