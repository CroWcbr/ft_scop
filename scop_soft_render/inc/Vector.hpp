#pragma once

#include <iostream>
#include <memory>
#include <cassert>
#include <type_traits>
#include <cstring>
#include <algorithm>
#include <cmath>

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

	template <size_t N, \
			typename T, \
			typename = std::enable_if_t<std::is_same<T, int>::value || std::is_same<T, float>::value>, \
			typename = std::enable_if_t<(N >= 2 && N <= 4)>>
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

			Vector(init_list data)
				: m_data(new value_type[N])
			{
				*this = data;
			}

			template <size_t N2>
			Vector(const Vector<N2, T>& copy, T a)
				:  m_data(new value_type[N])
			{
				ASSERT_VEC(N == N2 + 1, "wrong use Vector(const Vector<N2, T>& copy, T a)");
				std::copy(copy.getVector(), copy.getVector() + N2, m_data);
				m_data[N2] = a;
			}

			template <size_t N2>
			Vector(const Vector<N2, T>& copy, T a, T b)
				:  m_data(new value_type[N])
			{
				ASSERT_VEC(N == N2 + 2, "wrong use Vector(const Vector<N2, T>& copy, T a, T b)");
				std::copy(copy.getVector(), copy.getVector() + N2, m_data);
				m_data[N2 - 1] = a;
				m_data[N2] = b;
			}

			template <typename U>
			Vector(const Vector<N, U>& copy)
				:  m_data(new value_type[N])
			{
				*this = copy;
			}

			Vector&				operator=(const Vector& copy)
			{
				std::copy(copy.m_data, copy.m_data + N, m_data);
				return *this;
			}

			template <typename U>
			Vector&				operator=(const Vector<N, U>& copy)
			{
				if (std::is_same<U, float>::value)
				{
					std::transform(copy.getVector(), copy.getVector() + N, m_data, 
							[](U x) { return static_cast<T>(x + 0.5); });
				}
				else
				{
					std::transform(copy.getVector(), copy.getVector() + N, m_data, 
							[](U x) { return static_cast<T>(x); });
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

			Vector				operator+(const Vector& vec) const
			{
				Vector result(*this);
				return result += vec;
			}

			Vector&				operator+=(const Vector& vec)
			{
				std::transform(m_data, m_data + N, vec.m_data, m_data, std::plus<value_type>());
				return *this;
			}

			Vector				operator-(const Vector& vec) const
			{
				Vector result(*this);
				return result -= vec;
			}

			Vector&				operator-=(const Vector& vec)
			{
				std::transform(m_data, m_data + N, vec.m_data, m_data, std::minus<value_type>());
				return *this;
			}

			Vector<N, float>	operator*(float d) const
			{
				Vector<N, float> result(*this);
				for (int i = 0; i < N; ++i)
				{
					result[i] = result[i] * d;
				}
				return result;
			}

			Vector				operator/(float d) const
			{
				Vector result(*this);
				return result /= d;
			}

			Vector&				operator/=(float d)
			{
				ASSERT_VEC(d != 0, "wrong use operator/ - zero division");
				std::transform(m_data, m_data + N, m_data, [d](value_type x) { return x / d; });
				return *this;
			}

			value_type			operator*(const Vector& vec) const
			{
				value_type result = 0;
				for (size_type i = 0; i < N; ++i)
					result += m_data[i] * vec[i];
				return result;
			}

			Vector			operator^(const Vector &v) const
			{
				ASSERT_VEC(N == 3, "wrong use operator^");
				return Vector({y() * v.z() - z() * v.y(), z() * v.x() - x() * v.z(), x() * v.y() - y() * v.x()});
			}

			value_type		norm () const
			{
				value_type result = 0;
				for (size_type i = 0; i < N; ++i)
					result += m_data[i] * m_data[i];
				return std::sqrt(result);
			}

			Vector&			normalize()
			{
				const float n = norm();
				if (n > 0)
					*this = (*this) * ( 1 / n);
				return *this;
			}

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
