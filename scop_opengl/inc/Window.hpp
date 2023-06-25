#pragma once

#include <string>

struct GLFWwindow;

namespace Scop
{

	class Window
	{
	public:
		struct WindowData
		{
			unsigned int	width;
			unsigned int	height;
			unsigned int	resolution;
			bool			isClosed;

			WindowData(unsigned int	width, unsigned int	height)
				: width(width)
				, height(height)
				, isClosed(false)
				{}
		};
		std::string			m_title;
		WindowData			m_data;
		GLFWwindow*			m_pWindow = nullptr;
		unsigned int		textureID;
		int					m_resultCode;

		int					init();
		void				init_callback();
	
	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window() = delete;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(const Window&&) = delete;

		void				on_update(const unsigned char* image, const unsigned int image_resolution) const;
		inline int			getResultCode() const { return m_resultCode; }
		inline int			getResolution() const { return m_data.resolution; }
		inline bool			getIsClosed() const { return m_data.isClosed; }
	};

}
