#pragma once

#include "Model.hpp"
#include "TGAimage.hpp"

namespace Scop
{

	class Application
	{
	private:
		class Window*		m_pWindow = nullptr;
		Model*				m_pModel = nullptr;
		TGAimage*			m_pTga_image = nullptr;
		unsigned char*		m_pImage = nullptr;
		double*				m_pZbuffer = nullptr;
		unsigned int		m_image_size = 0;
		unsigned int		m_image_resolution = 0;	//Texture a multiple of 100
		unsigned int		m_bytespp = 3;			//RGB
		unsigned char*		m_pWhite;

	private:
		void			draw_point_triangle(Vec4f* world_coords);
		void			draw_line_triange(Vec4f* world_coords);
		void			line(int x0, int y0, int x1, int y1);

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
