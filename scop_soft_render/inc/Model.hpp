#pragma once

#include <vector>
#include "Vector.hpp"

class Model
{
public:
	const char*						m_filename;
	int								m_resultCode;
	std::vector<Vec3f>				m_v;
	std::vector<Vec2f>				m_vt;
	std::vector<Vec3f>				m_vn;
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
	const std::vector<Vec3f>&					get_v() const { return m_v; }
	const std::vector<Vec2f>&					get_vt() const { return m_vt; }
	const std::vector<Vec3f>&					get_vn() const { return m_vn; }
	const std::vector<std::vector<int>>&		get_f_v() const { return m_f_v; }
	const std::vector<std::vector<int>>&		get_f_vt() const { return m_f_vt; }
	const std::vector<std::vector<int>>&		get_f_vn() const { return m_f_vn; }
	bool										is_vt() const { return m_vt.size(); }
	bool										is_vn()const { return m_vn.size(); }
	bool										is_f_vt() const { return m_f_vt.size(); }
	bool										is_f_vn()const { return m_f_vn.size(); }
};
