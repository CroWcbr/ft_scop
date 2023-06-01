#pragma once

#include "Model.hpp"
#include "TGAimage.hpp"
#include "Camera.hpp"

#include <GLFW/glfw3.h>

namespace Scop
{

	class Application
	{
	protected:
		struct Window*		m_pWindow = nullptr;
		Model*				m_pModel = nullptr;
		TGAimage*			m_pTga_image = nullptr;
		struct IShader*		m_pShader = nullptr;
		unsigned char*		m_pImage = nullptr;
		float*				m_pZbuffer = nullptr;
		unsigned int		m_image_size = 0;
		unsigned int		m_image_resolution = 0;	//Texture a multiple of 100
		unsigned int		m_bytespp = 3;			//RGB
		bool				m_redraw = true;
		void				(Application::*m_pDrawFunction)() = nullptr;

		Camera				m_camera{ Vec3f({0, 0, 0}) };


	public:
		void				draw_model_in_point();
		void				draw_model_in_line();
		void				draw_model_in_simple_triangle_rand_color();

		void				draw_model_in_triangle_Bresenham_algoritm();

	protected:
		void				point(int x, int y, const unsigned char* color);
		void				line(int x0, int y0, int x1, int y1, const unsigned char* color);

		virtual void		on_update() {};

	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application&	operator=(const Application&) = delete;
		Application&	operator=(const Application&&) = delete;

		int				start(unsigned int window_width, \
								unsigned int window_height, \
								const char* title, \
								const char* path_model_obj, \
								const char* path_texture_tga);

	};

}
