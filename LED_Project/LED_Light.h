#pragma once

#include "LED_Object.h"
#include "LED_header.h"
#include "LED_Camera.h"
#include <glm/glm.hpp>
#include <vector>

class CLED_Light : public CLED_Object
{
protected:
	glm::mat4x4 m_transMat;
public:
	CLED_Light():CLED_Object()
	{

	}
	bool		m_lightOn;		//灯是否打开了
	unsigned int m_type;
	glm::vec3	m_ambient;
	glm::vec3	m_diffuse;
	glm::vec3	m_specular;
	glm::vec3	m_position;		//聚光灯、点光源的位置
	glm::vec3	m_direction;	//方向光、聚光灯的方向
	glm::vec3	m_fallOff;		//衰减
	float		m_diffusePower; //散射光强度
	float		m_range;		//范围
	float		m_attenuation0;	//聚光灯系数
	float		m_attenuation1;	//聚光灯系数
	float		m_attenuation2;	//聚光灯系数
	float		m_theta;		//聚光灯内角
	float		m_phi;			//聚光灯外角

	void light_forwardBack(float step);
	void light_rightLeft(float step);
	void light_upDown(float step);
    
};