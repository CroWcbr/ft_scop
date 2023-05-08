#include "../inc/Vector.hpp"

int main()
{
	Vec3i b;
	b = {1, 2, 6};
	std::cout << b << std::endl;
	std::cout << b[2] << std::endl;

	Vec3i a({1, 2, 3});
	std::cout << a << std::endl;

	Vec4i bbb(b);
	std::cout << bbb << std::endl;

	Vec2i bbbb(b);
	std::cout << bbbb << std::endl;

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