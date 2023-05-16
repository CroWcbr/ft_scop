#include "Camera.hpp"
#include "Matrix.hpp"

#include <cmath>

namespace Scop
{

	Camera::Camera(const Vec3f& model_position, \
			const Vec3f& model_rotation,
			const float model_scale,
			const Vec3f& view_position, \
			const Vec3f& view_rotation,
			const ProjectionMode projection_mode)
		: m_model_position(model_position)
		, m_model_rotation(model_rotation)
		, m_model_scale(model_scale)
		, m_view_position(view_position)
		, m_view_rotation(view_rotation)
		, m_projection_mode(projection_mode)
	{
		update_model_matrix();
		update_view_matrix();
		update_projection_matrix();
		update_viewport_matrix();
	}

	const Mat4&		Camera::get_mvpv_matrix()
	{
		if (m_update_mvpv_matrix)
		{
			update_mvpv_matrix();
		}
		return m_mvpv_matrix;
	}

	void			Camera::update_mvpv_matrix()
	{
		if (m_update_model_matrix)
		{
			update_model_matrix();
		}
		if (m_update_view_matrix)
		{
			update_view_matrix();
		}
		m_update_mvpv_matrix = false;
		m_mvpv_matrix = get_viewport_matrix() * get_model_matrix();
	}

	const Mat4&		Camera::get_model_matrix() const
	{
		return m_model_matrix;
	}

	void			Camera::set_model_rotation(const Vec3f& rotation)
	{
		m_model_rotation = rotation;
		m_update_model_matrix = true;
		m_update_mvpv_matrix = true;
	}
	
	void			Camera::set_model_position(const Vec3f& position)
	{
		m_model_position = position;
		m_update_model_matrix = true;
		m_update_mvpv_matrix = true;
	}

	void			Camera::set_model_scale(const float scale)
	{
		m_model_scale = scale;
		m_update_model_matrix = true;
		m_update_mvpv_matrix = true;
	}

	void			Camera::update_model_matrix()
	{
		const float roll_in_radians = m_model_rotation.x() * (M_PI / 180.0);
		const float pitch_in_radians = m_model_rotation.y() * (M_PI / 180.0);
		const float yaw_in_radians = m_model_rotation.z() * (M_PI / 180.0);

		Mat4 translate_matrix2 = {
				1, 0, 0, m_model_position[0],
				0, 1, 0, m_model_position[1],
				0, 0, 1, m_model_position[2],
				0, 0, 0, 1};

		const Mat4 rotate_matrix_x({
			1, 0, 0, 0,
			0, std::cos(roll_in_radians), -std::sin(roll_in_radians), 0,
			0, std::sin(roll_in_radians), std::cos(roll_in_radians), 0,
			0, 0, 0, 1
			});

		const Mat4 rotate_matrix_y({
			std::cos(pitch_in_radians), 0, std::sin(pitch_in_radians), 0,
			0, 1, 0, 0,
			-std::sin(pitch_in_radians), 0, std::cos(pitch_in_radians), 0,
			0, 0, 0, 1
			});

		Mat4 rotate_matrix_z({
			std::cos(yaw_in_radians), -std::sin(yaw_in_radians), 0, 0,
			std::sin(yaw_in_radians), std::cos(yaw_in_radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			});

		Mat4 translate_matrix = {
				1, 0, 0, -m_model_position[0],
				0, 1, 0, -m_model_position[1],
				0, 0, 1, -m_model_position[2],
				0, 0, 0, 1};

		Mat4 scale_matrix = {
				m_model_scale, 0, 0, 0,
				0, m_model_scale, 0, 0,
				0, 0, m_model_scale, 0,
				0, 0, 0, 1
		};

		Mat4 euler_rotate_matrix =  translate_matrix * rotate_matrix_z * rotate_matrix_y  * rotate_matrix_x * scale_matrix * translate_matrix2;

		m_model_matrix = euler_rotate_matrix * translate_matrix;
		m_update_model_matrix = false;
	}

	const Mat4&		Camera::get_view_matrix() const
	{
		return  m_view_matrix;
	}

	void			Camera::update_view_matrix()
	{
		Mat4 m_view_matrix = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};
		m_update_view_matrix = false;
	}

	void			Camera::set_view_rotation(const Vec3f& rotation)
	{

	}

	void			Camera::set_view_position(const Vec3f& position)
	{

	}

	const Mat4&		Camera::get_projection_matrix() const
	{
		return  m_projection_matrix;
	}

	void			Camera::change_projection_matrix()
	{
		m_projection_mode = m_projection_mode == ProjectionMode::Perspective ? ProjectionMode::Orthographic : ProjectionMode::Perspective;
		update_projection_matrix();
		m_update_mvpv_matrix = true;
	}

	void			Camera::update_projection_matrix()
	{
		// if (m_projection_mode == ProjectionMode::Perspective)
		// {
		// 	float r = 0.1f;
		// 	float t = 0.1f;
		// 	float f = 10.f;
		// 	float n = 0.1f;
		// 	m_projection_matrix = {
		// 		n / r, 0, 0, 0,
		// 		0, n / t, 0, 0,
		// 		0, 0, (-f - n) / (f - n), -2 * f * n / (f - n),
		// 		0, 0, -1, 0};
		// }
		// else
		// {
		// 	float r = 2.f;
		// 	float t = 2.f;
		// 	float f = 100.f;
		// 	float n = 0.1f;
		// 	m_projection_matrix = {
		// 		1 / r, 0, 0, 0,
		// 		0, 1 / t, 0, 0,
		// 		0, 0, -2 / (f - n), (-f - n) / (f - n),
		// 		0, 0, 0, 1};
		// }

		// if (m_projection_mode == ProjectionMode::Perspective)
		// 	m_projection_matrix = {
		// 		1, 0, 0, 0,
		// 		0, -1, 0, 0,
		// 		0, 0, 1, 0,
		// 		0, 0, -1.f / 0.f, 1};
		// else
		// {
			m_projection_matrix = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};
		// }
	}

	const Mat4&		Camera::get_viewport_matrix() const
	{
		return  m_viewport_matrix;
	}

	void			Camera::change_viewport_matrix(int size)
	{
		update_viewport_matrix(size);
		m_update_mvpv_matrix = true;
	}

	void			Camera::update_viewport_matrix(int size)
	{
		Mat4 translate_matrix_model = {
				1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1};

		Mat4 scale_matrix_model = {
				size / 2.f, 0, 0, 0,
				0, size / 2.f, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};

		m_viewport_matrix = scale_matrix_model * translate_matrix_model;
	}

}
