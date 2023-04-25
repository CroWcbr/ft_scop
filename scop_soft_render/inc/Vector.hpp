#pragma once

#include <cmath>
#include <ostream>

template <class t>
struct Vec2
{
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};

	Vec2() 
		: u(0)
		, v(0)
	{}

	Vec2(t _u, t _v)
		: u(_u)
		, v(_v)
	{}

	inline Vec2<t> operator +(const Vec2<t> &V) const
	{
		return Vec2<t>(u + V.u, v + V.v);
	}

	inline Vec2<t> operator -(const Vec2<t> &V) const
	{
		return Vec2<t>(u - V.u, v - V.v);
	}

	inline Vec2<t> operator *(float f) const
	{
		return Vec2<t>(u * f, v * f);
	}
};

template <class t>
struct Vec3
{
	union {
		struct {t x, y, z;};
		struct {t ivert, iuv, inorm;};
		t raw[3];
	};

	Vec3()
		: x(0)
		, y(0)
		, z(0)
	{}

	Vec3(t _x, t _y, t _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}

	template <class u> Vec3<t>(const Vec3<u> &v);

	inline Vec3<t> operator+(const Vec3<t> &V) const
	{
		return Vec3<t>(x + V.x, y + V.y, z + V.z);
	}

	inline Vec3<t> operator-(const Vec3<t> &v) const
	{
		return Vec3<t>(x - v.x, y - v.y, z - v.z);
	}

	inline Vec3<t> operator^(const Vec3<t> &v) const
	{
		return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	inline Vec3<t> operator *(float f) const
	{
		return Vec3<t>(x*f, y*f, z*f);
	}

	inline t operator *(const Vec3<t> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
		}

	float norm () const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3<t> & normalize(t l = 1)
	{
		*this = (*this) * ( l / norm());
		return *this;
	}
};

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;
typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;

template <> template <> Vec3<int>::Vec3<>(const Vec3<float> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {
}

template <> template <> Vec3<float>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {
}