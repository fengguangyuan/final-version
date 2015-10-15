#include "LED_RenderToTexture.h"
#include "LED_Log.h"
#include "LED_MainApplication.h"
CLED_RenderToTexture::CLED_RenderToTexture(unsigned int width,unsigned int height):CLED_Texture(GL_RGB)
{
	//m_data = new char[width * height * 4 * 4];

	m_size.Set(width,height);

	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_LUMINANCE, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &m_frameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);

	glGenRenderbuffersEXT(1, &m_depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texID, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBuffer);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(Status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		char buf [100];
		sprintf_s(buf,"Error : Create RenderTexture Failed with Status : 0x%x\n",Status);
		CLED_LOG::LogWrite(wxString(buf));
	}

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT,0);
	glBindTexture(GL_TEXTURE_2D,0);
}

CLED_RenderToTexture::~CLED_RenderToTexture()
{
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
	if(m_depthBuffer != -1)
		glDeleteRenderbuffers(1,&m_depthBuffer);
	if(m_frameBuffer != -1)
		glDeleteFramebuffers(1,&m_frameBuffer);
	if(m_texID != -1)
	{
		glDeleteTextures(1,&m_texID);
	}
}

GLuint CLED_RenderToTexture::getFrameBuffer()
{
	return m_frameBuffer;
}

void CLED_RenderToTexture::drawBegin()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);
	glClearColor(0.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CLED_RenderToTexture::drawEnd()
{
	//glReadPixels(0, 0, m_size.x, m_size.y, GL_RGBA32F, GL_FLOAT, m_data);
	glBindTexture(GL_TEXTURE_2D,0);
// 	glGenerateMipmap(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}