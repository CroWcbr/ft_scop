#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Scop
{

Window::Window(std::string title, const unsigned int width, const unsigned int height)
	: m_title(std::move(title))
	, m_width(width)
	, m_height(height)
{
	resultCode = init();
}

Window::~Window()
{
}

int Window::init()
{
	if (!glfwInit())
	{
		std::cerr << "Can't initialize GLFW!" << std::endl;
		return -1;
	}
	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// glfwOpenWindowHint(GLFW_WINDOW_RESIZABLE, GL_FALSE);

	m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if (!m_pWindow)
	{
		std::cerr << "Can't initialize window [" << m_title << "] with size " << m_width << "x" << m_height << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_pWindow);
	std::cout << "Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << (char*)glGetString(GL_VERSION) << std::endl;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return 0;
}

const int	Window::getResultCode() const 
{ 
	return resultCode;
}

const void	Window::on_update(unsigned char* data) const
{
	glClearColor(0.33f, 0.33f, 0.33f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

}
