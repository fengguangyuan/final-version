#pragma once

#include <GL/glew.h>
#include <wx/wx.h>
#include "LED_Object.h"

class CLED_ShaderFragment : public CLED_Object
{
public:
	CLED_ShaderFragment(const wxString& fileName,GLuint type);
	~CLED_ShaderFragment();
	GLuint getShaderFragment();
	GLuint getShaderFragmentType();
private:
	GLuint		m_fragment;
	GLuint		m_type;
public:
	enum
	{
		SHADER_VS = GL_VERTEX_SHADER,
		SHADER_PS = GL_FRAGMENT_SHADER,
	};
};
