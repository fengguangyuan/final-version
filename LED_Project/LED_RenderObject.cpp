#include "LED_RenderObject.h"
#include "LED_Texture.h"
#include "LED_MainApplication.h"
#include "LED_Log.h"
#include "LED_Material.h"
#include <GL/glew.h>
CLED_RenderObject::CLED_RenderObject():CLED_SceneNode(),m_state(OBJECT_UNSELECTED)
{
	m_uvBuffer = -1;
	m_vbuffer = -1;
	m_norBuffer = -1;
	m_bIsLight = false;
}

CLED_RenderObject::~CLED_RenderObject()
{
	for (unsigned int i = 0 ; i < m_lightNames.size(); ++i)
	{
		wxGetApp().LightCache().DeleteObject(m_lightNames[i]);
	}

	for (unsigned int i = 0 ; i < m_shaderNames.size(); ++i)
	{
		wxGetApp().ShaderCache().DeleteObject(m_shaderNames[i]);
	}

	for (unsigned int i = 0 ; i < m_materialNames.size(); ++i)
	{
		wxGetApp().MaterialCache().DeleteObject(m_materialNames[i]);
	}

	for (unsigned int i = 0 ; i < m_texNames.size(); ++i)
	{
		wxGetApp().TextureCache().DeleteObject(m_texNames[i]);
	}

	wxGetApp().ModelCache().DeleteObject(m_modelName);

	glDeleteTextures(m_texIDs.size(),&m_texIDs[0]);
}

void CLED_RenderObject::setRenderState(unsigned int state)
{
	m_state = state;
}

void CLED_RenderObject::setAsLight()
{
	m_bIsLight = true;
}

bool CLED_RenderObject::asLight()
{
	return m_bIsLight;
}

void CLED_RenderObject::setModel(const wxString& model)
{

	m_modelName = model;
	m_modelPtr = dynamic_cast<CLED_ObjectModel*>(wxGetApp().ModelCache().QueryObject(m_modelName));
	m_modelPtr->objName = m_modelName;
	//生成顶点缓冲
 	glGenBuffers(1, &m_vbuffer);
 	glBindBuffer(GL_ARRAY_BUFFER, m_vbuffer);
 	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*m_modelPtr->getVertices().size(),
				&m_modelPtr->getVertices()[0], GL_STATIC_DRAW);
	//生成纹理坐标缓冲
	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*m_modelPtr->getUVs().size(),
		&m_modelPtr->getUVs()[0], GL_STATIC_DRAW);
	//生成法线坐标缓冲
	glGenBuffers(1, &m_norBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_norBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*m_modelPtr->getNormals().size(),
		&m_modelPtr->getNormals()[0], GL_STATIC_DRAW);
}

void CLED_RenderObject::setMaterials(const std::vector<wxString>& materials)
{
	m_materialNames = materials;
	m_materials.clear();
	for (unsigned int i = 0 ; i < m_materialNames.size(); ++i)
	{
		CLED_Material* material = dynamic_cast<CLED_Material*>( wxGetApp().MaterialCache().QueryObject(m_materialNames[i]));
		m_materials.push_back(material);
	}
}

void CLED_RenderObject::setLights(const std::vector<wxString>& lights)
{
	m_lightNames = lights;
	m_lights.clear();
	for (unsigned int i = 0 ; i < m_lightNames.size(); ++i)
	{
		CLED_Light* light = dynamic_cast<CLED_Light*>( wxGetApp().LightCache().QueryObject(m_lightNames[i]));
		m_lights.push_back(light);
	}
}

void CLED_RenderObject::setShaders(const std::vector<wxString>& shaders)
{
	m_shaderNames = shaders;
	m_shaders.clear();
	for (unsigned int i = 0 ; i < m_shaderNames.size(); ++i)
	{
		CLED_ShaderProgram* prog = dynamic_cast<CLED_ShaderProgram*>(wxGetApp().ShaderCache().QueryObject(m_shaderNames[i]));
		m_shaders.push_back(prog);
		prog->linkShaderProgram();
	}
}

void CLED_RenderObject::setTextures(const std::vector<wxString>& textures)
{
	m_texNames = textures;
	m_texIDs.clear();
	for (unsigned int i = 0 ; i < m_texNames.size(); ++i)
	{
		CLED_Texture* tex = dynamic_cast<CLED_Texture*>(wxGetApp().TextureCache().QueryObject(m_texNames[i]));
		GLuint texID = tex->getTextureID();
		m_texIDs.push_back(texID);
	}
}

void CLED_RenderObject::setSelected(BOOL bSelected)
{
	m_state = bSelected;
}

void CLED_RenderObject::Draw(const CLED_Camera& camera,int shaderIndex)
{
	//下面代码会有很多冗余无效的部分
	static glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	m_shaders[shaderIndex]->useShaderProgram();

	

	//矩阵
 	GLuint mvpMatID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"MVP");
	GLuint ViewMatrixID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(), "V");
	GLuint ModelMatrixID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(), "M");
	GLuint MVID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"MV");
	//设置矩阵数据
	const glm::mat4x4 mvpMat = camera.getProjectionMatrix() * camera.getViewMatrix() * getTransformation();
	glUniformMatrix4fv(mvpMatID, 1, GL_FALSE, &mvpMat[0][0]);
	const glm::mat4x4 mvMat = camera.getViewMatrix() * m_transMat;
	glUniformMatrix4fv(MVID, 1, GL_FALSE, &mvMat[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &m_transMat[0][0]);

	GLuint LightID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(), "LightPosition_worldspace");//光源位置
	glm::vec3 lightPos = glm::vec3(-4,-4,-4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);//光源位置测试


	GLuint mtrlAmbientID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"mtrlAmbient");
	GLuint mtrlSpecularID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"mtrlSpecualr");
	GLuint mtrlDiffuseID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"mtrlDiffuse");
	GLuint mtrlSpecularPowerID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"mtrlSpecularPower");//材质

	GLuint lightAmbientID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"lightAmbient");
	GLuint lightSpecularID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"lightSpecualr");
	GLuint lightDiffuseID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"lightDiffuse");
	GLuint lightFallOffID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"lightFallOff");//光源

	GLuint bObjSelectedID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"isSelected");

	GLuint texID = glGetUniformLocation(m_shaders[shaderIndex]->getShaderProgram(),"TexSampler");
	//bind to texture unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D,m_texIDs[0]);
	//set sampler to user Texture Unit 0
	glUniform1i(texID,0);

	glUniform3f(mtrlAmbientID,m_materials[0]->m_ambient.x,m_materials[0]->m_ambient.y,m_materials[0]->m_ambient.z);
	glUniform3f(mtrlDiffuseID,m_materials[0]->m_diffuse.x,m_materials[0]->m_diffuse.y,m_materials[0]->m_diffuse.z);
	glUniform3f(mtrlSpecularID,m_materials[0]->m_specular.x,m_materials[0]->m_specular.y,m_materials[0]->m_specular.z);
	glUniform1f(mtrlSpecularPowerID,m_materials[0]->m_specularPower);
	glUniform1i(bObjSelectedID,m_state);

 	glEnableVertexAttribArray(0);
 	glEnableVertexAttribArray(1);
 	glEnableVertexAttribArray(2);
 
 	glBindBuffer(GL_ARRAY_BUFFER,m_vbuffer);
 	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
 
 	glBindBuffer(GL_ARRAY_BUFFER,m_uvBuffer);
 	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
 
 	glBindBuffer(GL_ARRAY_BUFFER,m_norBuffer);
 	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

 	glDrawArrays(GL_TRIANGLES,0,m_modelPtr->getVertices().size());
 
 	glEnableVertexAttribArray(0);
 	glEnableVertexAttribArray(1);
 	glEnableVertexAttribArray(2);
}