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
	bool		m_lightOn;		//���Ƿ����
	unsigned int m_type;
	glm::vec3	m_ambient;
	glm::vec3	m_diffuse;
	glm::vec3	m_specular;
	glm::vec3	m_position;		//�۹�ơ����Դ��λ��
	glm::vec3	m_direction;	//����⡢�۹�Ƶķ���
	glm::vec3	m_fallOff;		//˥��
	float		m_diffusePower; //ɢ���ǿ��
	float		m_range;		//��Χ
	float		m_attenuation0;	//�۹��ϵ��
	float		m_attenuation1;	//�۹��ϵ��
	float		m_attenuation2;	//�۹��ϵ��
	float		m_theta;		//�۹���ڽ�
	float		m_phi;			//�۹�����

	void light_forwardBack(float step);
	void light_rightLeft(float step);
	void light_upDown(float step);
    
};