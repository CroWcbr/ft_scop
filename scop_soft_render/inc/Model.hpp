#pragma once

#include <vector>
#include "Vector.hpp"

class Model
{
public:
	const char*						m_filename;
	int								m_resultCode;
	std::vector<Vec3f>				m_verts;
	std::vector<Vec2f>				m_uv;
	std::vector<Vec3f>				m_norms;
	std::vector<std::vector<int> >	m_faces;
	std::vector<std::vector<int> >	m_uv_faces;
	std::vector<std::vector<int> >	m_norms_faces;

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
	const std::vector<Vec2f>&						getUV() const { return m_uv; }
	const std::vector<Vec3f>&						getNorms() const { return m_norms; }
	const std::vector<std::vector<int>>&			getFaces() const { return m_faces; }
	const std::vector<std::vector<int>>&			getUVFaces() const { return m_uv_faces; }
	const std::vector<std::vector<int>>&			getNormsFaces() const { return m_norms_faces; }
};
