#include "VertexArray.hpp"

namespace Scop
{

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer, GLint size, GLenum type)
	{
		bind();
		vertex_buffer.bind();
		glEnableVertexAttribArray(m_elements_count);
		glVertexAttribPointer(
			m_elements_count,
			size,
			type,
			GL_FALSE,
			3 * sizeof(float),
			nullptr
		);
		++m_elements_count;
		unbind();
	}

	void VertexArray::set_index_buffer(const IndexBuffer& index_buffer)
	{
		bind();
		index_buffer.bind();
	}
}
