#pragma once
#include "LED_Object.h"
#include "LED_header.h"
#include "LED_Camera.h"
#include <glm/glm.hpp>
#include <vector>

class CLED_SceneNode : public CLED_Object
{
protected:
	glm::mat4x4 m_transMat;
	CLED_SceneNode* m_parent;
	std::vector<CLED_SceneNode*> m_children;

	glm::vec3	m_dir;
	glm::vec3	m_up;
	glm::vec3	m_right;
	glm::vec3	m_pos;

public:
	CLED_SceneNode();
	virtual ~CLED_SceneNode();

	const glm::mat4x4& getTransformation();//得到的结果最终是相对于世界原点的
	//参数 abs 指明是否使用相对变换（其值为true，则相对父节点进行变换，值为false相对于世界原点进行变换，默认值为true）
	void setTransformation(const glm::mat4x4& transMat,bool abs);

	void addChild(const CLED_SceneNode* node);
	void deleteChild(const CLED_SceneNode* node);
	const CLED_SceneNode* getParent();
	void setParent(const CLED_SceneNode* node);

	void forwardBack(float step);
	void rightLeft(float step);
	void upDown(float step);

	void roll(float angle);
	void yaw(float angle);
	void pitch(float angle);

	void scale(float times);


	virtual void Draw(const CLED_Camera& camera,int shaderIndex);
};