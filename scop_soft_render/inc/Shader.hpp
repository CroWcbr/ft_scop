#pragma once

#include "Model.hpp"
#include "Matrix.hpp"
#include "Camera.hpp"

namespace Scop
{

	struct IShader
	{
		const Model		&model;
		Camera			&camera;
		Vec4f			view_tri[3];
		unsigned char	color[3];

		IShader(const Model &m, Camera &c)
			: model(m)
			, camera(c)
		{};

		virtual ~IShader() {};
		virtual void vertex(int iface, int nthvert) = 0;
		virtual void fragment() = 0;
	};

	struct Shader_only_vertex: public IShader
	{
		Shader_only_vertex(const Model& model, Camera &camera)
			: IShader(model, camera)
		{
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
		}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> face = model.get_f_v()[i_face];
			view_tri[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment()
		{}
	};

	struct Shader_rand_color: public IShader
	{
		Shader_rand_color(const Model& model, Camera &camera)
			: IShader(model, camera)
		{}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> face = model.get_f_v()[i_face];
			view_tri[i_vert] = camera.get_mvpv_matrix() * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment()
		{
			color[0] = rand() % 255;
			color[1] = rand() % 255;
			color[2] = rand() % 255;
		}
	};

}

