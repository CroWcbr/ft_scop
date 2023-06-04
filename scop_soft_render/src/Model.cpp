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
		std::cout << "m_faces_uv:\t" << m_f_vt.size() << std::endl << "m_faces_norms:\t" << m_f_vn.size() << std::endl;
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
	float max_v = 0;
	float max_f = 0;
	std::string line;
	std::vector<std::vector<std::string>> f_tmp;
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
				iss >> v[i];
				if (max_v < abs(v[i]))
					max_v = abs(v[i]);
			}
			m_v.push_back(v);
		}
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec2f vt;
			for (int i = 0; i < 2; i++)
			{
				iss >> vt[i];
			}
			m_vt.push_back(vt);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			Vec3f vn;
			for (int i = 0; i < 3; i++)
			{
				iss >> vn[i];
			}
			m_vn.push_back(vn);
		}
		else if (!line.compare(0, 2, "f "))
		{
			iss >> trash;
			std::vector<int> f_v;
			std::vector<int> f_vt;
			std::vector<int> f_vn;

			std::string tmp;
			std::stringstream ss_line(line.substr(line.find(' ') + 1));
			std::vector<std::string> split_line;
			while(std::getline(ss_line, tmp, ' '))
			{
				split_line.push_back(tmp);
			}
			std::vector<std::string> split_elem;
			for (auto& elem : split_line)
			{
			
				std::stringstream ss_elem(elem);
				std::vector<int> split_elem;
				while(std::getline(ss_elem, tmp, '/'))
				{
					if (tmp.empty())
						tmp = "0";
					split_elem.push_back(std::stoi(tmp) - 1);
				}
				f_v.push_back(split_elem[0]);
				if (m_vt.size())
					f_vt.push_back(split_elem[1]);
				if (m_vn.size())
					f_vn.push_back(split_elem[2]);
				if (f_v.size() == 3)
				{
					m_f_v.push_back(f_v);
					f_v[1] = f_v[2];
					f_v.pop_back();
				}
				if (f_vt.size() == 3)
				{
					m_f_vt.push_back(f_vt);
					f_vt[1] = f_vt[2];
					f_vt.pop_back();
				}
				if (f_vn.size() == 3)
				{
					m_f_vn.push_back(f_vn);
					f_vn[1] = f_vn[2];
					f_vn.pop_back();
				}
			}
			// std::cout << f_v[0] << "\t" << f_vt[0] << "\t" << f_vn[0] << std::endl;
		}
	}

	if (max_v > 1)
		for(auto& vec : m_v)
			vec = vec /max_v;
	return 0;
}

Model::~Model()
{

}
