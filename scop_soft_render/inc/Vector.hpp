#pragma once

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <cmath>

namespace ft
{

	template <size_t N>
	class Vec
	{
		private:
			typedef		double								value_type;
			typedef		std::size_t							size_type;
			typedef		std::initializer_list<double>		init_list;

		private:
			value_type		m_data[N];

		public:
			Vec() { for (size_t i = 0; i < N; ++i) m_data[i] = 0; }
			Vec(const Vec& copy) { *this = copy; }
			Vec(const init_list& data) { *this = data; }
			~Vec() {}

			Vec&		operator=(const Vec& copy)
			{
				std::copy(copy.m_data, copy.m_data + N, m_data);
				return *this;
			}

			Vec&		operator=(const init_list& data)
			{
				assert (data.size() == N);
				std::copy(data.begin(), data.end(), m_data);
				return *this;
			}

			Vec			operator+(const Vec& vec) const
			{
				Vec result(*this);
				return result += vec;
			}

			Vec&		operator+=(const Vec& vec)
			{
				std::transform(m_data, m_data + N, vec.m_data, m_data, std::plus<value_type>());
				return *this;
			}

			Vec			operator-(const Vec& vec) const
			{
				Vec result(*this);
				return result -= vec;
			}

			Vec&		operator-=(const Vec& vec)
			{
				std::transform(m_data, m_data + N, vec.m_data, m_data, std::minus<value_type>());
				return *this;
			}

			value_type			operator*(const Vec& vec) const
			{
				value_type result = 0;
				for (size_type i = 0; i < N; ++i)
					result += m_data[i] * vec[i];
				return result;
			}

			Vec			operator*(double d) const
			{
				Vec result(*this);
				return result *= d;
			}

			Vec&		operator*=(double d)
			{
				std::transform(m_data, m_data + N, m_data, [d](value_type x) { return x * d; });
				return *this;
			}

			friend Vec	operator*(double d, const Vec& vec)
			{
				return vec * d;
			}

			value_type	norm () const
			{
				value_type result = 0;
				for (size_type i = 0; i < N; ++i)
					result += m_data[i] * m_data[i];
				return std::sqrt(result);
			}

			Vec&		normalize()
			{
				const double n = norm();
				if (n != 0.0)
					*this = (*this) * ( 1 / n);
				return *this;
			}

			Vec			operator^(const Vec &v) const
			{
				assert (N == 3);
				return Vec({y() * v.z() - z() * v.y(), z() * v.x() - x() * v.z(), x() * v.y() - y() * v.x()});
			}

			value_type&			operator[](size_type i) { assert(i >= 0 && i < N); return m_data[i]; }
			const value_type&	operator[](size_type i) const { assert(i >= 0 && i < N); return m_data[i]; }
			value_type&			x() { return (*this)[0]; }
			const value_type&	x() const { return (*this)[0]; }
			value_type&			y() { return (*this)[1]; }
			const value_type&	y() const { return (*this)[1]; }
			value_type&			z() { return (*this)[2]; }
			const value_type&	z() const { return (*this)[2]; }
			value_type&			w() { return (*this)[3]; }
			const value_type&	w() const { return (*this)[3]; }
			const value_type*	getVec() const { return m_data; }

			friend std::ostream&	operator<<(std::ostream& os, const Vec& v)
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

using Vec2 = ft::Vec<2>;
using Vec3 = ft::Vec<3>;
using Vec4 = ft::Vec<4>;
