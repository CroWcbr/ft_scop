#include "Camera.hpp"

#include <cmath>

namespace Scop
{
	Mat4			update_euler_rotatate_matrix(const Vec3f& rotatate)
	{
		const float roll_in_radians = rotatate.x() * (M_PI / 180.0);
		const float pitch_in_radians = rotatate.y() * (M_PI / 180.0);
		const float yaw_in_radians = rotatate.z() * (M_PI / 180.0);

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

		return rotate_matrix_z * rotate_matrix_y  * rotate_matrix_x;
	}

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
		m_view_position = {0,0,5};
		update_view_matrix();
		update_projection_matrix();
		update_viewport_matrix();
	}

	const Mat4&		Camera::get_mvp_matrix()
	{
		if (m_update_mvpv_matrix)
		{
			update_mvpv_matrix();
		}
		return m_mvp_matrix;
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
		m_mvp_matrix = get_projection_matrix() * get_view_matrix() * get_model_matrix();
		m_mvpv_matrix = get_viewport_matrix() * m_mvp_matrix;
		m_update_mvpv_matrix = false;
	}

	const Mat4&		Camera::get_model_matrix() const
	{
		return m_model_matrix;
	}

	void			Camera::set_model_rotation(const Vec3f& rotation)
	{
		m_model_rotation += rotation;
		m_update_model_matrix = true;
		m_update_mvpv_matrix = true;
	}
	
	void			Camera::set_model_position(const Vec3f& position)
	{
		m_model_position += position;
		m_update_model_matrix = true;
		m_update_mvpv_matrix = true;
	}

	void			Camera::set_model_rotation_and_position(const Vec3f& rotation, const Vec3f& position)
	{
		m_model_rotation += rotation;
		m_model_position += position;
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
		Mat4 scale_matrix = {
				m_model_scale, 0, 0, 0,
				0, m_model_scale, 0, 0,
				0, 0, m_model_scale, 0,
				0, 0, 0, 1
		};

		Mat4 translate_matrix = {
				1, 0, 0, m_model_position[0],
				0, 1, 0, m_model_position[1],
				0, 0, 1, m_model_position[2],
				0, 0, 0, 1};

		Mat4 rotate_matrix = update_euler_rotatate_matrix(m_model_rotation);

		m_model_matrix =  translate_matrix * scale_matrix * rotate_matrix ;
		m_update_model_matrix = false;
	}

	const Mat4&		Camera::get_view_matrix() const
	{
		return  m_view_matrix;
	}

	void			Camera::update_view_matrix()
	{
		m_view_matrix = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};

		Mat4 translate_matrix = {
				1, 0, 0, -m_view_position[0],
				0, 1, 0, -m_view_position[1],
				0, 0, 1, -m_view_position[2],
				0, 0, 0, 1};
		
		Mat4 rotate_matrix = update_euler_rotatate_matrix(-m_view_rotation);

		m_view_matrix = translate_matrix * rotate_matrix;
		m_update_view_matrix = false;
	}

	void			Camera::set_view_rotation(const Vec3f& rotation)
	{
		m_view_rotation += rotation;
		m_update_view_matrix = true;
		m_update_mvpv_matrix = true;
	}

	void			Camera::set_view_position(const Vec3f& position)
	{
		m_view_position += position;
		m_update_view_matrix = true;
		m_update_mvpv_matrix = true;
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
		if (m_projection_mode == ProjectionMode::Perspective)
		{
				float fov = 60;
				float aspect_ratio = 1;
				float near = 0.1;
				float far = 100.f;
				float fov_rad = fov * (M_PI / 180.0);;
				float tan_half_fov = std::tan(fov_rad / 2);
				float f = 1.0f / tan_half_fov;
				m_projection_matrix = {
					f / aspect_ratio, 0, 0, 0,
					0, f, 0, 0,
					0, 0, (far + near) / (near - far), (2 * far * near) / (near - far),
					0, 0, -1, 0
				};

		}
		else
		{
			float l = -2;
			float r = 2;
			float b = -2;
			float t = 2;
			float n = 0.1;
			float f = 100;
			m_projection_matrix = {
				2 / (r - l), 0, 0, -(r + l) / (r - l),
				0, 2 / (t - b), 0, -(t + b) / (t - b),
				0, 0, -2 / (f - n), -(f + n) / (f - n),
				0, 0, 0, 1
			};
		}
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
		Mat4 translate_viewport_matrix= {
				1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1};

		Mat4 scale_viewport_matrix = {
				size / 2.f, 0, 0, 0,
				0, size / 2.f, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};

		m_viewport_matrix = scale_viewport_matrix * translate_viewport_matrix;
	}



}
