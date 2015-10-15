#pragma once
#include <GL/wglew.h>
#include <GL/glew.h>
#include "LED_Texture.h"


class CLED_RenderToTexture : public CLED_Texture
{
public:
	CLED_RenderToTexture(unsigned int width,unsigned int height);
	virtual ~CLED_RenderToTexture();

	void drawBegin();
	void drawEnd();

	GLuint getFrameBuffer();
protected:
	GLuint m_frameBuffer;
	GLuint m_depthBuffer;

};