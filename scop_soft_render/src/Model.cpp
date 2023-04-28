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
		std::cout << "m_verts:\t" << m_verts.size() << std::endl << "m_uv:\t\t" << m_uv.size() << std::endl;
		std::cout << "m_norms:\t" << m_norms.size() << std::endl << "m_faces:\t" << m_faces.size() << std::endl;
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
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec2f v;
			for (int i = 0; i < 2; i++)
			{
				iss >> v.raw[i];
			}
			m_uv.push_back(v);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v.raw[i];
			}
			m_norms.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			std::vector<int> uv;
			std::vector<int> n;
			int idx1, idx2, idx3;
			iss >> trash;
			while (iss >> idx1 >> trash >> idx2 >> trash >> idx3)
			{
				f.push_back(--idx1);
				uv.push_back(--idx2);
				n.push_back(--idx3);
			}
			m_faces.push_back(f);
			m_uv_faces.push_back(uv);
			m_norms_faces.push_back(n);
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
