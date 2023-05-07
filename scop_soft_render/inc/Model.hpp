#pragma once

#include <vector>
#include "Vector.hpp"

class Model
{
public:
	const char*						m_filename;
	int								m_resultCode;
	std::vector<Vec3>			m_v;
	std::vector<Vec2>			m_vt;
	std::vector<Vec3>			m_vn;
	std::vector<std::vector<int> >	m_f_v;
	std::vector<std::vector<int> >	m_f_vt;
	std::vector<std::vector<int> >	m_f_vn;

private:
	int		init_model();

public:
	Model() = delete;
	Model(const Model& copy) = delete;
	Model& operator=(const Model& copy) = delete;
	Model(Model&& copy) = delete;
	Model& operator=(Model&& copy) = delete;

	Model(const char* filename);
	~Model();

	const int									getResultCode() const { return m_resultCode; }
	const std::vector<Vec3>&					get_v() const { return m_v; }
	const std::vector<Vec2>&					get_vt() const { return m_vt; }
	const std::vector<Vec3>&					get_vn() const { return m_vn; }
	const std::vector<std::vector<int>>&		get_f_v() const { return m_f_v; }
	const std::vector<std::vector<int>>&		get_f_vt() const { return m_f_vt; }
	const std::vector<std::vector<int>>&		get_f_vn() const { return m_f_vn; }
};
