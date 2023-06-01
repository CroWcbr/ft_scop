#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <glad/glad.h>

namespace Scop
{

	class VertexArray 
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertex_buffer) = delete;
		VertexArray(VertexArray&& vertex_buffer) = delete;

		void bind() const;
		static void unbind();

		void add_vertex_buffer(const VertexBuffer& vertex_buffer, GLint size, GLenum type);
		void set_index_buffer(const IndexBuffer& index_buffer);

	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
	};

}