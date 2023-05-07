#pragma once

#include "Model.hpp"
#include "TGAimage.hpp"

namespace Scop
{

	class Application
	{
	private:
		class Window*	m_pWindow = nullptr;
		Model*			model = nullptr;
		TGAimage*		tga_image = nullptr;
		unsigned char*	image = nullptr;
		double*			zbuffer = nullptr;
		unsigned int	image_size = 0;
		unsigned int	image_resolution = 0;	//Texture a multiple of 100
		unsigned int	bytespp = 3;			//RGB

	private:
		void			draw_point_triangle(Vec3* world_coords);
		void			draw_line_triange(Vec3* world_coords);
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
