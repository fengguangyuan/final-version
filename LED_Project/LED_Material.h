#pragma once

#include <glm/glm.hpp>
#include "LED_Object.h"

class CLED_Material : public CLED_Object
{
public:
	glm::vec3		m_emissive;
	glm::vec3		m_ambient;
	glm::vec3		m_diffuse;
	glm::vec3		m_specular;
	float			m_specularPower;
	unsigned int	m_texBlendMode;		//��Ϸ�ʽ
	unsigned int	m_depthCheckMode;	//��Ȳ���

	CLED_Material():CLED_Object()
	{

	}
};