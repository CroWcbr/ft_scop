#include "Camera.hpp"
#include "Matrix.hpp"

#include <cmath>

namespace Scop
{

	Camera::Camera(const Vec3f& position, \
		const Vec3f& rotation, \
		const ProjectionMode projection_mode)
		: m_position(position)
		, m_rotation(rotation)
		, m_projection_mode(projection_mode)
	{
		update_view_matrix();
		update_projection_matrix();
	}

	void Camera::update_view_matrix()
	{
		const float roll_in_radians = m_rotation.x() * (M_PI / 180.0);
		const float pitch_in_radians = m_rotation.y() * (M_PI / 180.0);
		const float yaw_in_radians = m_rotation.z() * (M_PI / 180.0);

		Mat4 translate_matrix2 = {
				1, 0, 0, m_position[0],
				0, 1, 0, m_position[1],
				0, 0, 1, m_position[2],
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
				1, 0, 0, -m_position[0],
				0, 1, 0, -m_position[1],
				0, 0, 1, -m_position[2],
				0, 0, 0, 1};

		Mat4 euler_rotate_matrix =  translate_matrix * rotate_matrix_z * rotate_matrix_y  * rotate_matrix_x * translate_matrix2;

		m_view_matrix = euler_rotate_matrix * translate_matrix;
		m_update_view_matrix = false;
	}

	void Camera::update_projection_matrix()
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

		if (m_projection_mode == ProjectionMode::Perspective)
			m_projection_matrix = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, -1.f / 3.f, 1};
		else
		{
			m_projection_matrix = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};
		}
	}

	void	Camera::set_rotation(const Vec3f& rotation)
	{
		m_rotation = rotation;
		m_update_view_matrix = true;
	}
	
	void	Camera::set_position(const Vec3f& position)
	{
		m_position = position;
		m_update_view_matrix = true;
	}

	const Mat4 Camera::get_view_matrix()
	{
		if (m_update_view_matrix)
		{
			update_view_matrix();
		}
		return  m_projection_matrix * m_view_matrix;
	}

	void	Camera::change_projection()
	{
		m_projection_mode = m_projection_mode == ProjectionMode::Perspective ? ProjectionMode::Orthographic : ProjectionMode::Perspective;
		update_projection_matrix();
	}

}