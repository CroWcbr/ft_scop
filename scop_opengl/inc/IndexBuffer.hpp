#pragma once

#include <vector>
#include <cstdint>

namespace Scop
{

	class IndexBuffer
	{
	private:
		unsigned int m_id = 0;

	public:
		IndexBuffer(const void* data, const size_t size);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& vertex_buffer) = delete;
		IndexBuffer(IndexBuffer&& vertex_buffer) = delete;

		void	bind() const;
		void	unbind() const;
	};

}
