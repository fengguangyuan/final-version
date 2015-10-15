#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "LED_Object.h"
#include "LED_SceneNode.h"
#include "LED_Material.h"
#include "LED_Light.h"
#include "LED_ObjectModel.h"
#include "LED_ShaderProgram.h"

//����RenderObject�еĶ������Ǵ�Cache��ȡ���ģ���˴���ʱʹ��ObjectCache��QueryObject��ɾ����ʱ��ʹ��ObjectCache��DeleteObject
//���ǵ����ܻ�ʹ�ö���������ʹ��vector
//����shaderͬ��

struct MyVertex
{
	float x, y, z;        //Vertex
	float s0, t0;         //Texcoord0
	float nx, ny, nz;     //Normal
};

class CLED_RenderObject : public CLED_SceneNode
{
protected:
	std::vector<wxString>	m_texNames;
	std::vector<wxString>	m_shaderNames;
	std::vector<wxString>	m_lightNames;
	std::vector<wxString>	m_materialNames;

	std::vector<GLuint>						m_texIDs;
	std::vector<CLED_ShaderProgram*>		m_shaders;//Ĭ��ʹ�õ�һ��shader�����Ը�����Ҫ�޸�
	std::vector<CLED_Material*>				m_materials;
	std::vector<CLED_Light*>				m_lights;
public:
	wxString	 m_modelName;
	GLuint		 m_vbuffer;
	GLuint		 m_uvBuffer;
	GLuint		 m_norBuffer;
	CLED_ObjectModel* m_modelPtr;
	unsigned int m_state;				//��ǰ״̬�����类ѡ�л�������
	bool m_bIsLight;

public:
	CLED_RenderObject();
	virtual ~CLED_RenderObject();

	void setRenderState(unsigned int state);
	void setMaterials(const std::vector<wxString>& materials);
	void setLights(const std::vector<wxString>& lights);
	void setShaders(const std::vector<wxString>& shaders);
	void setTextures(const std::vector<wxString>& textures);
	void setModel(const wxString& model);
	void setSelected(BOOL bSelected);
	void setAsLight();
	bool asLight();

	virtual void Draw(const CLED_Camera& camera,int shaderIndex);
};