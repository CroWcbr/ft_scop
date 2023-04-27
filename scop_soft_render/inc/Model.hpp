#pragma once

#include <vector>
#include "Vector.hpp"

class Model
{
private:
	const char*						m_filename;
	int								m_resultCode;
	std::vector<Vec3f>				m_verts;
	std::vector<std::vector<int> >	m_faces;

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

	const int										getResultCode() const { return m_resultCode; }
	const std::vector<Vec3f>&						getVerts() const { return m_verts; }
	const std::vector<std::vector<int>>&			getFaces() const { return m_faces; }
};
