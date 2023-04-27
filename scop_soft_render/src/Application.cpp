#include "Application.hpp"
#include "Window.hpp"
#include "FPS.hpp"


#include <cstring>
#include <iostream>

#include "Vector.hpp"
#include <GLFW/glfw3.h>

const unsigned char white[3] = { 255, 255, 255 };
const unsigned char red[3]   = { 255, 0, 0 };
const unsigned char green[3] = { 0, 255, 0 };
const unsigned char blue[3] = { 0, 0, 255 };

void line(Vec2i t0, Vec2i t1, unsigned char* image, const unsigned char *color, unsigned int image_resolution)
{
	int dx = abs(t1.x - t0.x);
	int dy = abs(t1.y - t0.y);
	int sx = t0.x < t1.x ? 1 : -1;
	int sy = t0.y < t1.y ? 1 : -1;
	int err = dx - dy;
	while (t0.x != t1.x || t0.y != t1.y)
	{
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			t0.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			t0.y += sy;
		}
		// установка пикселя (x0, y0) в изображении
		if (t0.x < 0 || t0.y < 0 || t0.x >= image_resolution || t0.y >= image_resolution)
			return;
		memcpy(image + (t0.y * image_resolution + t0.x) * 3, color, 3);
	}
}

void testDraw(Vec2i t0, Vec2i t1, Vec2i t2, unsigned char* image, const unsigned char *color, unsigned int image_resolution)
{
	// if (t0.y == t1.y)
	// 	return;

	// int total_height = t2.y - t0.y;
	// int segment_height = t2.y - t0.y;
	// for (int i = 0; i < total_height; i++)
	// {
	// 	float alpha = (float)i / total_height;
	// 	float beta  = (float)(i - delta) / segment_height; // be careful: with above conditions no division by zero here
	// 	Vec2i A =               t0 + (delta + t1 - t0) * alpha;
	// 	Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
	// 	if (A.x>B.x) std::swap(A, B);
	// 	for (int j=A.x; j<=B.x; j++)
	// 	{
	// 		memcpy(image + ((t0.y + i) * image_resolution + j) * 3, color, 3);
	// 	}
	// }

	// for (int y = t0.y; y <= t1.y; y++)
	// {
	// 	int segment_height = t1.y - t0.y + 1;
	// 	float alpha = (float)(y - t0.y) / total_height;
	// 	float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero
	// 	Vec2i A = t0 + (t2 - t0) * alpha;
	// 	Vec2i B = t0 + (t1 - t0) * beta;
	// 	if (A.x > B.x)  
	// 	{
	// 		std::swap(A, B);
	// 	}
	// 	for (int j = A.x; j <= B.x; j++)
	// 	{
	// 		memcpy(image + (y * image_resolution + j) * 3, color, 3);
	// 	}
	// }
}

void drawTriangle(Vec3i t0, Vec3i t1, Vec3i t2, unsigned char* image, int* zbuffer, const unsigned char *color, unsigned int image_resolution)
{
	if (t0.y==t1.y && t0.y==t2.y)
		return;
	// sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int		total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++)
	{
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3i A =               t0 + Vec3f(t2-t0)*alpha;
        Vec3i B = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;
		if (A.x > B.x)
		{
			std::swap(A, B);
		}
		for (int j = A.x; j <= B.x; j++)
		{
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);

// template <> template <> Vec3<int>::Vec3<>(const Vec3<float> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {
// }

// template <> template <> Vec3<float>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {
// }

			Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
			int idx = P.x + P.y * image_resolution;
			if (zbuffer[idx]<P.z) {
				zbuffer[idx] = P.z;
				memcpy(image + ((P.y) * image_resolution + P.x) * 3, color, 3);
			}
			
		}
	}

	// int total_height = t2.y - t0.y;
	// int segment_height = t1.y - t0.y + 1;
	// for (int y = t0.y; y <= t1.y; y++)
	// {
	// 	float alpha = (float)(y - t0.y) / total_height;
	// 	float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero
	// 	Vec2i A = t0 + (t2 - t0) * alpha;
	// 	Vec2i B = t0 + (t1 - t0) * beta;
	// 	if (A.x > B.x)  
	// 	{
	// 		std::swap(A, B);
	// 	}
	// 	for (int j = A.x; j <= B.x; j++)
	// 	{
	// 		memcpy(image + (y * image_resolution + j) * 3, color, 3);
	// 	}
	// }
	// segment_height =  t2.y - t1.y + 1;
	// for (int y = t1.y; y <= t2.y; y++)
	// {
	// 	float alpha = (float)(y - t0.y) / total_height;
	// 	float beta  = (float)(y - t1.y) / segment_height; // be careful with divisions by zero
	// 	Vec2i A = t0 + (t2 - t0) * alpha;
	// 	Vec2i B = t1 + (t2 - t1) * beta;
	// 	if (A.x > B.x)
	// 	{
	// 		std::swap(A, B);
	// 	}
	// 	for (int j = A.x; j <= B.x; j++)
	// 	{
	// 		memcpy(image + (y * image_resolution + j) * 3, color, 3);
	// 	}
	// }
}

void triangleLine(Vec2i t0, Vec2i t1, Vec2i t2, unsigned char* image, const unsigned char *color, unsigned int image_resolution)
{
	line(t0, t1, image, color, image_resolution);
	line(t1, t2, image, color, image_resolution);
	line(t2, t0, image, color, image_resolution);
}

namespace Scop
{

Vec3f light_dir(0,0,-1);

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
const int NUM_FRAMES_TO_AVERAGE = 100;
int frame_count = 0;
double total_frame_time = 0.0;
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
			zbuffer = new int[image_resolution * image_resolution];
		}
		memset(image, 0, image_size);
		memset(zbuffer, 0, image_resolution * image_resolution * sizeof(int));
// line render
		// for (int i = 0; i < model->faces_.size(); i++) 
		// {
		// 	std::vector<int> face = model->faces_[i];
		// 	for (int j = 0; j < 3; j++)
		// 	{
		// 		Vec3f v0 = model->verts_[face[j]];
		// 		Vec3f v1 = model->verts_[face[(j + 1) % 3]];
		// 		int x0 = (v0.x + 1.) * image_resolution / 2.;
		// 		int y0 = (v0.y + 1.) * image_resolution / 2.;
		// 		int x1 = (v1.x + 1.) * image_resolution / 2.;
		// 		int y1 = (v1.y + 1.) * image_resolution / 2.;
		// 		line(Vec2i(x0, y0), Vec2i(x1, y1), image, white, image_resolution);
		// 	}
		// }

//rand color render
		// for (int i = 0; i < model->faces_.size(); i++) 
		// {
		// 	std::vector<int> face = model->faces_[i];
		// 	Vec2i screen_coords[3];

		// 	for (int j = 0; j < 3; j++)
		// 	{
		// 		Vec3f world_coords = model->verts_[face[j]];
		// 		screen_coords[j] = Vec2i((world_coords.x+1.) * image_resolution/2., (world_coords.y+1.) * image_resolution/2.);
		// 	}
		// 	unsigned char color[3];
		// 	color[0] = rand() % 256;
		// 	color[1] = rand() % 256;
		// 	color[2] = rand() % 256;
		// 	drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, color, image_resolution);
		// }

//light
		// std::cout << frame_count << std::endl;
		for (int i = 0; i < model->getFaces().size(); i++) 
		{
			// std::cout << "\t\t" << i << std::endl;
			std::vector<int> face = model->getFaces()[i];
			Vec3i screen_coords[3];
			Vec3f world_coords[3];
			// std::cout << "0000" << std::endl;
			for (int j = 0; j < 3; j++)
			{
				// std::cout << "!!0000" << "\t" << j << std::endl;
				// std::cout << model->verts_[0].x << std::endl;
				// std::cout << model->verts_[0].y << std::endl;
				// std::cout << model->verts_[0].z << std::endl;
				// std::cout << face[j] << std::endl;

				Vec3f v = model->getVerts()[face[j]];
				// std::cout << "!!1111" << "\t" << j << std::endl;
				screen_coords[j] = Vec3i((v.x+1.)*image_resolution/2., (v.y+1.)*image_resolution/2., (v.z+1.)*depth/2.);
				// std::cout << "!!2222" << "\t" << j << std::endl;
				world_coords[j]  = v;
			}
			// std::cout << "1111" << std::endl;
			Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
			n.normalize();
			float intensity = n * light_dir;

			if (intensity > 0)
			{
				unsigned char int_color = intensity * 255;
				unsigned char color[3] = { int_color, int_color, int_color };;
				drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, zbuffer, color, image_resolution);
			}
		}
		// std::cout << "\t" << frame_count << std::endl;
        // TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
		// unsigned char *test = new unsigned char[image_size];
		// memset(test, 0, image_size);
        // for (int i=0; i<image_resolution; i++) {
        //     for (int j=0; j<image_resolution; j++) 
		// 	{
		// 		unsigned char color[3] = { zbuffer[i+j*image_resolution], zbuffer[i+j*image_resolution], zbuffer[i+j*image_resolution] };;
		// 		memcpy(image + (j * image_resolution + i) * 3, color, 3);
        //     }
        // }

//triangle test
		// Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
		// Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
		// Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

		// drawTriangle(t0[0], t0[1], t0[2], image, red, image_resolution);
		// drawTriangle(t1[0], t1[1], t1[2], image, green, image_resolution);
		// drawTriangle(t2[0], t2[1], t2[2], image, blue, image_resolution);

		// triangleLine(t0[0], t0[1], t0[2], image, red, image_resolution);
		// triangleLine(t1[0], t1[1], t1[2], image, green, image_resolution);
		// triangleLine(t2[0], t2[1], t2[2], image, blue, image_resolution);



		m_pWindow->on_update(image, image_resolution);
		// m_pWindow->on_update_test(tga_image->getTGAimage(), tga_image->getWidth(), tga_image->getHeight());

		FPS::end();
		FPS::calculate_fps();
std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::high_resolution_clock::now();

//FPS//
	}
	return 0;
}

}
