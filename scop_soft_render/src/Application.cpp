#include "Application.hpp"
#include "Window.hpp"

#include <cstring>
#include <iostream>

#include "Vector.hpp"
#include "Model.hpp"
#include <GLFW/glfw3.h>

void line(int x0, int y0, int x1, int y1, unsigned char* image, unsigned int image_resolution)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;
	while (x0 != x1 || y0 != y1)
	{
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
		// установка пикселя (x0, y0) в изображении
		if (x0<0 || y0<0 || x0>=image_resolution || y0>=image_resolution)
			return;
		memset(image + (y0 * image_resolution + x0) * 3, 255, 3);
	}
}

namespace Scop
{

Application::Application()
{

}

Application::~Application()
{
	delete image;
	delete m_pWindow;
}

int Application::start(unsigned int window_width, unsigned int window_height, const char* title, Model* model)
{
	m_pWindow = new Window(title, window_width, window_height);
	if (m_pWindow->getResultCode())
	{
		return m_pWindow->getResultCode();
	}

	while (!m_pWindow->getIsClosed())
	{
		if (image_resolution != m_pWindow->getResolution())
		{
			delete image;
			image_resolution = m_pWindow->getResolution();
			image_size = image_resolution * image_resolution * bytespp;
			image = new unsigned char[image_size];
		}

		memset(image, 0, image_size);
		for (int i = 0; i < model->faces_.size(); i++) 
		{
			std::vector<int> face = model->faces_[i];
			for (int j = 0; j < 3; j++)
			{
				Vec3f v0 = model->verts_[face[j]];
				Vec3f v1 = model->verts_[face[(j + 1) % 3]];
				int x0 = (v0.x + 1.) * image_resolution / 2.;
				int y0 = (v0.y + 1.) * image_resolution / 2.;
				int x1 = (v1.x + 1.) * image_resolution / 2.;
				int y1 = (v1.y + 1.) * image_resolution / 2.;
				line(x0, y0, x1, y1, image, image_resolution);
			}
		}
		m_pWindow->on_update(image, image_resolution);
	}
	return 0;
}

}
