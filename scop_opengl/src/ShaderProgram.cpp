#include "ShaderProgram.hpp"
#include <glad/glad.h>

namespace Scop
{

	bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id)
	{
		shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			char info_log[1024];
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);
			std::cout << "Shader compilation error: " << info_log  << std::endl;
			return false;
		}
		return true;
	}

	ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src)
	{
		m_resultCode = init(vertex_shader_src, fragment_shader_src);
	}

	int	ShaderProgram::init(const char* vertex_shader_src, const char* fragment_shader_src)
	{
		GLuint vertex_shader_id = 0;
		if (!create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id))
		{
			std::cout << "VERTEX SHADER: compile-time error!" << std::endl;
			glDeleteShader(vertex_shader_id);
			return -1;
		}

		GLuint fragment_shader_id = 0;
		if (!create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id))
		{
			std::cout << "FRAGMENT SHADER: compile-time error!" << std::endl;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return -2;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			std::cout << "SHADER PROGRAM: Link-time error: " <<  info_log << std::endl;
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return -3;
		}

		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		
		return 0;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	// ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram)
	// {
	// 	glDeleteProgram(m_id);
	// 	m_id = shaderProgram.m_id;
	// 	m_isCompiled = shaderProgram.m_isCompiled;

	// 	shaderProgram.m_id = 0;
	// 	shaderProgram.m_isCompiled = false;
	// 	return *this;
	// }

	// ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
	// {
	// 	m_id = shaderProgram.m_id;
	// 	m_isCompiled = shaderProgram.m_isCompiled;

	// 	shaderProgram.m_id = 0;
	// 	shaderProgram.m_isCompiled = false;
	// }

	// void ShaderProgram::setMatrix4(const char* name, const Mat4& matrix) const
	// {
	// 	// glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	// }

}