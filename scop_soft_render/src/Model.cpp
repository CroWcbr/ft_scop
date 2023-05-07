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
		std::cout << "m_verts:\t" << m_v.size() << std::endl << "m_uv:\t\t" << m_vt.size() << std::endl;
		std::cout << "m_norms:\t" << m_vn.size() << std::endl << "m_faces:\t" << m_f_v.size() << std::endl;
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
			Vec3 v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v[i];
				if (max < abs(v[i]))
					max = abs(v[i]);
			}
			m_v.push_back(v);
		}
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec2 vt;
			for (int i = 0; i < 2; i++)
			{
				iss >> vt[i];
			}
			m_vt.push_back(vt);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			Vec3 vn;
			for (int i = 0; i < 3; i++)
			{
				iss >> vn[i];
			}
			m_vn.push_back(vn);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f_v;
			std::vector<int> f_vt;
			std::vector<int> f_vn;
			int idx1, idx2, idx3;
			iss >> trash;
			while (iss >> idx1 >> trash >> idx2 >> trash >> idx3)
			{
				f_v.push_back(--idx1);
				f_vt.push_back(--idx2);
				f_vn.push_back(--idx3);
			}
			m_f_v.push_back(f_v);
			m_f_vt.push_back(f_vt);
			m_f_vn.push_back(f_vn);
		}
	}

	if (max > 1)
	{
		for( auto& vec : m_v)
		{
			vec.x() /= max;
			vec.y() /= max;
			vec.z() /= max;
		}
	}
	return 0;
}

Model::~Model()
{

}
