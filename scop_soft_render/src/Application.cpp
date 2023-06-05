#include <GLFW/glfw3.h>

#include <iomanip>
#include <cstring>
#include <iostream>
#include <cmath>
#include <limits>

#include "Application.hpp"
#include "Window.hpp"
#include "FPS.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Input.hpp"
#include "Keys.hpp"
#include "Shader.hpp"

namespace Scop
{

Application::Application()
{}

Application::~Application()
{
	delete m_pModel;
	delete m_pTga_image;
	delete m_pImage;
	delete m_pZbuffer;
	delete m_pWindow;
}

int Application::start(unsigned int window_width, unsigned int window_height, const char* title, const std::string& path_model_obj, const std::string& path_texture_tga)
{
	m_pModel = new Model(path_model_obj.c_str());
	if (m_pModel->getResultCode())
	{
		return m_pModel->getResultCode();
	}
	m_pTga_image = new TGAimage(path_texture_tga.c_str());
	if (m_pTga_image->getResultCode())
	{
		return m_pTga_image->getResultCode();
	}
	m_pWindow = new Window(title, window_width, window_height);
	if (m_pWindow->getResultCode())
	{
		return m_pWindow->getResultCode();
	}
	m_pShader = new Shader_only_vertex(*m_pModel, *m_pTga_image, m_camera);
	m_pDrawFunction = &Application::draw_model_in_line;

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
			m_pZbuffer = new float[m_image_resolution * m_image_resolution];
			m_redraw = true;
			m_camera.change_viewport_matrix(m_image_resolution);
		}

		if (m_rotate)
		{
			m_redraw = true;
			m_camera.set_view_rotation({0, 10, 0});
		}

		if (m_redraw)
		{
			m_redraw = false;
			memset(m_pImage, 0, m_image_size);
			std::fill_n(m_pZbuffer, m_image_resolution * m_image_resolution, std::numeric_limits<float>::max());
			for (size_t i = 0; i < m_pModel->get_f_v().size(); ++i)
			{
							std::cout << i << std::endl;
				for (int j : {0, 1, 2})
				{
					m_pShader->vertex(i, j);
				}
				(this->*m_pDrawFunction)();
			}
			m_pWindow->on_update(m_pImage, m_image_resolution);
		}
		m_pWindow->event();
		on_update();
		FPS::delay();
		FPS::end();
		FPS::calculate_fps();
	}
	return 0;
}

void	Application::draw_model_in_point()
{
	for (int i = 0; i < 3; ++i)
	{
		point(m_pShader->mvpv[i].x() / m_pShader->mvpv[i].w(), m_pShader->mvpv[i].y() / m_pShader->mvpv[i].w(), m_pShader->color);
	}
}

void	Application::point(int x, int y, const unsigned char* color)
{
	if (x < 0 || y < 0 || x >= m_image_resolution || y >= m_image_resolution)
		return;
	memcpy(m_pImage + static_cast<int>(y * m_image_resolution + x) * m_bytespp, color, m_bytespp);
}

void	Application::draw_model_in_line()
{
	std::cout << m_pShader->mvpv[0] << std::endl;
	std::cout << m_pShader->mvpv[1] << std::endl;
	std::cout << m_pShader->mvpv[2] << std::endl;
	int x0 = m_pShader->mvpv[0].x() / m_pShader->mvpv[0].w();
	int y0 = m_pShader->mvpv[0].y() / m_pShader->mvpv[0].w();
	int x1 = m_pShader->mvpv[1].x() / m_pShader->mvpv[1].w();
	int y1 = m_pShader->mvpv[1].y() / m_pShader->mvpv[1].w();
	int x2 = m_pShader->mvpv[2].x() / m_pShader->mvpv[2].w();
	int y2 = m_pShader->mvpv[2].y() / m_pShader->mvpv[2].w();

	line(x0, y0, x1, y1, m_pShader->color);
	line(x1, y1, x2, y2, m_pShader->color);
	line(x2, y2, x0, y0, m_pShader->color);
}

void	Application::line(int x0, int y0, int x1, int y1, const unsigned char* color)
{
	if ((x0 < 0 && x1 < 0) || \
		(x0 >= m_image_resolution && x1 >= m_image_resolution) || \
		(y0 < 0 && y1 < 0) || \
		(y0 >= m_image_resolution && y1 >= m_image_resolution))
		return;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;

	std::cout <<"!!!\t" << x0 << "\t" << x1 << "\t" << y0 << "\t" << y1 << std::endl;
	while (x0 != x1 || y0 != y1)
	{
		std::cout << "x\t" << x0 << "\t" << x1 << std::endl;
		std::cout << "y\t" << y0 << "\t" << y1 << std::endl;
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
			continue;
		memcpy(m_pImage + (y0 * m_image_resolution + x0) * m_bytespp, color, m_bytespp);
	}
}

Vec3f	barycentric(const Vec2f A, const Vec2f B, const Vec2f C, const Vec2f P)
{
	Vec3f s[2];
	for (int i=2; i--; ) {
		s[i][0] = C[i]-A[i];
		s[i][1] = B[i]-A[i];
		s[i][2] = A[i]-P[i];
	}
	Vec3f u = s[0] ^ s[1];
	if (std::abs(u[2]) > 1e-2)
		return Vec3f({1.f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z()});
	return Vec3f({-1,1,1});
}

void	Application::draw_model_barycentric_simple()
{
	Vec4f* tri = m_pShader->mvpv;
	Vec4f pts[3] = { tri[0], tri[1], tri[2] };
	Vec2f pts2[3] = { {(pts[0] / pts[0][3]).x(), (pts[0] / pts[0][3]).y()},
					{(pts[1] / pts[1][3]).x(), (pts[1] / pts[1][3]).y()},
					{(pts[2] / pts[2][3]).x(), (pts[2] / pts[2][3]).y()} };
	Vec3f bc0 = barycentric(pts2[0], pts2[1], pts2[2], pts2[0]);
	Vec3f bc1 = barycentric(pts2[0], pts2[1], pts2[2], pts2[1]);
	Vec3f bc2 = barycentric(pts2[0], pts2[1], pts2[2], pts2[2]);
	int bboxmin[2] = {static_cast<int>(m_image_resolution - 1), static_cast<int>(m_image_resolution - 1)};
	int bboxmax[2] = {0, 0};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::min(bboxmin[j], static_cast<int>(pts2[i][j]));
			bboxmax[j] = std::max(bboxmax[j], static_cast<int>(pts2[i][j]));
		}
	}
	m_pShader->fragment();
	for (int x = std::max(bboxmin[0], 0); x <= std::min(bboxmax[0], static_cast<int>(m_image_resolution - 1)); x++)
	{
		for (int y = std::max(bboxmin[1], 0); y <= std::min(bboxmax[1], static_cast<int>(m_image_resolution - 1)); y++)
		{
			float frag_depth = pts[0][2] * bc0.x() + pts[1][2] * bc1.y() + pts[2][2] * bc2.z();
			if (frag_depth > m_pZbuffer[x + y * m_image_resolution])
				continue;
			Vec2f p({static_cast<float>(x), static_cast<float>(y)});
			Vec3f bc_screen = barycentric(pts2[0], pts2[1], pts2[2], p);
			if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0)
				continue;
			m_pZbuffer[x + y * m_image_resolution] = frag_depth;
			memcpy(m_pImage + static_cast<int>(y * m_image_resolution + x) * m_bytespp, m_pShader->color, m_bytespp);
		}
	}
}

void	Application::draw_model_barycentric_full()
{
	Vec4f* tri = m_pShader->mvpv;
	Vec4f pts[3] = { tri[0], tri[1], tri[2] };
	Vec2f pts2[3] = { {pts[0][0]/pts[0][3] , pts[0][1]/pts[0][3]}, \
						{pts[1][0]/pts[1][3] , pts[1][1]/pts[1][3]},
						{pts[2][0]/pts[2][3] , pts[2][1]/pts[2][3]}};
	int bboxmin[2] = {static_cast<int>(m_image_resolution - 1), static_cast<int>(m_image_resolution - 1)};
	int bboxmax[2] = {0, 0};
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			bboxmin[j] = std::min(bboxmin[j], static_cast<int>(pts2[i][j]));
			bboxmax[j] = std::max(bboxmax[j], static_cast<int>(pts2[i][j]));
		}
	}
	for (int x = std::max(bboxmin[0], 0); x <= std::min(bboxmax[0], static_cast<int>(m_image_resolution - 1)); ++x)
	{
		for (int y = std::max(bboxmin[1], 0); y <= std::min(bboxmax[1], static_cast<int>(m_image_resolution - 1)); ++y)
		{
			Vec3f bc_screen = barycentric(pts2[0], pts2[1], pts2[2], {static_cast<float>(x), static_cast<float>(y)});
			if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0)
				continue;
			Vec3f bc_clip   = {bc_screen.x() / pts[0][3], bc_screen.y() / pts[1][3], bc_screen.z() / pts[2][3]};
			bc_clip = bc_clip / (bc_clip.x() + bc_clip.y() + bc_clip.z());
			float frag_depth = Vec3f({tri[0][2], tri[1][2], tri[2][2]}) * bc_clip;
			if (frag_depth > m_pZbuffer[x + y * m_image_resolution])
				continue;
			m_pShader->fragment(bc_clip);
			m_pZbuffer[x + y * m_image_resolution] = frag_depth;
			memcpy(m_pImage + (y * m_image_resolution + x) * m_bytespp, m_pShader->color, m_bytespp);
		}
	}
}

}
