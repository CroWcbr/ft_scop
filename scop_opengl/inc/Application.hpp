#pragma once

#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "TGAimage.hpp"

namespace Scop
{

	class Application
	{
	protected:
		struct Window*			m_pWindow = nullptr;
		Model*					m_pModel = nullptr;
		TGAimage*				m_pTga_image = nullptr;
		class ShaderProgram*	m_pShader = nullptr;

	protected:
		int						m_mode = 0;



		unsigned char*		m_pImage = nullptr;

		unsigned int		m_image_resolution = 0;	//Texture a multiple of 100

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
