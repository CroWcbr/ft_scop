#pragma once

#include <vector>
#include <cstdint>

namespace Scop
{

	class VertexBuffer
	{
	private:
		unsigned int m_id = 0;

	public:
		VertexBuffer(const void* data, const size_t size);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertex_buffer) = delete;
		VertexBuffer(VertexBuffer&& vertex_buffer) = delete;

		void	bind() const;
		void	unbind() const;
	};

}
