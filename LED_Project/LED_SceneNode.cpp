#include "LED_SceneNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

CLED_SceneNode::CLED_SceneNode():CLED_Object(),m_pos(0,0,0),m_right(1.0f,0,0),m_up(0,1.0f,0),m_dir(0,0,1.0f)
{
	m_parent = 0;
	m_transMat = glm::mat4x4(1.0);
}

CLED_SceneNode::~CLED_SceneNode()
{

}

const glm::mat4x4& CLED_SceneNode::getTransformation()//得到的结果最终是相对于世界原点的
{
	return m_transMat;
}

//参数 abs 指明是否使用相对变换（其值为true，则相对父节点进行变换，值为false相对于世界原点进行变换，默认值为true）
void CLED_SceneNode::setTransformation(const glm::mat4x4& transMat,bool abs)
{
	if(abs)
	{
		m_transMat = transMat;
	}
	else
	{
		m_transMat = m_parent->getTransformation() * transMat;
	}
}

void CLED_SceneNode::Draw(const CLED_Camera& camera,int shaderIndex)
{
	for (unsigned int i = 0 ; i < m_children.size(); ++i)
	{
		m_children[i]->Draw(camera,shaderIndex);
	}
}

void CLED_SceneNode::addChild(const CLED_SceneNode* node)
{
	m_children.push_back(const_cast<CLED_SceneNode*>(node));
}

void CLED_SceneNode::setParent(const CLED_SceneNode* node)
{
	m_parent = const_cast<CLED_SceneNode*>(node);
}

const CLED_SceneNode* CLED_SceneNode::getParent()
{
	return m_parent;
}

void CLED_SceneNode::deleteChild(const CLED_SceneNode* node)
{
	std::vector<CLED_SceneNode*>::iterator it = std::find(m_children.begin(),m_children.end(),node);
	m_children.erase(it);
}

void CLED_SceneNode::forwardBack(float step)
{
	m_pos += m_dir * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat,glm::vec3(0,step,0));
	m_transMat = transMat * m_transMat;
}

void CLED_SceneNode::upDown(float step)
{
	m_pos += m_up * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat, glm::vec3(0, 0, step));
	m_transMat = transMat * m_transMat;
}

void CLED_SceneNode::rightLeft(float step)
{
	m_pos += m_right * step;
	glm::mat4x4 transMat(1.0f);
	transMat = glm::translate(transMat, glm::vec3(step, 0, 0));
	m_transMat = transMat * m_transMat;
}

void CLED_SceneNode::roll(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat,angle,m_dir);
	m_transMat = rotMat * m_transMat;
	m_up = glm::normalize( glm::vec3(rotMat * glm::vec4(m_up,1.0f)));
	m_right = glm::normalize(glm::cross(m_dir,m_up));
}

void CLED_SceneNode::yaw(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat,angle,m_up);
	m_transMat = rotMat * m_transMat;
	m_right = glm::normalize( glm::vec3(rotMat * glm::vec4(m_right,1.0f)));
	m_dir = glm::normalize(glm::cross(m_right,m_up));
}

void CLED_SceneNode::pitch(float angle)
{
	glm::mat4x4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat, angle, m_right);
	m_transMat = rotMat * m_transMat;
	m_up = glm::normalize(glm::vec3(rotMat * glm::vec4(m_up, 1.0f)));
	m_dir = glm::normalize(glm::cross(m_right, m_up));
}

void CLED_SceneNode::scale(float times)
{
	glm::mat4x4 scalMat(1.0);
	scalMat = glm::scale(times, times, times);
	m_transMat = m_transMat*scalMat;
}