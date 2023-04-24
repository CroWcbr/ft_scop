#pragma once

#include "Model.hpp"

namespace Scop
{

	class Application
	{
	private:
		class Window*	m_pWindow;
		unsigned char*	image = nullptr;
		unsigned int	image_size;
		unsigned int	image_resolution;	//Texture a multiple of 100
		unsigned int	bytespp = 3;		//RGB

	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(const Application&&) = delete;

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title, Model* model);

	};

}
