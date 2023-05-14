#include "Application.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include <string>

class ApplicationEditor : public Scop::Application
{
	virtual void	on_update() override
	{
		if (Scop::Input::IsKeyEvent())
		{
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_1))
			{
				std::cout << "Application KEY_1" << std::endl;
				m_pShader = new Scop::Shader_only_vertex(*m_pModel);
				m_pDrawFunction = &Application::draw_model_in_point;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_2))
			{
				std::cout << "Application KEY_2" << std::endl;
				m_pShader = new Scop::Shader_only_vertex(*m_pModel);
				m_pDrawFunction = &Application::draw_model_in_line;
			}

			// if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_A))
			// {
			// 	std::cout << "Application KEY_W" << std::endl;

			// 	camera_rotation.y() += 10;
			// 	m_camera.set_rotation(camera_rotation);
			// }
			// if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_D))
			// {
			// 	std::cout << "Application KEY_W" << std::endl;

			// 	camera_rotation.y() -= 10;
			// 	m_camera.set_rotation(camera_rotation);
			// }
			// if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_J))
			// {
			// 	std::cout << "Application KEY_J" << std::endl;
			// 	camera_position.x() += 0.1;
			// 	m_camera.set_position(camera_position);
			// }
			// if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_L))
			// {
			// 	std::cout << "Application KEY_L" << std::endl;
			// 	camera_position.x() -= 0.1;
			// 	m_camera.set_position(camera_position);
			// }

			Scop::Input::ClearKeyEvent();
			m_redraw = true;
		}
	}
};

int main(int argc, char **argv)
{
	const char*	path_model_obj = "../blender/african_head/african_head.obj";
	// const char*	path_model = "../blender/african_head/reconstructed_head.obj";

	const char*	path_texture_tga = "../blender/african_head/african_head_diffuse.tga";
	// const char*	path_texture_tga = "../blender/cat.tga";
	// const char*	path_texture_tga = "../blender/output2.tga";

	ApplicationEditor*	scop = new ApplicationEditor();
	int returnCode = scop->start(800, 800, "test", path_model_obj, path_texture_tga);

	delete scop;

	return returnCode;
}
