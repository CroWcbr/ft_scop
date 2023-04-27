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
		int*			zbuffer = nullptr;
		unsigned int	image_size;
		unsigned int	image_resolution;	//Texture a multiple of 100
		unsigned int	bytespp = 3;		//RGB

	const int depth  = 255;

	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(const Application&&) = delete;

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title, const char* path_model_obj, const char* path_texture_tga);

	};

}
