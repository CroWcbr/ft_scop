#pragma once

#include "Matrix.hpp"


namespace Scop
{

	class ShaderProgram
	{
	private:
		unsigned int	m_id = 0;
		int				m_resultCode;

		int				init(const char* vertex_shader_src, const char* fragment_shader_src);

	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;
	
		void				bind() const;
		static void			unbind();
		inline int			getResultCode() const { return m_resultCode; }
	};

}