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
	m_pShader = new Shader_only_vertex(*m_pModel, m_camera);
	m_pDrawFunction = &Application::draw_model_in_point;

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

		// if (m_redraw)
		{
			memset(m_pImage, 0, m_image_size);
			std::fill_n(m_pZbuffer, m_image_resolution * m_image_resolution, std::numeric_limits<int>::min());

			for (size_t i = 0; i < m_pModel->get_f_v().size(); ++i)
			{
				// std:: cout << i << std::endl;
				for (int j : {0, 1, 2})
				{
					// std:: cout << "\t" << j << "\t";
					m_pShader->vertex(i, j);
				}
				(this->*m_pDrawFunction)();
			}
			m_redraw = false;
		}

		m_pWindow->on_update(m_pImage, m_image_resolution);
		on_update();
		FPS::end();
		FPS::calculate_fps();
		// std::getchar();
	}

	// int k = 0;

	// while (!m_pWindow->getIsClosed())
	// {
	// 	FPS::start();
	// 	// std::cout << k++ << std::endl;

	// 	if (m_image_resolution != m_pWindow->getResolution())
	// 	{
	// 		delete m_pImage;
	// 		delete m_pZbuffer;
	// 		m_image_resolution = m_pWindow->getResolution();
	// 		m_image_size = m_image_resolution * m_image_resolution * m_bytespp;
	// 		m_pImage = new unsigned char[m_image_size];
	// 		m_pZbuffer = new int[m_image_resolution * m_image_resolution];
	// 		m_redraw = true;
	// 	}

	// 	if (m_redraw)
	// 	{
	// 		memset(m_pImage, 0, m_image_size);
	// 		std::fill_n(m_pZbuffer, m_image_resolution * m_image_resolution, std::numeric_limits<int>::min());

	// 		Mat4 view = m_camera.get_view_matrix();

	// 		Mat4 translate_matrix = {
	// 				1, 0, 0, 1,
	// 				0, 1, 0, 1,
	// 				0, 0, 1, 1,
	// 				0, 0, 0, 1};

	// 		Mat4 scale_matrix = {
	// 				m_image_resolution / 2.f, 0, 0, 0,
	// 				0, m_image_resolution / 2.f, 0, 0,
	// 				0, 0, depth / 2.f, 0,
	// 				0, 0, 0, 1};

	// 		Mat4 LookAt = scale_matrix * translate_matrix;

	// 		Vec3f world_coords[3];
	// 		Vec4i screen_coords[3];
	// 		// Vec3f light_dir = {0, 0, -1};
	// 		Vec3i screen_coords_test[3];

	// 		float	intens[3];

	// light_dir.normalize();

	// Mat4 ModelView  = lookat(eye, center, Vec3f({0,1,0}));
	// Mat4 ViewPort   = viewport(m_image_resolution/8, m_image_resolution/8, m_image_resolution*3/4, m_image_resolution*3/4);
	// Mat4 Projection = {
	// 				1, 0, 0, 0,
	// 				0, 1, 0, 0,
	// 				0, 0, 1, 0,
	// 				0, 0, 0, 1};
	// Projection[3][2] = -1.f/(eye-center).norm();
	// // Projection[3][2] = 0;
	// Mat4 z = (ViewPort*Projection*ModelView);
	// z = ViewPort * Projection * view;

	// Vec4f screen_coords_shader[3];
	// Vec3f varying_intensity;
	// 		for (size_t i = 0; i < m_pModel->get_f_v().size(); ++i)
	// 		{
	// 			for (size_t j = 0; j < 3; j++)
	// 				screen_coords_shader[j] = shader_vertex(i, j, m_pModel, varying_intensity, z);

	// 			triangle_shader(screen_coords_shader, varying_intensity);
	// 			// std::cout << i << "\t";
	// 			std::vector<int> face = m_pModel->get_f_v()[i];
	// 			// Vec4f ttt[3];
	// 			std::vector<int> vt = m_pModel->get_f_vn()[i];
	// 			// for (size_t j = 0; j < 3; j++)
	// 			// {
	// 			// 	// std::cout << m_pModel->get_v()[face[j]] << std::endl;
	// 			// 	// world_coords[j] = view * Vec4f(m_pModel->get_v()[face[j]], 1);

	// 			// // world_coords[j] = m_pModel->get_v()[face[j]];
	// 			// 	// screen_coords[j] = LookAt * view * Vec4f(world_coords[j], 1.f);
	// 			// 	// std::cout << world_coords[j] << std::endl;
					
	// 			// 	// ttt[j] = ViewPort * view * Vec4f(world_coords[j], 1.f);
	// 			// // ttt[j] = z * Vec4f(world_coords[j], 1.f);
	// 			// // screen_coords_test[j] = {static_cast<int>(ttt[j].x()/ttt[j].w()), static_cast<int>(ttt[j].y()/ttt[j].w()), static_cast<int>(ttt[j].z()/ttt[j].w())};
				
	// 			// // Vec3f ttt = m_pModel->get_vn()[vt[j]];

	// 			// // intens[j] = (ttt.normalize()) * light_dir;
	// 			// 	// std::cout << screen_coords[j] << std::endl;
	// 			// }
	// 			// // draw_line_triange_test(screen_coords);
	// 			// Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
	// 			// n.normalize();
	// 			// float intensity = n * light_dir;

	// 			// // if (intensity > 0) 
	// 			// {
	// 			// 	Vec2i uv[3];
	// 			// 	std::vector<int> uv_face = m_pModel->get_f_vt()[i];
	// 			// 	for (int k=0; k<3; k++)
	// 			// 	{
	// 			// 		int idx = uv_face[k];
	// 			// 		uv[k].x() = m_pModel->get_vt()[idx].x() * m_pTga_image->getWidth();
	// 			// 		uv[k].y() = m_pModel->get_vt()[idx].y() * m_pTga_image->getHeight();
	// 			// 	}
	// 			// 	// unsigned char col = static_cast<unsigned char>(intensity * 255);
	// 			// 	// unsigned char color[3] =  {col, col, col};
	// 			// 	// draw_fill_triange_test(screen_coords, color);
	// 			// 	// draw_fill_triange_texture(screen_coords, uv, intensity);
	// 			// 	draw_fill_triange_texture_test_inten(screen_coords_test, intens);
	// 			// }

	// 			// unsigned char color[3] =  {rand()%255, rand()%255, rand()%255};
	// 			// draw_fill_triange_test(screen_coords, color);


	// 		}
	// 		m_redraw = false;
	// 	}

	// 	m_pWindow->on_update(m_pImage, m_image_resolution);
	// 	on_update();
	// 	// std::getchar();
	// 	FPS::end();
	// 	FPS::calculate_fps();
	// }
	return 0;
}

// void shader_fragment(Vec3f& bar, unsigned char* color, Vec3f& varying_intensity)
// {
// 	float intensity = varying_intensity * bar;   // interpolate intensity for the current pixel
// 	if (intensity < 0)
// 		intensity = 0;
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		color[i] = 255 * intensity; // well duh
// 	}
// }

// void shader_fragment_first_modif(Vec3f& bar, unsigned char* color, Vec3f& varying_intensity)
// {
// 	float intensity = varying_intensity * bar;   // interpolate intensity for the current pixel
// 	if (intensity>.85) intensity = 1;
// 	else if (intensity>.60) intensity = .80;
// 	else if (intensity>.45) intensity = .60;
// 	else if (intensity>.30) intensity = .45;
// 	else if (intensity>.15) intensity = .30;
// 	else intensity = 0;
// 	{
// 		color[0] = 255 * intensity;
// 		color[1] = 155 * intensity;
// 		color[2] = 0 * intensity;
// 	}
// }

// void	Application::draw_fill_triange_texture_test_inten(Vec3i*  screen_coords, float* intensity)
// {
// 	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
// 	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
// 	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};
// 	float ity0 = intensity[0];
// 	float ity1 = intensity[1];
// 	float ity2 = intensity[2];

// 	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
// 	if (t0.y()>t1.y()) { std::swap(t0, t1); std::swap(ity0, ity1); }
// 	if (t0.y()>t2.y()) { std::swap(t0, t2); std::swap(ity0, ity2); }
// 	if (t1.y()>t2.y()) { std::swap(t1, t2); std::swap(ity1, ity2); }

// 	int total_height = t2.y()-t0.y();
// 	for (int i=0; i<total_height; i++)
// 	{
// 		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
// 		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
// 		float alpha = (float)i/total_height;
// 		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
// 		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
// 		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

// 		float ityA =               ity0 +   (ity2-ity0)*alpha;
//         float ityB = second_half ? ity1 +   (ity2-ity1)*beta : ity0 +   (ity1-ity0)*beta;

// 		if (A.x()>B.x()) { std::swap(A, B); std::swap(ityA, ityB);}
// 		for (int j=A.x(); j<=B.x(); j++)
// 		{
// 			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
// 			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
// 			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
// 				continue;
// 			float ityP =    ityA  + (ityB-ityA)*phi;
// 			int idx = P.x() + P.y() * m_image_resolution;
// 			if (m_pZbuffer[idx] < P.z())
// 			{
// 				m_pZbuffer[idx] = P.z();
// 				unsigned char color[3];
// 				if (ityP < 0)
// 					ityP = 0;
// 				for (int i = 0; i < 3; ++i)
// 					color[i] = 255 * ityP;
// 				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
// 			}
// 		}
// 	}
// }

// void	Application::draw_fill_triange_texture(Vec4i* screen_coords, Vec2i* uv, float intensivity)
// {
// 	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
// 	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
// 	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};
// 	Vec2i uv0 = {uv[0].x(), uv[0].y()};
// 	Vec2i uv1 = {uv[1].x(), uv[1].y()};
// 	Vec2i uv2 = {uv[2].x(), uv[2].y()};

// 	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
// 	if (t0.y()>t1.y()) { std::swap(t0, t1); std::swap(uv0, uv1); }
// 	if (t0.y()>t2.y()) { std::swap(t0, t2); std::swap(uv0, uv2); }
// 	if (t1.y()>t2.y()) { std::swap(t1, t2); std::swap(uv1, uv2); }

// 	int total_height = t2.y()-t0.y();
// 	for (int i=0; i<total_height; i++)
// 	{
// 		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
// 		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
// 		float alpha = (float)i/total_height;
// 		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
// 		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
// 		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

// 		Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
// 		Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;

// 		if (A.x()>B.x()) { std::swap(A, B); std::swap(uvA, uvB);}
// 		for (int j=A.x(); j<=B.x(); j++)
// 		{
// 			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
// 			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
// 			Vec2i uvP =     uvA +   (uvB-uvA)*phi;
// 			int idx = P.x() + P.y() * m_image_resolution;
// 			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
// 				continue;
// 			if (m_pZbuffer[idx] < P.z())
// 			{
// 				m_pZbuffer[idx] = P.z();
// 				unsigned char color[3];
// 				memcpy(color, &m_pTga_image->getTGAimage()[(int)(uvP.x() + uvP.y() * m_pTga_image->getWidth()) * 3], 3);
// 				for (int i = 0; i < 3; ++i)
// 					color[i] *= intensivity;
// 				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
// 			}
// 		}
// 	}
// }

// void	Application::draw_fill_triange_texture_test(Vec3i* screen_coords, Vec2i* uv, float intensivity)
// {
// 	Vec3i t0 = {screen_coords[0].x(), screen_coords[0].y(), screen_coords[0].z()};
// 	Vec3i t1 = {screen_coords[1].x(), screen_coords[1].y(), screen_coords[1].z()};
// 	Vec3i t2 = {screen_coords[2].x(), screen_coords[2].y(), screen_coords[2].z()};
// 	Vec2i uv0 = {uv[0].x(), uv[0].y()};
// 	Vec2i uv1 = {uv[1].x(), uv[1].y()};
// 	Vec2i uv2 = {uv[2].x(), uv[2].y()};

// 	if (t0.y()==t1.y() && t0.y()==t2.y()) return; // i dont care about degenerate triangles
// 	if (t0.y()>t1.y()) { std::swap(t0, t1); std::swap(uv0, uv1); }
// 	if (t0.y()>t2.y()) { std::swap(t0, t2); std::swap(uv0, uv2); }
// 	if (t1.y()>t2.y()) { std::swap(t1, t2); std::swap(uv1, uv2); }

// 	int total_height = t2.y()-t0.y();
// 	for (int i=0; i<total_height; i++)
// 	{
// 		bool second_half = i>t1.y()-t0.y() || t1.y()==t0.y();
// 		int segment_height = second_half ? t2.y()-t1.y() : t1.y()-t0.y();
// 		float alpha = (float)i/total_height;
// 		float beta  = (float)(i-(second_half ? t1.y()-t0.y() : 0))/segment_height; // be careful: with above conditions no division by zero here
// 		Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
// 		Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

// 		Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
// 		Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;

// 		if (A.x()>B.x()) { std::swap(A, B); std::swap(uvA, uvB);}
// 		for (int j=A.x(); j<=B.x(); j++)
// 		{
// 			float phi = B.x() == A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
// 			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
// 			Vec2i uvP =     uvA +   (uvB-uvA)*phi;
// 			int idx = P.x() + P.y() * m_image_resolution;
// 			if (P.x() < 0 || P.y() < 0 || P.x() >= m_image_resolution || P.y() >= m_image_resolution)
// 				continue;
// 			if (m_pZbuffer[idx] < P.z())
// 			{
// 				m_pZbuffer[idx] = P.z();
// 				unsigned char color[3];
// 				memcpy(color, &m_pTga_image->getTGAimage()[(int)(uvP.x() + uvP.y() * m_pTga_image->getWidth()) * 3], 3);
// 				for (int i = 0; i < 3; ++i)
// 					color[i] *= intensivity;
// 				memcpy(m_pImage + (P.y() * m_image_resolution + P.x()) * 3, color, 3);
// 			}
// 		}
// 	}
// }

void	Application::draw_model_in_point()
{
	for (int i = 0; i < 3; ++i)
	{
		point(m_pShader->view_tri[i].x(), m_pShader->view_tri[i].y(), m_pShader->color);
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
	int x0 = m_pShader->view_tri[0].x();
	int y0 = m_pShader->view_tri[0].y();
	int x1 = m_pShader->view_tri[1].x();
	int y1 = m_pShader->view_tri[1].y();
	int x2 = m_pShader->view_tri[2].x();
	int y2 = m_pShader->view_tri[2].y();

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

Vec3f barycentric(const Vec2f A, const Vec2f B, const Vec2f C, const Vec2f P)
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

// void	Application::draw_model_in_simple_triangle_rand_color()
// {
// 	Vec4f* tri = m_pShader->view_tri;
// 	Vec4f pts[3]  = { tri[0], tri[1], tri[2]};  // triangle screen coordinates before persp. division	
// 	Vec2f pts2[3] = { {(pts[0]/pts[0][3]).x(), (pts[0]/pts[0][3]).y()}, \
// 						{(pts[1]/pts[1][3]).x(), (pts[1]/pts[1][3]).y()}, \
// 						{(pts[2]/pts[2][3]).x(), (pts[2]/pts[2][3]).y()}};

// 	int bboxmin[2] = {static_cast<int>(m_image_resolution - 1), static_cast<int>(m_image_resolution - 1)};
// 	int bboxmax[2] = {0, 0};
// 	for (int i=0; i<3; i++)
// 	{
// 		for (int j=0; j<2; j++) {
// 			bboxmin[j] = std::min(bboxmin[j], static_cast<int>(pts2[i][j]));
// 			bboxmax[j] = std::max(bboxmax[j], static_cast<int>(pts2[i][j]));
// 		}
// 	}
// 	m_pShader->fragment();
// 	for (int x = std::max(bboxmin[0], 0); x <= std::min(bboxmax[0], static_cast<int>(m_image_resolution-1)); x++)
// 	{
// 		for (int y = std::max(bboxmin[1], 0); y <= std::min(bboxmax[1], static_cast<int>(m_image_resolution-1)); y++)
// 		{
// 			Vec3f bc_screen = barycentric(pts2[0], pts2[1], pts2[2], {static_cast<float>(x), static_cast<float>(y)});
// 			Vec3f bc_clip   = {bc_screen.x() / pts[0][3], bc_screen.y() / pts[1][3], bc_screen.z() / pts[2][3]};
// 			bc_clip = bc_clip / (bc_clip.x() + bc_clip.y() + bc_clip.z());
// 			float frag_depth = Vec3f{pts[0][2], pts[1][2], pts[2][2]} * bc_clip;
// 			if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0 || frag_depth < m_pZbuffer[x + y * m_image_resolution])
// 				continue;
// 			m_pZbuffer[x + y * m_image_resolution] = frag_depth;
// 			memcpy(m_pImage + static_cast<int>(y * m_image_resolution + x) * m_bytespp, m_pShader->color, m_bytespp);
// 		}
// 	}
// }

void	Application::draw_model_in_simple_triangle_rand_color()
{
	Vec4f* tri = m_pShader->view_tri;
	Vec4f pts[3] = { tri[0], tri[1], tri[2] };  // triangle screen coordinates before persp. division
	Vec2f pts2[3] = { {(pts[0] / pts[0][3]).x(), (pts[0] / pts[0][3]).y()},
					{(pts[1] / pts[1][3]).x(), (pts[1] / pts[1][3]).y()},
					{(pts[2] / pts[2][3]).x(), (pts[2] / pts[2][3]).y()} };

	// Precompute barycentric coordinates and depth values for each vertex
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
			if (frag_depth < m_pZbuffer[x + y * m_image_resolution])
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

}
