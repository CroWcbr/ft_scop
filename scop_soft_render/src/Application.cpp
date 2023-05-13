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

const int depth  = 255;

Mat4 viewport(int x, int y, int w, int h) {
	// std::cout << x << "\t" << y << "\t" << w << "\t" << h << std::endl;
    Mat4 m = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1};
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    // m[0][3] = w/2;
    // m[1][3] = h/2;
    // m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}


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

	int k = 0;

	while (!m_pWindow->getIsClosed())
	{
		FPS::start();
		// std::cout << k++ << std::endl;

		if (m_image_resolution != m_pWindow->getResolution())
		{
			delete m_pImage;
			delete m_pZbuffer;
			m_image_resolution = m_pWindow->getResolution();
			m_image_size = m_image_resolution * m_image_resolution * m_bytespp;
			m_pImage = new unsigned char[m_image_size];
			m_pZbuffer = new int[m_image_resolution * m_image_resolution];
			m_redraw = true;
		}

		if (m_redraw)
		{
			memset(m_pImage, 0, m_image_size);
			std::fill_n(m_pZbuffer, m_image_resolution * m_image_resolution, std::numeric_limits<int>::min());

			Mat4 view = m_camera.get_view_matrix();

			Mat4 translate_matrix = {
					1, 0, 0, 1,
					0, 1, 0, 1,
					0, 0, 1, 1,
					0, 0, 0, 1};

			Mat4 scale_matrix = {
					m_image_resolution / 2.f, 0, 0, 0,
					0, m_image_resolution / 2.f, 0, 0,
					0, 0, depth / 2.f, 0,
					0, 0, 0, 1};

			Mat4 LookAt = scale_matrix * translate_matrix;

			Vec3f world_coords[3];
			Vec4i screen_coords[3];
			Vec3f light_dir = {0, 0, -1};
			Vec3i screen_coords_test[3];

	Vec3f camera = {0, 0, 3};
	Mat4 Projection = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1};
	Mat4 ViewPort   = viewport(0, 0, m_image_resolution*7/8, m_image_resolution*7/8);
	Projection[3][2] = -1.f/camera.z();
	// Projection[3][2] = 0;

			for (size_t i = 0; i < m_pModel->get_f_v().size(); ++i)
			{
				// std::cout << i << "\t";
				std::vector<int> face = m_pModel->get_f_v()[i];
				Vec4f ttt[3];
				for (size_t j = 0; j < 3; j++)
				{
					// std::cout << m_pModel->get_v()[face[j]] << std::endl;
					// world_coords[j] = view * Vec4f(m_pModel->get_v()[face[j]], 1);

					world_coords[j] = m_pModel->get_v()[face[j]];
					screen_coords[j] = LookAt * view * Vec4f(world_coords[j], 1.f);
					// std::cout << world_coords[j] << std::endl;
					
					ttt[j] = ViewPort * view * Vec4f(world_coords[j], 1.f);
					screen_coords_test[j] = {static_cast<int>(ttt[j].x()/ttt[j].w()), static_cast<int>(ttt[j].y()/ttt[j].w()), static_cast<int>(ttt[j].z()/ttt[j].w())};
					
					// std::cout << screen_coords[j] << std::endl;
				}
				// draw_line_triange_test(screen_coords);
				Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
				n.normalize();
				float intensity = n * light_dir;

				if (intensity > 0) 
				{
					Vec2i uv[3];
					std::vector<int> uv_face = m_pModel->get_f_vt()[i];
					for (int k=0; k<3; k++)
					{
						int idx = uv_face[k];
						uv[k].x() = m_pModel->get_vt()[idx].x() * m_pTga_image->getWidth();
						uv[k].y() = m_pModel->get_vt()[idx].y() * m_pTga_image->getHeight();
					}
					// unsigned char col = static_cast<unsigned char>(intensity * 255);
					// unsigned char color[3] =  {col, col, col};
					// draw_fill_triange_test(screen_coords, color);
					// draw_fill_triange_texture(screen_coords, uv, intensity);
					draw_fill_triange_texture_test(screen_coords_test, uv, intensity);
				}

				// unsigned char color[3] =  {rand()%255, rand()%255, rand()%255};
				// draw_fill_triange_test(screen_coords, color);
			}
			m_redraw = false;
		}

		m_pWindow->on_update(m_pImage, m_image_resolution);
		on_update();
		// std::getchar();
		FPS::end();
		FPS::calculate_fps();
	}
	return 0;
}

void	Application::draw_fill_triange_test(Vec4i* screen_coords, unsigned char color[3])
{
	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};

	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
	if (t0.y()>t1.y()) std::swap(t0, t1);
	if (t0.y()>t2.y()) std::swap(t0, t2);
	if (t1.y()>t2.y()) std::swap(t1, t2);

	int total_height = t2.y()-t0.y();
	for (int i=0; i<total_height; i++) {
		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
		float alpha = (float)i/total_height;
		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;
		if (A.x()>B.x()) std::swap(A, B);
		for (int j=A.x(); j<=B.x(); j++)
		{
			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
			int idx = P.x() + P.y() * m_image_resolution;
			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
				continue;
			if (m_pZbuffer[idx] < P.z())
			{
				m_pZbuffer[idx] = P.z();
				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
			}
		//     image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
		}
	}
}

void	Application::draw_fill_triange_texture(Vec4i* screen_coords, Vec2i* uv, float intensivity)
{
	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};
	Vec2i uv0 = {uv[0].x(), uv[0].y()};
	Vec2i uv1 = {uv[1].x(), uv[1].y()};
	Vec2i uv2 = {uv[2].x(), uv[2].y()};

	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
	if (t0.y()>t1.y()) { std::swap(t0, t1); std::swap(uv0, uv1); }
	if (t0.y()>t2.y()) { std::swap(t0, t2); std::swap(uv0, uv2); }
	if (t1.y()>t2.y()) { std::swap(t1, t2); std::swap(uv1, uv2); }

	int total_height = t2.y()-t0.y();
	for (int i=0; i<total_height; i++)
	{
		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
		float alpha = (float)i/total_height;
		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

		Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
		Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;

		if (A.x()>B.x()) { std::swap(A, B); std::swap(uvA, uvB);}
		for (int j=A.x(); j<=B.x(); j++)
		{
			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
			Vec2i uvP =     uvA +   (uvB-uvA)*phi;
			int idx = P.x() + P.y() * m_image_resolution;
			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
				continue;
			if (m_pZbuffer[idx] < P.z())
			{
				m_pZbuffer[idx] = P.z();
				unsigned char color[3];
				memcpy(color, &m_pTga_image->getTGAimage()[(int)(uvP.x() + uvP.y() * m_pTga_image->getWidth()) * 3], 3);
				for (int i = 0; i < 3; ++i)
					color[i] *= intensivity;
				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
			}
		}
	}
}

void	Application::draw_fill_triange_texture_test(Vec3i* screen_coords, Vec2i* uv, float intensivity)
{
	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};
	Vec2i uv0 = {uv[0].x(), uv[0].y()};
	Vec2i uv1 = {uv[1].x(), uv[1].y()};
	Vec2i uv2 = {uv[2].x(), uv[2].y()};

	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
	if (t0.y()>t1.y()) { std::swap(t0, t1); std::swap(uv0, uv1); }
	if (t0.y()>t2.y()) { std::swap(t0, t2); std::swap(uv0, uv2); }
	if (t1.y()>t2.y()) { std::swap(t1, t2); std::swap(uv1, uv2); }

	int total_height = t2.y()-t0.y();
	for (int i=0; i<total_height; i++)
	{
		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
		float alpha = (float)i/total_height;
		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

		Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
		Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;

		if (A.x()>B.x()) { std::swap(A, B); std::swap(uvA, uvB);}
		for (int j=A.x(); j<=B.x(); j++)
		{
			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
			Vec2i uvP =     uvA +   (uvB-uvA)*phi;
			int idx = P.x() + P.y() * m_image_resolution;
			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
				continue;
			if (m_pZbuffer[idx] < P.z())
			{
				m_pZbuffer[idx] = P.z();
				unsigned char color[3];
				memcpy(color, &m_pTga_image->getTGAimage()[(int)(uvP.x() + uvP.y() * m_pTga_image->getWidth()) * 3], 3);
				for (int i = 0; i < 3; ++i)
					color[i] *= intensivity;
				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
			}
		}
	}
}

void	Application::draw_point_triangle(Vec4f* world_coords)
{
	int x0 = static_cast<int>((world_coords[0].x() + 1.) * m_image_resolution / 2.);
	int y0 = static_cast<int>((world_coords[0].y() + 1.) * m_image_resolution / 2.);
	int x1 = static_cast<int>((world_coords[1].x() + 1.) * m_image_resolution / 2.);
	int y1 = static_cast<int>((world_coords[1].y() + 1.) * m_image_resolution / 2.);
	int x2 = static_cast<int>((world_coords[2].x() + 1.) * m_image_resolution / 2.);
	int y2 = static_cast<int>((world_coords[2].y() + 1.) * m_image_resolution / 2.);
	
	memcpy(m_pImage + static_cast<int>(y0 * m_image_resolution + x0) * m_bytespp, m_white, m_bytespp);
	memcpy(m_pImage + static_cast<int>(y1 * m_image_resolution + x1) * m_bytespp, m_white, m_bytespp);
	memcpy(m_pImage + static_cast<int>(y2 * m_image_resolution + x2) * m_bytespp, m_white, m_bytespp);
}

void	Application::draw_line_triange(Vec4f* world_coords)
{
	int x0 = static_cast<int>((world_coords[0].x() + 1.) * m_image_resolution / 2.);
	int y0 = static_cast<int>((world_coords[0].y() + 1.) * m_image_resolution / 2.);
	int x1 = static_cast<int>((world_coords[1].x() + 1.) * m_image_resolution / 2.);
	int y1 = static_cast<int>((world_coords[1].y() + 1.) * m_image_resolution / 2.);
	int x2 = static_cast<int>((world_coords[2].x() + 1.) * m_image_resolution / 2.);
	int y2 = static_cast<int>((world_coords[2].y() + 1.) * m_image_resolution / 2.);

	line(x0, y0, x1, y1, m_white);
	line(x1, y1, x2, y2, m_white);
	line(x2, y2, x0, y0, m_white);
}

void	Application::draw_line_triange_test(Vec4i* screen_coords)
{
	int x0 = screen_coords[0].x();
	int y0 = screen_coords[0].y();
	int x1 = screen_coords[1].x();
	int y1 = screen_coords[1].y();
	int x2 = screen_coords[2].x();
	int y2 = screen_coords[2].y();

	line(x0, y0, x1, y1, m_white);
	line(x1, y1, x2, y2, m_white);
	line(x2, y2, x0, y0, m_white);
}

void	Application::line(int x0, int y0, int x1, int y1, const unsigned char* color)
{
	if ((x0 < 0 && x1 < 0) || \
		(x0 >= m_image_resolution && x1 >= m_image_resolution) || \
		(y0 < 0 && y1 < 0) || \
		(y0 >= m_image_resolution && y1 >= m_image_resolution))
		return;

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
			continue;
		memcpy(m_pImage + (y0 * m_image_resolution + x0) * m_bytespp, color, m_bytespp);
	}
}

void	Application::on_update()
{
	if (Input::IsKeyEvent())
	{
		if (Input::IsKeyPressed(KeyCode::KEY_A))
		{
			std::cout << "Application KEY_W" << std::endl;

			camera_rotation.y() += 10;
			m_camera.set_rotation(camera_rotation);
		}
		if (Input::IsKeyPressed(KeyCode::KEY_D))
		{
			std::cout << "Application KEY_W" << std::endl;

			camera_rotation.y() -= 10;
			m_camera.set_rotation(camera_rotation);
		}
		if (Input::IsKeyPressed(KeyCode::KEY_J))
		{
			std::cout << "Application KEY_J" << std::endl;
			camera_position.x() += 0.1;
			m_camera.set_position(camera_position);
		}
		if (Input::IsKeyPressed(KeyCode::KEY_L))
		{
			std::cout << "Application KEY_L" << std::endl;
			camera_position.x() -= 0.1;
			m_camera.set_position(camera_position);
		}
		if (Input::IsKeyPressed(KeyCode::KEY_1))
		{
			std::cout << "Application KEY_1" << std::endl;
			m_camera.change_projection();
		}	
		Input::ClearKeyEvent();
		m_redraw = true;
	}
}

}
