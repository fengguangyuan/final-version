#include <GL/glew.h>
#include <glm/glm.hpp>
#include "LED_OpenGLCanvas.h"
#include "LED_Log.h"
#include "LED_MainApplication.h"
#include "LED_Camera.h"
#include "LED_RenderToTexture.h"
#include "LED_BMPTexture.h"
#include "LED_header.h"
#include "teapot.h"

CLED_OpenGLCanvas::CLED_OpenGLCanvas(wxWindow* parent,int id,const wxPoint& pos,const wxSize& size):
	wxGLCanvas(parent,id,pos,size,wxFULL_REPAINT_ON_RESIZE,wxT("LED_Canvas")),m_canvasSizeChanged(true)
{
	m_glSizeEvt = 0;
	m_glPaintEvt = 0;

	memset(texRTT,-1,sizeof(GLuint)*3);
	memset(fboRTT,-1,sizeof(GLuint)*3);
	memset(rboRTT,-1,sizeof(GLuint)*3);

	SetCurrent();

	glewExperimental = true;
	if(glewInit() != GLEW_OK) 
	{
		CLED_LOG::LogWrite(wxString("ERROR ---> GLEW Init Failed!\n"));
	}
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	m_mainCamera = NULL;

	static const GLfloat quad_buffer[] = { 
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
		-1.0f ,1.0f, 0.0f,
	};

	static const GLfloat quad_uv_buffer[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	glGenBuffers(1,&m_quadVB);
	glGenBuffers(1,&m_quadUVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_buffer), quad_buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadUVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_uv_buffer), quad_uv_buffer, GL_STATIC_DRAW);

// 	m_NorTex = 0;
// 	m_PosTex = 0;
// 	m_TexTex = 0;

	//绘制Quad的shader
	CLED_ShaderFragment* simpleTransShader = new CLED_ShaderFragment(wxString("ResData\\renderQuad.vs"),
		CLED_ShaderFragment::SHADER_VS);
	CLED_ShaderFragment* simpleColorShader = new CLED_ShaderFragment(wxString("ResData\\renderQuad.ps"),
		CLED_ShaderFragment::SHADER_PS);
	std::vector<wxString> simpleSample;
	simpleSample.push_back(wxString("RenderQuadVS"));
	simpleSample.push_back(wxString("RenderQuadPS"));
	wxGetApp().ShaderCache().AddObject(simpleSample[0],simpleTransShader);
	wxGetApp().ShaderCache().AddObject(simpleSample[1],simpleColorShader);
	m_DSShader = new CLED_ShaderProgram(simpleSample);
	m_DSShader->linkShaderProgram();

	//计算光照的shader
	simpleTransShader = new CLED_ShaderFragment(wxString("ResData\\renderQuad.vs"),
		CLED_ShaderFragment::SHADER_VS);
	simpleColorShader = new CLED_ShaderFragment(wxString("ResData\\renderLight.ps"),
		CLED_ShaderFragment::SHADER_PS);
	simpleSample.clear();
	simpleSample.push_back(wxString("RenderLightVS"));
	simpleSample.push_back(wxString("RenderLightPS"));
	wxGetApp().ShaderCache().AddObject(simpleSample[0],simpleTransShader);
	wxGetApp().ShaderCache().AddObject(simpleSample[1],simpleColorShader);
	m_LightCalSahder = new CLED_ShaderProgram(simpleSample);
	m_LightCalSahder->linkShaderProgram();
}

CLED_OpenGLCanvas::~CLED_OpenGLCanvas()
{
	wxGetApp().CamerasCache().DeleteObject(LED_MAIN_CAMERA);
	glDeleteBuffers(1,&m_quadVB);
	glDeleteBuffers(1,&m_quadUVB);
}


const COpenGLEnv& CLED_OpenGLCanvas::getOpenGLEnv()
{
	return m_glEnv;
}



void CLED_OpenGLCanvas::OnSize(wxSizeEvent& event)
{
	if(!IsShownOnScreen())
		return;
	SetCurrent();
	wxGLCanvas::OnSize(event);
	m_canvasSizeChanged = true;
	
	SetSize(event.GetSize());

	m_glEnv.m_aspect = (float)event.GetSize().x / (float)event.GetSize().y;
	m_glEnv.m_size = event.GetSize();

	glDeleteTextures(4, texRTT);
}

void CLED_OpenGLCanvas::rebuildDS()
{
}

void CLED_OpenGLCanvas::calculateLighting()
{
	m_LightCalSahder->useShaderProgram();

	GLuint texID = glGetUniformLocation(m_LightCalSahder->getShaderProgram(),"TextureSampler");
	GLuint lightID = glGetUniformLocation(m_LightCalSahder->getShaderProgram(),"LightSampler");
	//	CLED_BMPTexture* t = dynamic_cast<CLED_BMPTexture*>(wxGetApp().TextureCache().QueryObject("TestTexture"));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texRTT[TEX_RTT]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,texRTT[LIGHT_RTT]);

	//set sampler to user Texture Units
	glUniform1i(texID,0);
	glUniform1i(lightID,1);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadUVB);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the quad !
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	wxGetApp().ShaderCache().DeleteObject(SHADER_RENDER_FINAL_QUAD);
}

void CLED_OpenGLCanvas::renderQuad(CLED_Light* light)
{
	m_DSShader->useShaderProgram();

	GLuint texID = glGetUniformLocation(m_DSShader->getShaderProgram(),"TextureSampler");
	GLuint posID = glGetUniformLocation(m_DSShader->getShaderProgram(),"PositionSampler");
	GLuint norID = glGetUniformLocation(m_DSShader->getShaderProgram(),"NormalSampler");
//	CLED_BMPTexture* t = dynamic_cast<CLED_BMPTexture*>(wxGetApp().TextureCache().QueryObject("TestTexture"));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texRTT[TEX_RTT]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,texRTT[POS_RTT]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,texRTT[NOR_RTT]);

	//set sampler to user Texture Units
	glUniform1i(texID,0);
	glUniform1i(posID,1);
	glUniform1i(norID,2);

	//设置灯光
	if(light)
	{
		GLuint ambientColor = glGetUniformLocation(m_DSShader->getShaderProgram(),"uAmbientColor");
		GLuint lightDiffuseColor = glGetUniformLocation(m_DSShader->getShaderProgram(),"ulightDiffuseColor");
		GLuint lightWorldPos = glGetUniformLocation(m_DSShader->getShaderProgram(),"ulightWorldPos");
		GLuint lightFallOff = glGetUniformLocation(m_DSShader->getShaderProgram(),"ulightFallOff");
		GLuint lightDiffusePower = glGetUniformLocation(m_DSShader->getShaderProgram(),"ulightDiffusePower");

		glUniform3f(ambientColor,0.3,0.3,0.3);//默认全局环境光照强度为0.3
		glUniform3f(lightDiffuseColor,light->m_diffuse.x,light->m_diffuse.y,light->m_diffuse.z);
		glUniform3f(lightWorldPos,light->m_position.x,light->m_position.y,light->m_position.z);
		glUniform3f(lightFallOff,light->m_fallOff.x,light->m_fallOff.y,light->m_fallOff.z);
		glUniform1f(lightDiffusePower,light->m_diffusePower);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadUVB);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the quad !
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	wxGetApp().ShaderCache().DeleteObject(SHADER_RENDER_FINAL_QUAD);
}

void CLED_OpenGLCanvas::resetRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY());
}

//创建DS使用的FBO
#define BEGIN_DRAW_FBO(fbo)	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);\
							glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);\
							glClearColor(0.0, 0.0, 0.0, 0.0);\
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#define END_DRAW_FBO()	glBindTexture(GL_TEXTURE_2D,0);\
						glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);\
						glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

#define REBIND_FBO()	for (int i = 0; i < 4; ++i) \
						{\
							glGenTextures(1,&texRTT[i]);\
							glBindTexture(GL_TEXTURE_2D, texRTT[i]);\
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);\
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);\
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);\
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);\
							if(i == 0)\
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY(), \
										 0, GL_RGB, GL_UNSIGNED_BYTE, 0);\
							else\
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY(), \
										 0, GL_RGBA, GL_FLOAT, 0);\
							glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboRTT[i]);\
							glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboRTT[i]);\
							glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY());\
							glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texRTT[i], 0);\
							glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboRTT[i]);\
							GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);\
							if(Status != GL_FRAMEBUFFER_COMPLETE_EXT)\
							{\
								char buf [100];\
								sprintf_s(buf,"DS Re-Rendering Error : Create NO: [%d] RenderTexture Failed with Status : 0x%x\n",i,Status);\
								CLED_LOG::LogWrite(wxString(buf));\
							}\
							glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);\
							glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT,0);\
							glBindTexture(GL_TEXTURE_2D,0);\
						}
							

void CLED_OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC(this);
	glEnable(GL_DEPTH_TEST);
	if(m_canvasSizeChanged)
	{
		SetCurrent();
		//设置视口
		glViewport(0, 0, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY());
		//rebuildDS();
		m_canvasSizeChanged = false;

		//删除DS渲染相关项
		glDeleteTextures(3,texRTT);
		glDeleteFramebuffersEXT(3,fboRTT);
		glDeleteRenderbuffersEXT(3,rboRTT);

		for (int i = 0; i < 4; i++)
		{
			//tex
			glGenTextures(1,&texRTT[i]);
			glBindTexture(GL_TEXTURE_2D, texRTT[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if(i == 0)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY(), 
					0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY(), 
					0, GL_RGBA, GL_FLOAT, 0);
			//fbo
			glGenFramebuffersEXT(1, &fboRTT[i]);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboRTT[i]);
			//rbo
			glGenRenderbuffersEXT(1, &rboRTT[i]);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboRTT[i]);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_glEnv.m_size.GetX(), m_glEnv.m_size.GetY());
			//attach tex rbo fbo
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texRTT[i], 0);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboRTT[i]);
			//check
			GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if(Status != GL_FRAMEBUFFER_COMPLETE_EXT)
			{
				char buf [100];
				sprintf_s(buf,"DS Rendering Error : Create NO: [%d] RenderTexture Failed with Status : 0x%x\n",i,Status);
				CLED_LOG::LogWrite(wxString(buf));
			}
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
			glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT,0);
			glBindTexture(GL_TEXTURE_2D,0);
		}
	}
	REBIND_FBO();
	std::map<wxString,CLED_Object*>& lights = wxGetApp().LightCache().m_cache;
	std::map<wxString,CLED_Object*>::iterator it = lights.begin();
	//普通绘制
	if(m_mainCamera == NULL)
		m_mainCamera = dynamic_cast<CLED_Camera*>(wxGetApp().CamerasCache().QueryObject(LED_MAIN_CAMERA));
	if(m_mainCamera)
	{
		BEGIN_DRAW_FBO(fboRTT[TEX_RTT]);
		m_mainCamera->setPerspective(m_glEnv.m_size.x,m_glEnv.m_size.y);
		wxGetApp().SceneManager()->drawScene(*m_mainCamera,RENDER_TEXTURE);
		END_DRAW_FBO();
	}
	//绘制顶点
	if(m_mainCamera)
	{
		BEGIN_DRAW_FBO(fboRTT[POS_RTT]);
		m_mainCamera->setPerspective(m_glEnv.m_size.x,m_glEnv.m_size.y);
		wxGetApp().SceneManager()->drawScene(*m_mainCamera,RENDER_POSITION);
		END_DRAW_FBO();
	}
	//绘制法线
	if(m_mainCamera)
	{
		BEGIN_DRAW_FBO(fboRTT[NOR_RTT]);
		m_mainCamera->setPerspective(m_glEnv.m_size.x,m_glEnv.m_size.y);
		wxGetApp().SceneManager()->drawScene(*m_mainCamera,RENDER_NORMAL);
		END_DRAW_FBO();
	}
//	resetRender();
// 绘制光照值
	BEGIN_DRAW_FBO(fboRTT[LIGHT_RTT]);
	it = lights.begin();
	glEnable(GL_BLEND);
	glDisable( GL_DEPTH_TEST );
	glBlendFunc(GL_ONE,GL_ONE);
	for (;it != lights.end(); ++it)
	{
		//if (it->first == WHITE_LIGHT || it->first == GREEN_LIGHT)
		{
			CLED_Light* light = dynamic_cast<CLED_Light*>(it->second);
			if(light->m_lightOn)
				renderQuad(light);
		}
	}
	glDisable(GL_BLEND);
	glEnable( GL_DEPTH_TEST );
	END_DRAW_FBO();

	resetRender();
	calculateLighting();
	SwapBuffers();

	glDeleteTextures(4,texRTT);
	return;
}

BEGIN_EVENT_TABLE(CLED_OpenGLCanvas,wxGLCanvas)
	EVT_PAINT(CLED_OpenGLCanvas::OnPaint)
	EVT_SIZE(CLED_OpenGLCanvas::OnSize)
END_EVENT_TABLE()