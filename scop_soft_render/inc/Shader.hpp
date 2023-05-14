#pragma once

#include "Model.hpp"
#include "Matrix.hpp"

namespace Scop
{

	struct IShader
	{
		const Model		&model;
		Vec4f			view_tri[3];
		unsigned char	color[3];

		IShader(const Model &m)
			: model(m)
		{};

		virtual ~IShader() {};
		virtual void vertex(int iface, int nthvert) = 0;
		virtual void fragment() = 0;
	};

	struct Shader_only_vertex: public IShader
	{
//delete from here
	Mat4 translate_matrix_shader = {
			2, 0, 0, 1,
			0, 2, 0, 1,
			0, 0, 2, 1,
			0, 0, 0, 1};

	Mat4 scale_matrix_shader = {
			800 / 2.f, 0, 0, 0,
			0, 800 / 2.f, 0, 0,
			0, 0, 255 / 2.f, 0,
			0, 0, 0, 1};

	Mat4 LookAt_shader = scale_matrix_shader * translate_matrix_shader;
/////////////////////////////


		Shader_only_vertex(const Model& model)
			: IShader(model)
		{
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
		}

		virtual void vertex(int i_face, int i_vert)
		{
			std::vector<int> face = model.get_f_v()[i_face];
			view_tri[i_vert] = LookAt_shader * Vec4f(model.get_v()[face[i_vert]], 1.f);
		}

		virtual void fragment()
		{
		}
	};

}

