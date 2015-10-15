#include "LED_ShaderProgram.h"
#include "LED_MainApplication.h"


CLED_ShaderProgram::CLED_ShaderProgram(const std::vector<wxString>& fragments):CLED_Object()
{
	m_program = glCreateProgram();
	m_fragments = fragments;
	for (unsigned int i = 0 ; i < fragments.size(); ++ i)
	{
		CLED_ShaderFragment* fragment = dynamic_cast<CLED_ShaderFragment*>(wxGetApp().ShaderCache().QueryObject(fragments[i]));
		glAttachShader(m_program,fragment->getShaderFragment());
		glProgramParameteriEXT(m_program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES_ADJACENCY_EXT);
		glProgramParameteriEXT(m_program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP_ADJACENCY_EXT);

	}
}

CLED_ShaderProgram::~CLED_ShaderProgram()
{
	for (unsigned int i = 0; i < m_fragments.size(); ++i)
	{
		CLED_ShaderFragment* fragment = dynamic_cast<CLED_ShaderFragment*>(wxGetApp().ShaderCache().QueryObject(m_fragments[i]));
		glDetachShader(m_program,fragment->getShaderFragment());
	}
	glDeleteProgram(m_program);
}

void CLED_ShaderProgram::linkShaderProgram()
{
	glLinkProgram(m_program);
	int error_len  = 0;
	glGetProgramiv(m_program,GL_INFO_LOG_LENGTH,&error_len);
	if(error_len > 1)
	{
		char* infoLog = (char*)malloc(error_len);
		glGetProgramInfoLog(m_program,error_len,&error_len,infoLog);
		CLED_LOG::LogWrite(wxString("ERROR : Use Shader Program Failed --> \n") + wxString(infoLog) + wxString("\n"));
		free(infoLog);
	}
}

void CLED_ShaderProgram::useShaderProgram()
{
	glUseProgram(m_program);
}

GLuint CLED_ShaderProgram::getShaderProgram()
{
	return m_program;
}