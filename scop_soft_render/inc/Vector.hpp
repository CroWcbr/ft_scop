#pragma once

#include <iostream>
#include <memory>
#include <cassert>
#include <type_traits>
#include <cstring>

namespace ft
{

	#define ASSERT_VEC(condition, message) \
	do { \
		if (! (condition)) \
		{ \
			std::cerr << "Vector ERROR: " << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)

	template <size_t N, typename T>
	class Vector
	{
		private:
			typedef		T									value_type;
			typedef		T*									pointer;
			typedef		T&									reference;
			typedef		std::size_t							size_type;
			typedef		std::initializer_list<value_type>	init_list;

		private:
			pointer			m_data;

		public:
			Vector()
				: m_data(new value_type[N]())
			{}

			~Vector() 
			{
				delete [] m_data;
			}

			Vector(const Vector& copy)
				:  m_data(new value_type[N])
			{
				*this = copy;
			}

			template <size_t N2>
			Vector(const Vector<N2, T>& copy)
				:  m_data(new value_type[N])
			{
				*this = copy;
			}

			Vector(init_list data)
				: m_data(new value_type[N])
			{
				*this = data;
			}

			Vector&				operator=(const Vector &copy)
			{
				std::copy(copy.m_data, copy.m_data + N, m_data);
				return *this;
			}

			template <size_t N2>
			Vector&				operator=(const Vector<N2, T>& copy)
			{
				if (N > N2)
				{
					std::copy(copy.getVector(), copy.getVector() + N2, m_data);
					(*this)[3] = 1;
				}
				else
				{
					std::copy(copy.getVector(), copy.getVector() + N, m_data);
				}
				return *this;
			}

			Vector&				operator=(const init_list data)
			{
				ASSERT_VEC(data.size() == N, "init_list.size() != Vector.size()");
				std::copy(data.begin(), data.end(), m_data);
				return *this;
			}

			value_type&			operator[](size_type index)
			{
				ASSERT_VEC(index < N, "wrong index in operator []");
				return m_data[index];
			}
			const value_type&	operator[](size_type index) const
			{
				ASSERT_VEC(index < N, "wrong index in operator []");
				return m_data[index];
			}

	// 		template <typename U>
	// 		Vec(const Vec<N, U>& copy)
	// 			: m_size(N)
	// 			, m_vec(new value_type[N])
	// 		{
	// std::cout << "Vec(const Vec<N, U>& copy)" << std::endl;
	// 			*this = copy;
	// 		}

	// 		template <typename U>
	// 		Vec	&operator=(const Vec<N, U> &copy)
	// 		{
	// std::cout << "Vec	&operator=(const Vec<N, U> &copy)" << std::endl;
	// 			if (std::is_same_v<T, int> && std::is_same_v<U, float>)
	// 			{
	// 				for (size_type i = 0; i < N; ++i)
	// 				{
	// 					m_vec[i] = static_cast<int>(copy[i] + 0.5);
	// 				}
	// 			}
	// 			else if (std::is_same_v<T, float> && std::is_same_v<U, int>)
	// 			{
	// 				for (size_type i = 0; i < N; ++i)
	// 				{
	// 					m_vec[i] = static_cast<float>(copy[i]);
	// 				}
	// 			}
	// 			return *this;
	// 		}



			// template <typename U>
			// Vec<N, U>			operator+(const Vec<N, U> &vec) const
			// {
			// 	Vec<N, U> result;
			// 	for (size_type i = 0; i < m_size; ++i)
			// 	{
			// 		result.m_vec[i] = m_vec[i] + vec.m_vec[i];
			// 	}
			// 	return result;
			// }

			// template <typename U>
			// Vec<N, U>&		operator+=(const Vec<N, U> &vec)
			// {
			// 	for (size_type i = 0; i < m_size; i++)
			// 	{
			// 		m_vec[i] += vec[i];
			// 	}
			// 	return *this;
			// }

			size_type			getSize() const { return N; }
			pointer				getVector() const { return m_data; }
			value_type&			x() { return (*this)[0]; }
			const value_type&	x() const { return (*this)[0]; }
			value_type&			y() { return (*this)[1]; }
			const value_type&	y() const { return (*this)[1]; }
			value_type&			z() { return (*this)[2]; }
			const value_type&	z() const { return (*this)[2]; }
			value_type&			w() { return (*this)[3]; }
			const value_type&	w() const { return (*this)[3]; }

			friend std::ostream& operator<<(std::ostream& os, const Vector<N, T>& v)
			{
				os << "[";
				for (size_type i = 0; i < N; i++)
				{
					os << v[i];
					if (i != N - 1)
					{
						os << ", ";
					}
				}
				os << "]";
				return os;
			}
	};

}

using Vec2i = ft::Vector<2, int>;
using Vec2f = ft::Vector<2, float>;
using Vec3i = ft::Vector<3, int>;
using Vec3f = ft::Vector<3, float>;
using Vec4i = ft::Vector<4, int>;
using Vec4f = ft::Vector<4, float>;
