#include "LED_MainApplication.h"
#include "LED_Log.h"
#include "LED_ObjectModel_EXT.h"
#include "LED_RenderObject.h"
#include "LED_Camera.h"
#include "LED_Material.h"
#include "LED_BMPTexture.h"
#include "LED_RenderToTexture.h"
#include "LED_header.h"
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "LED_3DSFile.h"
#include "updateEvent.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

CLED_MainApplication::CLED_MainApplication()
{
	m_mainFrame = NULL;
	m_models = NULL;
	m_sounds = NULL;
	m_textures = NULL;
}

CLED_MainApplication::~CLED_MainApplication()
{
	if(m_models)
		delete m_models;
	if(m_textures)
		delete m_textures;
	if(m_sounds)
		delete m_sounds;
	if(m_shaders)
		delete m_shaders;
	if(m_lights)
		delete m_lights;
	if(m_materials)
		delete m_materials;
	if(m_cameras)
		delete m_cameras;
	if(m_scene)
		delete m_scene;
}

bool CLED_MainApplication::OnInit()
{
	CLED_LOG::LogWrite(wxString("Initialize LED Designer...\n"));

	if(!wxApp::OnInit())
	{
		CLED_LOG::LogWrite(wxString("Initialize LED Designer Failed\n"));
		return false;
	}
	m_models = new CLED_ObjectCache(wxString("Models"));
	m_textures = new CLED_ObjectCache(wxString("Textures"));
	m_sounds = new CLED_ObjectCache(wxString("Sounds"));
	m_shaders = new CLED_ObjectCache(wxString("Shaders"));
	m_materials = new CLED_ObjectCache(wxString("Materials"));
	m_lights = new CLED_ObjectCache(wxString("Lights"));
	m_cameras = new CLED_ObjectCache(wxString("Cameras"));
	m_scene = new CLED_SceneManager();

	m_mainFrame = new CLED_MainFrame(NULL,MAIN_FRAME,wxT("LED室内灯光设计"),wxPoint(100,100),wxSize(800,600),wxDEFAULT_FRAME_STYLE);
	m_mainFrame->m_glCanvas->SetCurrent();
	glewInit();
	

	CLED_LOG::LogWrite(wxString("Initialize LED Designer Successfully\n"));

	//在这里进行资源的加载
	wxString resourcePath("ResData\\");//指定资源文件所在的位置，相对路径

	//加载模型资源
	CLED_ObjectModel* study_floor = new CLED_ObjectModel(resourcePath+wxString("floor.obj"));

	m_models->AddObject(wxString("FLOOR"), study_floor);

	CLED_ObjectModel* study_sofa = new CLED_ObjectModel(resourcePath + wxString("sofa.obj"));
	m_models->AddObject(study_sofa->objName, study_sofa);

	CLED_ObjectModel* study_desk = new CLED_ObjectModel(resourcePath + wxString("desk.obj"));
	m_models->AddObject(study_desk->objName, study_desk);

	CLED_ObjectModel* study_cube = new CLED_ObjectModel(resourcePath + wxString("light.obj"));
	m_models->AddObject(wxString("LIGHT"), study_cube);

	CLED_ObjectModel* study_wall = new CLED_ObjectModel(resourcePath + wxString("wall01.obj"));
	m_models->AddObject(wxString("WALL01"), study_desk);

	study_wall = new CLED_ObjectModel(resourcePath + wxString("wall02.obj"));
	m_models->AddObject(wxString("WALL02"), study_desk);

	study_wall = new CLED_ObjectModel(resourcePath + wxString("wall03.obj"));
	m_models->AddObject(wxString("WALL03"), study_desk);

	study_wall = new CLED_ObjectModel(resourcePath + wxString("wall04.obj"));
	m_models->AddObject(wxString("WALL04"), study_desk);

	//设置相机
	CLED_Camera* mainCamera = new CLED_Camera(glm::vec3(-2,2,5.0f),glm::vec3(0,0,0),glm::vec3(0,1.0f,0),60.0f,
		800,600,0.1f,300.0f);
	m_cameras->AddObject(LED_MAIN_CAMERA,mainCamera);

	//加载纹理资源
	CLED_BMPTexture* texTex = new CLED_BMPTexture(resourcePath+wxString("walls.bmp"));
	m_textures->AddObject("ROOMTexture",texTex);

	texTex = new CLED_BMPTexture(resourcePath+wxString("white.bmp"));
	m_textures->AddObject("WHITETexture",texTex);	

	//加载material
	CLED_Material* material = new CLED_Material();
	material->m_emissive = glm::vec3(1.0,1.0,1.0);
	m_materials->AddObject("TestMaterial",material);

	//加载Shader文件
		//正常绘制的shader
		CLED_ShaderFragment* simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("SimpleTransform.vertexshader"),
		CLED_ShaderFragment::SHADER_VS);
		CLED_ShaderFragment* simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("SimpleColor.fragmentshader"),
		CLED_ShaderFragment::SHADER_PS);
		std::vector<wxString> simpleSample;
		simpleSample.push_back(wxString("SimpleTransVS"));
		simpleSample.push_back(wxString("SimpleColorPS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		CLED_ShaderProgram* simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject("simpleSample",simpleShader);

		//绘制灯的shader
		simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("SimpleTransform.vertexshader"),
			CLED_ShaderFragment::SHADER_VS);
		simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("WhiteColor.fragmentshader"),
			CLED_ShaderFragment::SHADER_PS);

		simpleSample.push_back(wxString("SimpleTransVS"));
		simpleSample.push_back(wxString("SimpleColorPS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject("LightSample",simpleShader);

		//只绘制位置的shader
		simpleSample.clear();
		simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("renderPosition.vs"),
			CLED_ShaderFragment::SHADER_VS);
		simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("renderPosition.ps"),
			CLED_ShaderFragment::SHADER_PS);
		simpleSample.push_back(wxString("RenderPositionVS"));
		simpleSample.push_back(wxString("RenderPositionPS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject(SHADER_RENDER_POSITION,simpleShader);

		//只绘制法线的shader
		simpleSample.clear();
		simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("renderNormal.vs"),
			CLED_ShaderFragment::SHADER_VS);
		simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("renderNormal.ps"),
			CLED_ShaderFragment::SHADER_PS);
		simpleSample.push_back(wxString("RenderNormalVS"));
		simpleSample.push_back(wxString("RenderNormalPS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject(SHADER_RENDER_NORMAL,simpleShader);

		//只绘制基本像素的shader
		simpleSample.clear();
		simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("renderTexture.vs"),
			CLED_ShaderFragment::SHADER_VS);
		simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("renderTexture.ps"),
			CLED_ShaderFragment::SHADER_PS);
		simpleSample.push_back(wxString("RenderTextureVS"));
		simpleSample.push_back(wxString("RenderTexturePS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject(SHADER_RENDER_TEXTURE,simpleShader);

		//延迟渲染的shader
		simpleSample.clear();
		simpleTransShader = new CLED_ShaderFragment(resourcePath + wxString("dsRendering.vs"),
			CLED_ShaderFragment::SHADER_VS);
		simpleColorShader = new CLED_ShaderFragment(resourcePath + wxString("dsRendering.ps"),
			CLED_ShaderFragment::SHADER_PS);
		simpleSample.push_back(wxString("RenderDSVS"));
		simpleSample.push_back(wxString("RenderDSPS"));
		m_shaders->AddObject(simpleSample[0],simpleTransShader);
		m_shaders->AddObject(simpleSample[1],simpleColorShader);
		simpleShader = new CLED_ShaderProgram(simpleSample);
		m_shaders->AddObject(SHADER_RENDER_DS,simpleShader);
		


	//向场景中加入需要绘制的节点
		CLED_RenderObject* simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("FLOOR"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("simpleSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("ROOMTexture"));
		simpleNode->setTextures(simpleSample);

		m_scene->addNode(wxString("FLOOR"),simpleNode);//把节点加入到场景树中


		//桌子
		/*CLED_RenderObject*  */simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(study_desk->objName);
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("simpleSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);

		m_scene->addNode(wxString("DESK"),simpleNode);//把节点加入到场景树中

		//沙发
		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(study_sofa->objName);
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("simpleSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);

		m_scene->addNode(wxString("SOFA"),simpleNode);//把节点加入到场景树中


		//加载内置的十个灯
		//红灯
		CLED_Light* light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(5.0f,7.0f,6.0f);
		light->m_diffuse = RED;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(RED_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		glm::mat4x4 mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("red_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//绿灯
		light = new CLED_Light;
		light->m_diffusePower = 15.0f;
		light->m_position = glm::vec3(-3.0f,3.0f,3.0f);
		light->m_diffuse = GREEN;
		light->m_fallOff = glm::vec3(1.0f,1.0f,1.0f);
		//m_lights->AddObject(GREEN_LIGHT,light);
		m_lights->AddObject("Cylinder02", light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);

		m_scene->addNode(wxString("green_light"),simpleNode);//把节点加入到场景树中
		
		///***************************************************************************************************************************/
		//CLoad3DS *lightModel = new CLoad3DS();
		//lightModel->ImportModel(1, (resourcePath + "lightModel01.3ds").char_str());
		//CLED_ObjectModel_EXT *new_models = lightModel->transStdModel();

		//new_models->models.at(0)->setLocation(glm::vec4(light->m_position,1.0f));
		//m_models->AddObject(new_models->models.at(0)->objName, new_models->models.at(0));
		//simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		////设置该节点用到的资源
		//simpleNode->setModel(new_models->models.at(0)->objName);
		////加入Shader的时候应注意顺序，参考LED_header中的列表
		//simpleSample.clear();
		//simpleSample.push_back(wxString("simpleSample"));
		//simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		//simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		//simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		//simpleSample.push_back(wxString(SHADER_RENDER_DS));
		//simpleNode->setShaders(simpleSample);

		//simpleSample.clear();
		//simpleSample.push_back(wxString("TestMaterial"));
		//simpleNode->setMaterials(simpleSample);

		//simpleSample.clear();
		//simpleSample.push_back(wxString("WHITETexture"));
		//simpleNode->setTextures(simpleSample);

		//mat = glm::translate(simpleNode->getTransformation(), light->m_position);
		//glm::mat4x4 scale(1.0);
		//scale = glm::scale(0.01, 0.01, 0.01);
		//mat = mat * scale;
		//simpleNode->setTransformation(mat, true);
		//wxGetApp().SceneManager()->addNode(new_models->models.at(0)->objName, simpleNode);//把节点加入到场景树中
		//updateTreeCtrl(new_models->models.at(0)->objName);
		///***************************************************************************************************************************/
		//light->m_lightOn = false;
		//蓝灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(15.0f,5.0f,1.0f);
		light->m_diffuse = BLUE;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(BLUE_LIGHT,light);
		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("yellow_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//白灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(16.0f,3.0f,-14.0f);
		light->m_diffuse = WHITE;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(WHITE_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("white_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//青色灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(5.0f,5.0f,-3.0f);
		light->m_diffuse = CYAN;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(CYAN_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("cyan_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//红褐色灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(-5.0f,15.0f,15.0f);
		light->m_diffuse = MAROON;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(MAROON_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("maroon_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//橄榄绿灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(-10.0f,7.0f,-1.0f);
		light->m_diffuse = OLIVE;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(OLIVE_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("olive_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//紫色灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(-8.0f,8.0f,-7.0f);
		light->m_diffuse = PURPLE;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(PURPLE_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("purple_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//金黄色灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(-13.0f,12.0f,-11.0f);
		light->m_diffuse = GOLD;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(GOLD_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("gold_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;
		//惨白色灯
		light = new CLED_Light;
		light->m_diffusePower = 8.0f;
		light->m_position = glm::vec3(5.0f,14.0f,-11.0f);
		light->m_diffuse = GHOSTWHITE;
		light->m_fallOff = glm::vec3(0.3f,0.3f,0.3f);
		m_lights->AddObject(GHOSTWHITE_LIGHT,light);

		simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
		//设置该节点用到的资源
		simpleNode->setModel(wxString("LIGHT"));
		//加入Shader的时候应注意顺序，参考LED_header中的列表
		simpleSample.clear();
		simpleSample.push_back(wxString("LightSample"));
		simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
		simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
		simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
		simpleSample.push_back(wxString(SHADER_RENDER_DS));
		simpleNode->setShaders(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("TestMaterial"));
		simpleNode->setMaterials(simpleSample);

		simpleSample.clear();
		simpleSample.push_back(wxString("WHITETexture"));
		simpleNode->setTextures(simpleSample);
		mat = glm::translate(simpleNode->getTransformation(),light->m_position);
		simpleNode->setTransformation(mat,true);
		m_scene->addNode(wxString("ghostwhite_light"),simpleNode);//把节点加入到场景树中
		light->m_lightOn = false;


	return true;
}

int CLED_MainApplication::OnExit()
{

	CLED_LOG::LogWrite(wxString("LED Designer Exit\n"));
	return wxApp::OnExit();
}

const COpenGLEnv& CLED_MainApplication::getOpenGLEnv()
{
	if(m_mainFrame)
		return m_mainFrame->getOpenGLEnv();
}

int CLED_MainApplication::FilterEvent(wxEvent& event)
{
	if(event.GetEventType() == wxEVT_CHAR)
	{
		wxKeyEvent& keyEvt = (wxKeyEvent&)(event);
		m_mainFrame->OnKeyDown(keyEvt);
		event.Skip();
		return 1;
	}
	return wxApp::FilterEvent(event);
}

CLED_ObjectCache& CLED_MainApplication::ModelCache()
{
	return *m_models;
}

CLED_ObjectCache& CLED_MainApplication::TextureCache()
{
	return *m_textures;
}

CLED_ObjectCache& CLED_MainApplication::SoundCache()
{
	return *m_sounds;
}

CLED_ObjectCache& CLED_MainApplication::ShaderCache()
{
	return *m_shaders;
}

CLED_ObjectCache& CLED_MainApplication::MaterialCache()
{
	return *m_materials;
}

CLED_ObjectCache& CLED_MainApplication::LightCache()
{
	return *m_lights;
}


CLED_ObjectCache& CLED_MainApplication::CamerasCache()
{
	return *m_cameras;
}

CLED_SceneManager* CLED_MainApplication::SceneManager()
{
	return m_scene;
}