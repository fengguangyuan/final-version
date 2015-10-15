#pragma once

#include "LED_Object.h"
#include <GL/glew.h>

class CLED_Texture : public CLED_Object
{
protected:
	char* m_data;
	wxSize m_size;
	GLuint		m_texID;
public:
	unsigned int m_texDataFMT;	//数据的格式，创建纹理的时候有用

	GLuint getTextureID()
	{
		return m_texID;
	}

	CLED_Texture(unsigned int dataFMT):CLED_Object(),m_texDataFMT(dataFMT),m_data(0)
	{

	}

	virtual ~CLED_Texture()
	{
		if(m_data)
			delete [] m_data;
		if(m_texID != -1)
			glDeleteTextures(1,&m_texID);
	}
public:
	const void* getTextureData()
	{
		return m_data;
	}

	const wxSize& getTextureSize()
	{
		return m_size;
	}
};