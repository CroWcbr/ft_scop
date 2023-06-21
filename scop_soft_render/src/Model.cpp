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

Model::~Model()
{}

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
	std::string line;
	std::vector<std::vector<std::string>> f_tmp;
	while (!file.eof())
	{
		std::getline(file, line);
		// std::cout << line << std::endl;
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v[i];
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
		}
	}
	file.close();

	return optimization_model();
}

int	Model::optimization_model()
{
	if (m_v.empty())
	{
		std::cerr << "No model point in file: " << m_filename << std::endl;
		return 2;
	}
	Vec3f	_min, _max;
	_min = _max = m_v[0];
	for (const auto& v : m_v)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (_min[i] > v[i])
				_min[i] = v[i];
			if (_max[i] < v[i])
				_max[i] = v[i];
		}
	}
	Vec3f	delta_v = (_max + _min) / 2;
	Vec3f	delta_v_max = (_max - _min) / 2;;
	float max_v = (std::max({delta_v_max.x(), delta_v_max.y(), delta_v_max.z()}));
	for (auto& v : m_v)
	{
		v = (v - delta_v) / max_v;
	}

	if (m_vt.empty())
	{
		if (m_f_vt.empty())
		{
			m_f_vt = m_f_v;
			SphericalTextureCoords();
		}
		else
		{
			for (auto &c : m_v)
			m_vt.push_back(Vec2f({c.x(), c.y()}));
		}
	}

	if (m_vn.empty())
	{
		if (m_f_vn.empty())
		{
			m_f_vn = m_f_v;
			calculateNormal();
		}
		else
		{
			m_vn = m_v;
		}
	}
	return 0;
}

void Model::SphericalTextureCoords()
{
	for (const auto& v : m_v)
	{
		float theta = std::atan2(v.z(), v.x());
		float phi = std::acos(v.y());
		float uu = 1 - std::abs(theta) / (2.0f * M_PI);
		float vv = 1 - std::abs(phi) / M_PI;
		m_vt.push_back(Vec2f({ uu, vv }));
	}
}

void Model::CylindricalTextureCoords()
{
	for (const auto& v : m_v)
	{
		float theta = std::atan2(v.z(), v.x());
		float vv = v.y();
		float uu = std::abs(theta) / (2.0f * M_PI);
		vv = (vv + 1.0f) / 2.0f;
		m_vt.push_back(Vec2f({ uu, vv }));
	}
}

void Model::PlanarTextureCoords()
{
	Vec3f	_max;
	_max = m_v[0];
	_max[2] = 0;
	for (const auto& v : m_v)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (_max[i] < v[i])
				_max[i] = v[i];
		}
	}
	float max_uv = std::max(_max.x(), _max.y());
	for (const auto& v : m_v)
	{
		float uu = (v.x() + max_uv) / (2 * max_uv);
		float vv = (v.y() + max_uv) / (2 * max_uv);
		m_vt.push_back(Vec2f({ uu, vv }));
	}
}

void Model::calculateNormal()
{
	m_vn.assign(m_v.size(), {0,0,0});
	std::vector<int> counts(m_v.size(), 0);
	for (const auto& f : m_f_v)
	{
		const Vec3f& v0 = m_v[f[0]];
		const Vec3f& v1 = m_v[f[1]];
		const Vec3f& v2 = m_v[f[2]];
		Vec3f side1 = v1 - v0;
		Vec3f side2 = v2 - v0;
		Vec3f normal = side1 ^ side2;
		m_vn[f[0]] += normal;
		m_vn[f[1]] += normal;
		m_vn[f[2]] += normal;
		counts[f[0]] += 1;
		counts[f[1]] += 1;
		counts[f[2]] += 1;
	}
	for (size_t i = 0; i < m_vn.size(); ++i)
	{
		if (counts[i] > 0)
		{
			m_vn[i] = (m_vn[i] / static_cast<float>(counts[i])).normalize();
		}
	}
}
