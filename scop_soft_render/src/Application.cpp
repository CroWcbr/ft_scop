#include <GLFW/glfw3.h>

#include <iomanip>
#include <cstring>
#include <iostream>

#include "Application.hpp"
#include "Window.hpp"
#include "FPS.hpp"
#include "Vector.hpp"

const unsigned char white[3] = { 255, 255, 255 };
const unsigned char red[3]   = { 255, 0, 0 };
const unsigned char green[3] = { 0, 255, 0 };
const unsigned char blue[3] = { 0, 0, 255 };

// void drawTriangle(ft::Vec3 t0, ft::Vec3 t1, ft::Vec3 t2, \
// 				ft::Vec2 uv0, ft::Vec2 uv1, ft::Vec2 uv2, \
// 				unsigned char* image, \
// 				TGAimage* tga_image, \
// 				int* zbuffer, \
// 				const unsigned char *color, \
// 				unsigned int image_resolution, \
// 				float ity0, float ity1, float ity2)
// {

// }

namespace Scop
{

Application::Application()
{

}

Application::~Application()
{
	delete model;
	delete tga_image;
	delete image;
	delete zbuffer;
	delete m_pWindow;
}

int Application::start(unsigned int window_width, unsigned int window_height, const char* title, const char* path_model_obj, const char* path_texture_tga)
{
	Model*	model = new Model(path_model_obj);
	if (model->getResultCode())
	{
		return model->getResultCode();
	}
	TGAimage*	tga_image = new TGAimage(path_texture_tga);
	if (tga_image->getResultCode())
	{
		return tga_image->getResultCode();
	}
	m_pWindow = new Window(title, window_width, window_height);
	if (m_pWindow->getResultCode())
	{
		return m_pWindow->getResultCode();
	}
//FPS//

	while (!m_pWindow->getIsClosed())
	{
		FPS::start();

		if (image_resolution != m_pWindow->getResolution())
		{
			delete image;
			delete zbuffer;
			image_resolution = m_pWindow->getResolution();
			image_size = image_resolution * image_resolution * bytespp;
			image = new unsigned char[image_size];
			zbuffer = new double[image_resolution * image_resolution];
		}
		memset(image, 0, image_size);
		memset(zbuffer, std::numeric_limits<int>::min(), image_resolution * image_resolution * sizeof(int));

		Vec3 world_coords[3];
		for (size_t i = 0; i < model->get_f_v().size(); ++i)
		{
			std::vector<int> face = model->get_f_v()[i];
			for (size_t j = 0; j < 3; j++)
			{
				world_coords[j] = model->get_v()[face[j]];
			}
			draw_line_triange(world_coords);
		}

		m_pWindow->on_update(image, image_resolution);

		FPS::end();
		FPS::calculate_fps();
	}
	return 0;
}

void	Application::draw_point_triangle(Vec3* world_coords)
{

}

void	Application::draw_line_triange(Vec3* world_coords)
{
	line((world_coords[0].x() + 1.) * image_resolution / 2., (world_coords[0].y() + 1.) * image_resolution / 2., \
			(world_coords[1].x() + 1.) * image_resolution / 2., (world_coords[1].y() + 1.) * image_resolution / 2.);
	line((world_coords[1].x() + 1.) * image_resolution / 2., (world_coords[1].y() + 1.) * image_resolution / 2., \
			(world_coords[2].x() + 1.) * image_resolution / 2., (world_coords[2].y() + 1.) * image_resolution / 2.);
	line((world_coords[2].x() + 1.) * image_resolution / 2., (world_coords[2].y() + 1.) * image_resolution / 2., \
			(world_coords[0].x() + 1.) * image_resolution / 2., (world_coords[0].y() + 1.) * image_resolution / 2.);
}

void	Application::line(int x0, int y0, int x1, int y1)
{
	const unsigned char color[3] = { 255, 255, 255 };
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
		if (x0 < 0 || y0 < 0 || x0 >= image_resolution || y0 >= image_resolution)
			return;
		memcpy(image + static_cast<int>(y0 * image_resolution + x0) * 3, color, 3);
	}
}



}
