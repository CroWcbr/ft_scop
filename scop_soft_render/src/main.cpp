#include "Application.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include <string>
#include <GLFW/glfw3.h>

class ApplicationEditor : public Scop::Application
{
	float	scale = 1;

	virtual void	on_update() override
	{
		if (Scop::Input::IsKeyEvent())
		{
			Vec3f rotation_delta = { 0, 0, 0 };
			Vec3f position_delta = { 0, 0, 0 };

			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_1))
			{
				std::cout << "Application KEY_1" << std::endl;
				m_pShader = new Scop::Shader_only_vertex(*m_pModel, *m_pTga_image, m_camera);
				m_pDrawFunction = &Application::draw_model_in_point;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_2))
			{
				std::cout << "Application KEY_2" << std::endl;
				m_pShader = new Scop::Shader_only_vertex(*m_pModel, *m_pTga_image, m_camera);
				m_pDrawFunction = &Application::draw_model_in_line;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_3))
			{
				std::cout << "Application KEY_3" << std::endl;
				m_pShader = new Scop::Shader_rand_color(*m_pModel, *m_pTga_image, m_camera);
				m_pDrawFunction = &Application::draw_model_in_simple_triangle_rand_color_barycentric;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_4))
			{
				std::cout << "Application KEY_4" << std::endl;
				m_pShader = new Scop::Shader_test(*m_pModel, *m_pTga_image, m_camera);
				m_pDrawFunction = &Application::draw_model_test;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_5))
			{
				std::cout << "Application KEY_5" << std::endl;
				m_pShader = new Scop::Shader_test(*m_pModel, *m_pTga_image, m_camera);
				m_pDrawFunction = &Application::draw_model_in_simple_triangle_rand_color_barycentric;
			}

			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_TAB))
			{
				std::cout << "Application KEY_TAB" << std::endl;
				m_camera.change_projection_matrix();
			}

			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_F))
			{
				std::cout << "Application KEY_F" << std::endl;
				position_delta.x() -= 0.05;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_H))
			{
				std::cout << "Application KEY_H" << std::endl;
				position_delta.x() += 0.05;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_G))
			{
				std::cout << "Application KEY_G" << std::endl;
				position_delta.y() -= 0.05;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_T))
			{
				std::cout << "Application KEY_T" << std::endl;
				position_delta.y() += 0.05;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_Y))
			{
				std::cout << "Application KEY_Y" << std::endl;
				position_delta.z() += 0.05;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_R))
			{
				std::cout << "Application KEY_R" << std::endl;
				position_delta.z() -= 0.05;
			}

			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_W))
			{
				std::cout << "Application KEY_W" << std::endl;
				rotation_delta.x() -= 10;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_S))
			{
				std::cout << "Application KEY_S" << std::endl;
				rotation_delta.x() += 10;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_A))
			{
				std::cout << "Application KEY_A" << std::endl;
				rotation_delta.y() -= 10;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_D))
			{
				std::cout << "Application KEY_D" << std::endl;
				rotation_delta.y() += 10;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_Q))
			{
				std::cout << "Application KEY_Q" << std::endl;
				rotation_delta.z() += 10;
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_E))
			{
				std::cout << "Application KEY_E" << std::endl;
				rotation_delta.z() -= 10;
			}

			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_EQUAL))
			{
				std::cout << "Application KEY_EQUAL" << std::endl;
				scale += 0.1;
				m_camera.set_model_scale(scale);
			}
			if (Scop::Input::IsKeyPressed(Scop::KeyCode::KEY_MINUS))
			{
				std::cout << "Application KEY_MINUS" << std::endl;
				scale -= 0.1;
				m_camera.set_model_scale(scale);
			}

			m_camera.set_model_rotation_and_position(rotation_delta, position_delta);
			Scop::Input::ClearKeyEvent();
			m_redraw = true;
		}
	}
};

int main(int argc, char **argv)
{
	const char*	path_model_obj = "../blender/african_head/african_head.obj";
	// const char*	path_model_obj = "../blender/teapot.obj";
	// const char*	path_model_obj = "../blender/42.obj";
	// const char*	path_model_obj = "../blender/test.obj";

	const char*	path_texture_tga = "../blender/african_head/african_head_diffuse.tga";
	// const char*	path_texture_tga = "../blender/cat.tga";
	// const char*	path_texture_tga = "../blender/output2.tga";

	ApplicationEditor*	scop = new ApplicationEditor();
	int returnCode = scop->start(400, 400, "test", path_model_obj, path_texture_tga);

	delete scop;

	return returnCode;
}
