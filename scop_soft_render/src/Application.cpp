#include <GLFW/glfw3.h>

#include <iomanip>
#include <cstring>
#include <iostream>
#include <cmath>

#include "Application.hpp"
#include "Window.hpp"
#include "FPS.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

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
	m_pWhite = new unsigned char[m_bytespp];
	memset(m_pWhite, 255, m_bytespp);
}

Application::~Application()
{
	delete m_pModel;
	delete m_pTga_image;
	delete m_pImage;
	delete m_pZbuffer;
	delete m_pWhite;
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

	Mat4 ttt;
	ttt = {1, 0, 0, 0.2,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1};

	// double roll_in_radians = -1;
	// Mat4 rotate_matrix({cos(roll_in_radians), 0, sin(roll_in_radians), 0,
	// 					0, 1, 0, 0,
	// 					-sin(roll_in_radians), 0, cos(roll_in_radians), 0,
	// 					0, 0, 0, 1});

		// const glm::mat3 rotate_matrix_x(
		// 	1, 0, 0,
		// 	0, cos(roll_in_radians), sin(roll_in_radians),
		// 	0, -sin(roll_in_radians), cos(roll_in_radians)
		// );

		// const glm::mat3 rotate_matrix_y(
		// 	cos(pitch_in_radians), 0, -sin(pitch_in_radians),
		// 	0, 1, 0,
		// 	sin(pitch_in_radians), 0, cos(pitch_in_radians)
		// 	);

		// const glm::mat3 rotate_matrix_z(
		// 	cos(yaw_in_radians), sin(yaw_in_radians), 0,
		// 	-sin(yaw_in_radians), cos(yaw_in_radians), 0,
		// 	0, 0, 1
		// 	);

	while (!m_pWindow->getIsClosed())
	{
		FPS::start();

		if (m_image_resolution != m_pWindow->getResolution())
		{
			delete m_pImage;
			delete m_pZbuffer;
			m_image_resolution = m_pWindow->getResolution();
			m_image_size = m_image_resolution * m_image_resolution * m_bytespp;
			m_pImage = new unsigned char[m_image_size];
			m_pZbuffer = new double[m_image_resolution * m_image_resolution];
		}
		memset(m_pImage, 0, m_image_size);
		memset(m_pZbuffer, std::numeric_limits<int>::min(), m_image_resolution * m_image_resolution * sizeof(int));

		Vec4f world_coords[3];
		for (size_t i = 0; i < m_pModel->get_f_v().size(); ++i)
		{
			// std::cout << i << std::endl;
			std::vector<int> face = m_pModel->get_f_v()[i];
			for (size_t j = 0; j < 3; j++)
			{
				// std::cout << m_pModel->get_v()[face[j]] << std::endl;
				world_coords[j] = ttt * Vec4f(m_pModel->get_v()[face[j]], 1);
				// std::cout << world_coords[j] << std::endl;
			}
			draw_line_triange(world_coords);
		}

		m_pWindow->on_update(m_pImage, m_image_resolution);
		std::getchar();
		FPS::end();
		FPS::calculate_fps();
	}
	return 0;
}

void	Application::draw_point_triangle(Vec4f* world_coords)
{
	int x0 = static_cast<int>((world_coords[0].x() + 1.) * m_image_resolution / 2.);
	int y0 = static_cast<int>((world_coords[0].y() + 1.) * m_image_resolution / 2.);
	int x1 = static_cast<int>((world_coords[1].x() + 1.) * m_image_resolution / 2.);
	int y1 = static_cast<int>((world_coords[1].y() + 1.) * m_image_resolution / 2.);
	int x2 = static_cast<int>((world_coords[2].x() + 1.) * m_image_resolution / 2.);
	int y2 = static_cast<int>((world_coords[2].y() + 1.) * m_image_resolution / 2.);
	
	memcpy(m_pImage + static_cast<int>(y0 * m_image_resolution + x0) * m_bytespp, m_pWhite, m_bytespp);
	memcpy(m_pImage + static_cast<int>(y1 * m_image_resolution + x1) * m_bytespp, m_pWhite, m_bytespp);
	memcpy(m_pImage + static_cast<int>(y2 * m_image_resolution + x2) * m_bytespp, m_pWhite, m_bytespp);
}

void	Application::draw_line_triange(Vec4f* world_coords)
{
	int x0 = static_cast<int>((world_coords[0].x() + 1.) * m_image_resolution / 2.);
	int y0 = static_cast<int>((world_coords[0].y() + 1.) * m_image_resolution / 2.);
	int x1 = static_cast<int>((world_coords[1].x() + 1.) * m_image_resolution / 2.);
	int y1 = static_cast<int>((world_coords[1].y() + 1.) * m_image_resolution / 2.);
	int x2 = static_cast<int>((world_coords[2].x() + 1.) * m_image_resolution / 2.);
	int y2 = static_cast<int>((world_coords[2].y() + 1.) * m_image_resolution / 2.);

	line(x0, y0, x1, y1);
	line(x1, y1, x2, y2);
	line(x2, y2, x0, y0);
}

void	Application::line(int x0, int y0, int x1, int y1)
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
		if (x0 < 0 || y0 < 0 || x0 >= m_image_resolution || y0 >= m_image_resolution)
			return;
		memcpy(m_pImage + static_cast<int>(y0 * m_image_resolution + x0) * m_bytespp, m_pWhite, m_bytespp);
	}
}



}
