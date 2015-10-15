#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LED_Object.h"

class CLED_Camera : public CLED_Object
{
private:
	glm::mat4x4 m_viewMat;
	glm::mat4x4 m_ProjMat;
	//view的三个轴
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
	void stepForwardBack(float step);	//前后移动，负值为后，正值为前
	void stepRightLeft(float step);		//左右移动，负值为左，正值为右
	void stepUpDown(float step);		//上下移动，负值为下，正值为上
	void roll(float angle);				//横滚
	void pitch(float angle);			//仰角
	void yaw(float angle);				//扫视
};