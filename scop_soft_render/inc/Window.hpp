#pragma once

#include <string>

struct GLFWwindow;

namespace Scop
{


	class Window
	{
	public:
		GLFWwindow*		m_pWindow = nullptr;
		unsigned int	textureID;

		std::string		m_title;
		unsigned int	m_width;
		unsigned int	m_height;
		int				resultCode;

		int				init();

	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window() = delete;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(const Window&&) = delete;
		
		const int		getResultCode() const;
		const void		on_update(unsigned char* data) const;
	};

}
