#pragma once

#include <GL/glew.h>
#include "LED_Texture.h"

class CLED_DSRender
{
public:
	CLED_DSRender();
	~CLED_DSRender();

	void reBuild(int x,int y);

	void begin();
	void end();

	
	GLuint m_fbo;
	GLuint m_depthBuffer;

	GLuint m_diffuseRT;
	GLuint m_positionRT;
	GLuint m_normalRT;

	GLuint m_diffuseTex;
	GLuint m_positionTex;
	GLuint m_normalTex;

	int m_width;
	int m_height;
};