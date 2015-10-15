#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <wx/wx.h>
#include "LED_Object.h"
#include "shadow.h"

class CLED_ObjectModel : public CLED_Object
{
	friend class CLED_ObjectModel_EXT;
	friend class CLoad3DS;
public:
	CLED_ObjectModel();
	CLED_ObjectModel(const wxString& filename);
	virtual ~CLED_ObjectModel();

	unsigned int		faceCount();
	const std::vector<glm::vec3>& getVertices() const;
	const std::vector<glm::vec2>& getUVs() const;
	const std::vector<glm::vec3>& getNormals() const;
	
	const std::vector<glm::vec3>& getFaceCoor ( ) const
	{
		return m_faceCoors;
	}
	/******************************************************/
	//this var is used to produce a shadow
	ShadowedObject obj;
	wxString objName;
	const std::vector<glm::vec3>& getPoints() const;
	const std::vector<glm::vec2>& getUVCooors() const;
	const std::vector<glm::vec3>& getNormalCooors() const;
	const void setLocation(glm::vec4 &pos);
	glm::vec4 & getLocation(){ return location; }
	//CLED_ObjectModel(CLED_ObjectModel & model)
	//{
	//	obj = model.obj;
	//	m_pos = model.m_pos;
	//	m_faceCount = model.m_faceCount;
	//	m_UVs = model.m_UVs;
	//	m_normals = model.m_normals;
	//	m_faceCoors = model.m_faceCoors;
	//	m_boundingSphereCenter = model.m_boundingSphereCenter;
	//	m_boundingSphereRadius = model.m_boundingSphereRadius;
	//}
	//CLED_ObjectModel operator=(CLED_ObjectModel & model)
	//{
	//}
	/******************************************************/
	const glm::vec3&	getBoundingSphereCenter();
	float				getBoundingSphereRadius();
private:
	glm::vec3	m_boundingSphereCenter;
	float		m_boundingSphereRadius;
	unsigned int m_faceCount;
	std::vector<glm::vec3>	m_pos;
	std::vector<glm::vec2>	m_UVs;
	std::vector<glm::vec3>	m_normals;
	std::vector<glm::vec3>	m_Position;
	std::vector<glm::vec2>	m_UVCooors;
	std::vector<glm::vec3>	m_NormalCooors;
	glm::vec4 location;
	std::vector<glm::vec3>  m_faceCoors;
};