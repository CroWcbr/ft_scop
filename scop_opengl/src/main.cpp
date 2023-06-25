#include "Application.hpp"
#include "Input.hpp"
#include <string>
#include <GLFW/glfw3.h>

class ApplicationEditor : public Scop::Application
{

	virtual void	on_update() override
	{
		if (Scop::Input::IsKeyEvent())
		{
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_1))
			{
				std::cout << "Application KEY_1" << std::endl;
				m_mode = 0;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_2))
			{
				std::cout << "Application KEY_2" << std::endl;
				m_mode = 1;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_3))
			{
				std::cout << "Application KEY_3" << std::endl;
				m_mode = 2;
			}
		}
	}
};

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	const char*	path_model_obj = "../blender/teapot.obj";
	const char*	path_texture_tga = "../blender/cat.tga";

	ApplicationEditor*	scop = new ApplicationEditor();
	int returnCode = scop->start(800, 800, "test", path_model_obj, path_texture_tga);
	delete scop;

	return returnCode;
}
