#include "Application.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <math.h>

namespace Scop
{

Application::Application()
{
}

Application::~Application()
{
}

int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
{
	m_pWindow = new Window(title, window_width, window_height);
	if (m_pWindow->getResultCode())
	{
		return m_pWindow->getResultCode();
	}


	unsigned char* data = new unsigned char[window_width * window_height * 3];
	while (true)
	{
		double time = glfwGetTime();
		for (int i = 0; i < window_width * window_height * 3; i += 3)
		{
			int r = (int) (255 * (time - floor(time)));
			data[i] = r;      // R
			data[i + 1] = 0;  // G
			data[i + 2] = 0;  // B
		}

		m_pWindow->on_update(data);
	}

}

}

