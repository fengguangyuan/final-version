#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LED_Object.h"

class CLED_Camera : public CLED_Object
{
private:
	glm::mat4x4 m_viewMat;
	glm::mat4x4 m_ProjMat;
	//view��������
	glm::vec3	m_pos;
	glm::vec3	m_dir;
	glm::vec3	m_up;
	glm::vec3	m_right;
	float		m_fov;
	float		m_near;
	float		m_far;
	float		m_width;
	float		m_height;

	void updateView();
public:
	CLED_Camera(const glm::vec3& pos,const glm::vec3& lookat,const glm::vec3& up,float fov,float width,float height,float nearPlane,float farPlane);
	~CLED_Camera();
public:
	const glm::mat4x4& getViewMatrix() const;
	const glm::mat4x4& getProjectionMatrix() const;
	const glm::vec3& getPosition() const;
	void setPerspective(float width,float height);
	void stepForwardBack(float step);	//ǰ���ƶ�����ֵΪ����ֵΪǰ
	void stepRightLeft(float step);		//�����ƶ�����ֵΪ����ֵΪ��
	void stepUpDown(float step);		//�����ƶ�����ֵΪ�£���ֵΪ��
	void roll(float angle);				//���
	void pitch(float angle);			//����
	void yaw(float angle);				//ɨ��
};