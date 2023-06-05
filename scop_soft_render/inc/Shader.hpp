#pragma once

#include "Model.hpp"
#include "TGAimage.hpp"
#include "Matrix.hpp"
#include "Camera.hpp"

namespace Scop
{

	struct IShader
	{
		const Model		&model;
		const TGAimage	&texture;
		Camera			&camera;
		Vec4f			view_tri[3];
		unsigned char	color[3];

		Vec2f			uv[3];
		Vec4f			mv[3];
		Vec4f			mvp[3];
		Vec4f			mvpv[3];
		Vec4f			gl_Position[3];

		IShader(const Model &m, const TGAimage &t, Camera &c)
			: model(m)
			, texture(t)
			, camera(c)
		{
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
		};

		virtual ~IShader() {};
		virtual void vertex(int iface, int nthvert) = 0;
		virtual void fragment(Vec3f bar = {0,0,0}) = 0;
	};

	struct Shader_only_vertex: public IShader
	{
		Shader_only_vertex(const Model& model, const TGAimage &texture, Camera &camera)
			: IShader(model, texture, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> face = model.get_f_v()[i_face];
			view_tri[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{}
	};

	struct Shader_rand_color: public IShader
	{
		Shader_rand_color(const Model& model, const TGAimage &texture, Camera &camera)
			: IShader(model, texture, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> face = model.get_f_v()[i_face];
			mv[i_vert] = camera.get_view_matrix() *  camera.get_model_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvp[i_vert] = camera.get_mvp_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			view_tri[i_vert] = mv[i_vert];
			gl_Position[i_vert] = mvp[i_vert];
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			color[0] = rand() % 255;
			color[1] = rand() % 255;
			color[2] = rand() % 255;
		}
	};

	struct Shader_test: public IShader
	{





		Shader_test(const Model& model, const TGAimage &texture, Camera &camera)
			: IShader(model, texture, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> f_vt = model.get_f_vt()[i_face];
			uv[i_vert] = model.get_vt()[f_vt[i_vert]];

			std::vector<int> face = model.get_f_v()[i_face];
			mv[i_vert] = camera.get_view_matrix() *  camera.get_model_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvp[i_vert] = camera.get_mvp_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			view_tri[i_vert] = mv[i_vert];
			gl_Position[i_vert] = mvp[i_vert];
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			Vec2f uv_tmp;
		// std::cout << "uv0\t" << uv[0] << std::endl;
		// std::cout << "uv1\t" << uv[1] << std::endl;
		// std::cout << "uv2\t" << uv[2] << std::endl;
		// std::cout << "bar\t" << bar << std::endl;
		
			uv_tmp.x() = (Vec3f({uv[0].x(), uv[1].x(), uv[2].x()}) * bar);
			uv_tmp.y() = (Vec3f({uv[0].y(), uv[1].y(), uv[2].y()}) * bar);
		// std::cout << bar << "\t" << uv[0] << uv[1] << uv[2] << "\t" << uv_tmp << std::endl;
			// std::cout << uv_tmp.x() << "\t" << uv_tmp.y() << std::endl;
			int idx = ((int)(uv_tmp.x() * texture.getWidth()) + (int)(uv_tmp.y() * texture.getHeight()) * texture.getWidth()) * 3;
		// std::cout << "uv_tmp\t" << uv_tmp << "\t" << idx << std::endl;
			memcpy(color, texture.getTGAimage() + idx, 3);
			// color[0] = m_pTga_image->getTGAimage()[idx];
			// color[1] = m_pTga_image->getTGAimage()[idx + 1];
			// color[2] = m_pTga_image->getTGAimage()[idx + 2];
			// color = model->diffuse(uv)*intensity;

			// color[0] = rand() % 255;
			// color[1] = rand() % 255;
			// color[2] = rand() % 255;
		}
	};


}

