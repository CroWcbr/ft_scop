#pragma once

#include <cstddef>

#include "Vector.hpp"

namespace ft
{
	#define ASSERT_MAT(condition, message) \
	do { \
		if (! (condition)) \
		{ \
			std::cerr << "Matrix ERROR: " << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)

	template <size_t M, size_t N, typename T,
				typename = std::enable_if_t<std::is_same<T, float>::value>>
	class Matrix
	{
	private:
		typedef		T									value_type;
		// typedef		T*									pointer;
		// typedef		T&									reference;
		typedef		std::size_t							size_type;
		typedef		std::initializer_list<value_type>	init_list;

	private:
		T		m_data[M][N];

	public:
		Matrix()
		{
			memset(m_data, 0, sizeof(m_data));
		}

		~Matrix()
		{}

		Matrix(const Matrix& copy)
		{
			*this = copy;
		}

		Matrix(init_list data)
		{
			*this = data;
		}

		Matrix&				operator=(const Matrix &copy)
		{
			std::copy(copy.m_data, copy.m_data + M * N, m_data);
			return *this;
		}

		Matrix&				operator=(const init_list data)
		{
			ASSERT_MAT(data.size() == M * N, "init_list.size() != Matrix.size()");
			std::copy(data.begin(), data.end(), &m_data[0][0]);
			return *this;
		}

		value_type*			operator[](size_type index)
		{
			ASSERT_VEC(index < M, "wrong index in operator []");
			return m_data[index];
		}
		const value_type*	operator[](size_type index) const
		{
			ASSERT_VEC(index < M, "wrong index in operator []");
			return m_data[index];
		}

		Vector<N, T> operator*(const Vector<M, T>& v) const
		{
			// std::cout << "!!!!!!!" << std::endl;
			// std::cout << v << std::endl;
			// std::cout << (*this) << std::endl;
			Vector<N, T> result;
			for (size_t j = 0; j < N; ++j)
			{
				T sum = 0;
				for (size_t i = 0; i < M; ++i)
				{
					sum += m_data[j][i] * v[i];
					// std::cout << "\t" << j << " " << i << "\t" << m_data[j][i] << "\t" << v[i] << "\t" <<  sum << std::endl;
				}
				result[j] = sum;
			}
			// std::cout << result << std::endl;
			// std::cout << "?????" << std::endl;
			return result;
		}

		
		friend std::ostream& operator<<(std::ostream& os, const Matrix<M, N, T>& m)
		{
			os << "[";
			for (size_type i = 0; i < M; i++)
			{
				const value_type* row = m[i];
				os << "\t";
				for (size_type j = 0; j < N; j++)
				{
					os << row[j];
					if (j != N - 1)
					{
						os << "\t";
					}
				}
				if (i != M - 1)
				{
					os << std::endl;
				}
			}
			os << "\t]";
			return os;
		}

	};
}

using Mat4 = ft::Matrix<4, 4, float>;