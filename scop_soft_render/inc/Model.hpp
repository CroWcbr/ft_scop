#pragma once

#include <vector>
#include "Vector.hpp"

class Model
{
public:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;

public:
	Model() = delete;
	Model(const char* filename);
	~Model();
};
