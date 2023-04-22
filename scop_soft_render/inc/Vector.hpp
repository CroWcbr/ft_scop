#pragma once

template <class t>
struct Vec2
{
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u),v(_v) {}
};

template <class t>
struct Vec3
{
	union {
		struct {t x, y, z;};
		struct {t ivert, iuv, inorm;};
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
};

typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;
typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;
