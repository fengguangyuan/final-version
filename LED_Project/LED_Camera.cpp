#include "LED_Camera.h"

CLED_Camera::CLED_Camera(const glm::vec3& pos,const glm::vec3& lookat,const glm::vec3& up,float fov,float width,float height,float nearPlane,float farPlane):CLED_Object()
{
	m_viewMat = glm::lookAt(pos,lookat,up);
	m_dir = glm::normalize(lookat - pos);
	m_pos = pos;
	m_up = up;
	m_near = nearPlane;
	m_far = farPlane;
	m_fov = fov;
	m_width = width;
	m_height = height;
	m_right = glm::normalize(glm::cross(m_dir,m_up));
	m_up = glm::normalize(glm::cross(m_right,m_dir));
	m_ProjMat = glm::perspectiveFov(fov,width,height,nearPlane,farPlane);
}

CLED_Camera::~CLED_Camera()
{

};

void CLED_Camera::updateView()
{
	m_viewMat = glm::lookAt(m_pos,m_pos + m_dir,m_up);
}

const glm::mat4x4& CLED_Camera::getViewMatrix() const
{
	return m_viewMat;
}

const glm::mat4x4& CLED_Camera::getProjectionMatrix() const
{
	return m_ProjMat;
}

void CLED_Camera::pitch(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat,angle,m_right);
	//m_viewMat = rotMat * m_viewMat;

	m_dir = glm::normalize( glm::vec3(rotMat * glm::vec4(m_dir,1.0f)));
	m_up = glm::normalize(glm::cross(m_right,m_dir));

	//m_viewMat = glm::lookAt(m_pos,m_pos + m_dir,m_up);
	updateView();
}
void CLED_Camera::yaw(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat,angle,m_up);
	//m_viewMat = rotMat * m_viewMat;
	
	m_dir = glm::normalize( glm::vec3(rotMat * glm::vec4(m_dir,1.0f)));
	m_right = glm::normalize(glm::cross(m_dir,m_up));

	//m_viewMat = glm::lookAt(m_pos,m_pos + m_dir,m_up);
	updateView();
}

void CLED_Camera::roll(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat,angle,m_dir);
	//m_viewMat = rotMat * m_viewMat;
	
	m_up = glm::normalize( glm::vec3(rotMat * glm::vec4(m_up,1.0f)));
	m_right = glm::normalize(glm::cross(m_dir,m_up));

	//m_viewMat = glm::lookAt(m_pos,m_pos + m_dir,m_up);
	updateView();
}

void CLED_Camera::stepRightLeft(float step)
{
	m_pos += m_right * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat,glm::vec3(step,0,0));
	//m_viewMat = transMat * m_viewMat;
	updateView();
}

void CLED_Camera::stepUpDown(float step)
{
	m_pos += m_up * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat,glm::vec3(0,step,0));
	//m_viewMat = transMat * m_viewMat;
	updateView();
}

void CLED_Camera::stepForwardBack(float step)
{
	m_pos += m_dir * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat,glm::vec3(0,0,step));
	//m_viewMat = transMat * m_viewMat;
	updateView();
}

const glm::vec3& CLED_Camera::getPosition() const
{
	return m_pos;
}

void CLED_Camera::setPerspective(float width,float height)
{
	m_width = width;
	m_height = height;
	m_ProjMat = glm::perspectiveFov(m_fov,width,height,m_near,m_far);
}