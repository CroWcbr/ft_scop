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
		Mat4			m_model_matrix;
		Mat4			m_view_matrix;
		Mat4			m_projection_matrix;
		Mat4			m_viewport_matrix;
		Mat4			m_mvpv_matrix;

		bool			m_update_mvpv_matrix = false;

		Vec3f			m_model_position;
		Vec3f			m_model_rotation; // X - Roll, Y - Pitch, Z - Yaw
		float			m_model_scale;
		bool			m_update_model_matrix = false;

		Vec3f			m_view_position;
		Vec3f			m_view_rotation; // X - Roll, Y - Pitch, Z - Yaw
		bool			m_update_view_matrix = false;

		ProjectionMode	m_projection_mode;

	private:
		void			update_model_matrix();
		void			update_view_matrix();
		void			update_projection_matrix();
		void			update_viewport_matrix(int size = 0);
		void			update_mvpv_matrix();

	public:
		Camera(const Vec3f& model_position = { 0, 0, 0 }, \
			const Vec3f& model_rotation = { 0, 0, 0 },
			const float model_scale = 1,
			const Vec3f& view_position = { 0, 0, 0 }, \
			const Vec3f& view_rotation = { 0, 0, 0 },
			const ProjectionMode projection_mode = ProjectionMode::Perspective);

		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera&	operator=(const Camera&) = delete;
		Camera&	operator=(const Camera&&) = delete;

		const Mat4&		get_mvpv_matrix();
		const Mat4&		get_model_matrix() const;
		const Mat4&		get_view_matrix() const;
		const Mat4&		get_projection_matrix() const;
		const Mat4&		get_viewport_matrix() const;

		void			set_model_rotation(const Vec3f& rotation);
		void			set_model_position(const Vec3f& position);
		void			set_model_rotation_and_position(const Vec3f& rotation, const Vec3f& position);
		void			set_model_scale(const float scale);
		void			set_view_rotation(const Vec3f& rotation);
		void			set_view_position(const Vec3f& position);
		void			change_projection_matrix();
		void			change_viewport_matrix(int size);
	};

}
