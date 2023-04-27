#include "Model.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstring>

Model::Model(const char* filename)
	: m_filename(filename)
{
	m_resultCode = init_model();

	std::cout << "model file:\t";
	std::cout << (strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename) << std::endl;
	std::cout << "loading:\t";
	if (m_resultCode)
	{
		std::cout << "FAILED" << std::endl;
	}
	else
	{
		std::cout << "OK" << std::endl;
		std::cout << "m_verts:\t" << m_verts.size() << std::endl << "m_faces:\t" << m_faces.size() << std::endl;
	}
	std::cout << std::endl;
}


int	Model::init_model()
{
	std::ifstream file;
	file.open(m_filename, std::ifstream::in);
	if (file.fail())
	{
		std::cerr << "cannot open file: " << m_filename << std::endl;
		file.close();
		return 1;
	}
	float max = 0;
	std::string line;
	while (!file.eof())
	{
		std::getline(file, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v.raw[i];
				if (max < abs(v.raw[i]))
					max = abs(v.raw[i]);
			}
			m_verts.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			// std::string test_string;
			while (iss >> idx >> trash >> itrash >> trash >> itrash)
			// while(iss >> test_string)
			{
				// idx = std::stoi(test_string);
				idx--;
				f.push_back(idx);
			}
			m_faces.push_back(f);
		}
	}

	if (max > 1)
	{
		for( auto& vec : m_verts)
		{
			vec.x /= max;
			vec.y /= max;
			vec.z /= max;
		}
	}
	return 0;
}

Model::~Model()
{

}
