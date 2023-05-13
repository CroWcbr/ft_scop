#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

namespace Scop
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

	private:
		ProjectionMode	m_projection_mode;
		Vec3f			m_position;
		Vec3f			m_rotation; // X - Roll, Y - Pitch, Z - Yaw
		bool			m_update_view_matrix = false;

		Mat4			m_view_matrix;
		Mat4			m_projection_matrix;

	private:
		void	update_view_matrix();
		void	update_projection_matrix();

	public:
		Camera(const Vec3f& position = { -5, 0, 0 }, \
			const Vec3f& rotation = { 0, 0, 0 },
			const ProjectionMode projection_mode = ProjectionMode::Perspective);
		
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera&	operator=(const Camera&) = delete;
		Camera&	operator=(const Camera&&) = delete;

		void			set_rotation(const Vec3f& rotation);
		void			set_position(const Vec3f& position);
		const Mat4		get_view_matrix();
		void			change_projection();
		
	};

}
