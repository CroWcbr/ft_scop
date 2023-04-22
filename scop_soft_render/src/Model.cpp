#include "Model.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Model::Model(const char* filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail())
	{
		std::cerr << "cannot open file" << std::endl;
		return;
	}
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v.raw[i];
			}
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash)
			{
				idx--;
				f.push_back(idx);
			}
			faces_.push_back(f);
		}
	}
	std::cout << "verts_: " << verts_.size() << std::endl << "faces_: " << faces_.size() << std::endl;
}

Model::~Model()
{
}
