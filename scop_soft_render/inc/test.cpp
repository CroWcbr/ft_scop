#include "Vector.hpp"
#include "Matrix.hpp"

int main()
{
	Vec3f b;
	b = {1.3, 2.6, 6.5};
	std::cout << b << std::endl;
	std::cout << b[2] << std::endl;

	Vec3i a(b);
	std::cout << a << std::endl;


	Mat4 ttt;
	ttt = {1, 0, 0, 0.2,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1};

	std::cout << ttt << std::endl;
	// Vec4i bbb(b);
	// std::cout << bbb << std::endl;

	// Vec2i bbbb(b);
	// std::cout << bbbb << std::endl;

	// // ft::Vec3f c = {1.2, 2.3, 4.6};
	// // std::cout << c << std::endl;
	// Vec3i d(ft::Vec3f({1.2, 2.3, 4.6}));
	// std::cout << d << std::endl;
	// // std::cout << d[2] << std::endl;

	// Vec3i ttt;
	// ttt = b + d;
	// std::cout << ttt << std::endl;
	// std::cout << "222221" << std::endl;
	return 0;
}