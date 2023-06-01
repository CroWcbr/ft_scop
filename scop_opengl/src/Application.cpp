#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../inc/Application.hpp"
#include "Window.hpp"
#include "FPS.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

namespace Scop
{

const char* vertex_shader =
	R"(#version 330
	layout(location = 0) in vec3 position;
	void main()
	{
		gl_Position = vec4(position, 1.0);
	}
	)";

const char* fragment_shader =
	R"(#version 330
	out vec4 FragColor;
	void main()
	{
			FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	)";

Application::Application()
{}

Application::~Application()
{
	delete m_pModel;
	delete m_pTga_image;
	delete m_pShader;
	delete m_pWindow;
}

int Application::start(unsigned int window_width, unsigned int window_height, const char* title, const char* path_model_obj, const char* path_texture_tga)
{
	m_pModel = new Model(path_model_obj);
	if (m_pModel->getResultCode())
	{
		return m_pModel->getResultCode();
	}
	m_pTga_image = new TGAimage(path_texture_tga);
	if (m_pTga_image->getResultCode())
	{
		return m_pTga_image->getResultCode();
	}
	m_pWindow = new Window(title, window_width, window_height);
	if (m_pWindow->getResultCode())
	{
		return m_pWindow->getResultCode();
	}
	m_pShader = new ShaderProgram(vertex_shader, fragment_shader);
	if (m_pShader->getResultCode())
	{
		return m_pShader->getResultCode();
	}

std::vector<float> vertices;
for (auto& a : m_pModel->m_v)
{
	vertices.push_back(a[0]);
	vertices.push_back(a[1]);
	vertices.push_back(a[2]);
}

std::vector<unsigned int> indices;
for (auto& a : m_pModel->m_f_v)
{
	indices.push_back(a[0]);
	indices.push_back(a[1]);
	indices.push_back(a[2]);
}

std::cout << m_pModel->m_v.size() << "\t" << vertices.size() << std::endl;
std::cout <<  m_pModel->m_f_v.size() << "\t" << indices.size() << std::endl;


	VertexArray*	vao = new VertexArray();
	VertexBuffer*	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	IndexBuffer*	ebo = new IndexBuffer(indices.data(), indices.size());

	vao->add_vertex_buffer(*vbo, 3, GL_FLOAT);
	vao->set_index_buffer(*ebo);

	while (!m_pWindow->getIsClosed())
	{
		FPS::start();

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_pShader->bind();
		vao->bind();

		switch (m_mode)
		{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES , indices.size(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 1:
			glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
			break;
		case 2:
			glDrawElements(GL_TRIANGLES , indices.size(), GL_UNSIGNED_INT, nullptr);
			break;
		}

		vao->unbind();
		m_pShader->unbind();

		m_pWindow->on_update(m_pImage, m_image_resolution);
		on_update();
		FPS::end();
		FPS::calculate_fps();
	}

	delete vbo;
	delete ebo;
	delete vao;

	return 0;
}

}