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
		unsigned char	color[3];

		Vec2f			uv[3];
		Vec4f			mv[3];
		Vec4f			mvp[3];
		Vec4f			mvpv[3];
		Vec3f			varying_intensity;

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
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
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
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			color[0] = rand() % 255;
			color[1] = rand() % 255;
			color[2] = rand() % 255;
		}
	};

	struct Shader_texture: public IShader
	{
		Shader_texture(const Model& model, const TGAimage &texture, Camera &camera)
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
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			Vec2i uv_tmp;
			uv_tmp.x() = (Vec3f({uv[0].x(), uv[1].x(), uv[2].x()}) * bar) * texture.getWidth();
			uv_tmp.y() = (Vec3f({uv[0].y(), uv[1].y(), uv[2].y()}) * bar) * texture.getHeight();
			int idx = (uv_tmp.x() + uv_tmp.y() * texture.getWidth()) * texture.getBytespp();
			memcpy(color, texture.getTGAimage() + idx, texture.getBytespp());
		}
	};

	struct Shader_intensity: public IShader
	{
		Vec3f light_dir = Vec3f({1, 1, 1}).normalize();
		Shader_intensity(const Model& model, const TGAimage &texture, Camera &camera)
			: IShader(model, texture, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> f_vn = model.get_f_vn()[i_face];
			Vec3f tmp = model.get_vn()[f_vn[i_vert]];
			varying_intensity[i_vert] = std::min(1.f, std::max(0.f, model.get_vn()[f_vn[i_vert]] * light_dir));
			std::vector<int> face = model.get_f_v()[i_face];
			mv[i_vert] = camera.get_view_matrix() *  camera.get_model_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvp[i_vert] = camera.get_mvp_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			float intensity = varying_intensity * bar;
			if (intensity>.85) intensity = 1;
			else if (intensity>.60) intensity = .80;
			else if (intensity>.45) intensity = .60;
			else if (intensity>.30) intensity = .45;
			else if (intensity>.15) intensity = .30;
			else intensity = 0;
			// if (intensity < 0) intensity = 0;
			for (int i = 0; i < 3; i++)
				color[i] = 255 * intensity;
		}
	};

	struct Shader_texture_intensity: public IShader
	{
		Vec3f light_dir = Vec3f({1, 1, 1}).normalize();
		Shader_texture_intensity(const Model& model, const TGAimage &texture, Camera &camera)
			: IShader(model, texture, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> f_vt = model.get_f_vt()[i_face];
			uv[i_vert] = model.get_vt()[f_vt[i_vert]];
			std::vector<int> f_vn = model.get_f_vn()[i_face];
			varying_intensity[i_vert] = std::max(0.f, model.get_vn()[f_vn[i_vert]] * light_dir);
			std::vector<int> face = model.get_f_v()[i_face];
			mv[i_vert] = camera.get_view_matrix() *  camera.get_model_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvp[i_vert] = camera.get_mvp_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
			mvpv[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment(Vec3f bar = {0,0,0})
		{
			Vec2i uv_tmp;
			uv_tmp.x() = (Vec3f({uv[0].x(), uv[1].x(), uv[2].x()}) * bar) * texture.getWidth();
			uv_tmp.y() = (Vec3f({uv[0].y(), uv[1].y(), uv[2].y()}) * bar) * texture.getHeight();
			int idx = (uv_tmp.x() + uv_tmp.y() * texture.getWidth()) * texture.getBytespp();
			memcpy(color, texture.getTGAimage() + idx, texture.getBytespp());
			float intensity = varying_intensity * bar;
			if (intensity < 0) intensity = 0;
			if (intensity > 1) intensity = 1;
			for (int i = 0; i < 3; i++)
				color[i] *= intensity;
		}
	};
}
