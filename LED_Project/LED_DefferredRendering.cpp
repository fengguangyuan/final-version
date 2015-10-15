#include "LED_DefferredRendering.h"
#include "LED_Log.h"

CLED_DSRender::CLED_DSRender()
{
	m_fbo = -1;
	m_depthBuffer = -1;

	m_diffuseRT = -1;
	m_positionRT = -1;
	m_normalRT = -1;

	m_diffuseTex = -1;
	m_positionTex = -1;
	m_normalTex = -1;

	m_width = -1;
	m_height = -1;
}

CLED_DSRender::~CLED_DSRender()
{
	glDeleteTextures(1, &m_normalTex);
	glDeleteTextures(1, &m_positionTex);
	glDeleteTextures(1, &m_diffuseTex);
	glDeleteFramebuffersEXT(1, &m_fbo);
	glDeleteRenderbuffersEXT(1, &m_diffuseRT);
	glDeleteRenderbuffersEXT(1, &m_positionRT);
	glDeleteRenderbuffersEXT(1, &m_normalRT);
	glDeleteRenderbuffersEXT(1, &m_depthBuffer);
}

void CLED_DSRender::begin()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,m_width, m_height);

	// Clear the render targets
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	// Specify what to render an start acquiring
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, buffers);
}

void CLED_DSRender::end()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

void CLED_DSRender::reBuild(int width,int height)
{
	if(m_normalTex != -1)
	{
		glDeleteTextures(1, &m_normalTex);
		m_normalTex = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteTextures(1, &m_positionTex);
		m_positionTex = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteTextures(1, &m_diffuseTex);
		m_diffuseTex = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteFramebuffersEXT(1, &m_fbo);
		m_fbo = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteRenderbuffersEXT(1, &m_diffuseRT);
		m_diffuseRT = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteRenderbuffersEXT(1, &m_positionRT);
		m_positionRT = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteRenderbuffersEXT(1, &m_normalRT);
		m_normalRT = -1;
	}
	if(m_normalTex != -1)
	{
		glDeleteRenderbuffersEXT(1, &m_depthBuffer);
		m_depthBuffer = -1;
	}

	m_width  = width;
	m_height = height;

	glGenFramebuffersEXT(1, &m_fbo);
	glGenRenderbuffersEXT(1, &m_diffuseRT);
	glGenRenderbuffersEXT(1, &m_positionRT);
	glGenRenderbuffersEXT(1, &m_normalRT);
	glGenRenderbuffersEXT(1, &m_depthBuffer);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_diffuseRT);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA32F, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_diffuseRT);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_positionRT);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA32F_ARB, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, m_positionRT);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_normalRT);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA32F, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_RENDERBUFFER_EXT, m_normalRT);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBuffer);

	glGenTextures(1, &m_diffuseTex);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &m_positionTex);
	glBindTexture(GL_TEXTURE_2D, m_positionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &m_normalTex);
	glBindTexture(GL_TEXTURE_2D, m_normalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		char buf [100];
		sprintf_s(buf,"Error : Create RenderTexture Failed with Status : 0x%x\n",status);
		CLED_LOG::LogWrite(wxString(buf));
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}