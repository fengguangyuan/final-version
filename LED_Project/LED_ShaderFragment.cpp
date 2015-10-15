#include "LED_ShaderFragment.h"
#include "LED_Log.h"
CLED_ShaderFragment::CLED_ShaderFragment(const wxString& fileName,GLuint type):CLED_Object()
{
	FILE* fp = fopen(fileName.ToStdString().c_str(),"rb");
	if(!fp)
		CLED_LOG::LogWrite(wxString("Error : Open Shader File '") + fileName + wxString("Failed\n"));
	fseek(fp, 0, SEEK_END);
	unsigned int uiEnd = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned int uiStart = ftell(fp);
	unsigned int uiFileSize = uiEnd - uiStart;

	char* buffer = new char[uiFileSize+1];
	buffer[uiFileSize] = '\0';
	if(!buffer)
		CLED_LOG::LogWrite(wxString("Error : Open Shader File '") + fileName + wxString("Failed -> Not Enougth Memory!n"));
	if( fread(buffer,1,uiFileSize,fp) == 0 )
		CLED_LOG::LogWrite(wxString("Error : Read Shader File '") + fileName + wxString("Failed\n"));

	m_type = type;
	m_fragment = glCreateShader(type);

	const GLchar* shader_src = buffer;
	glShaderSource(m_fragment,1,&shader_src,NULL);
	glCompileShader(m_fragment);
	delete [] buffer;

	int error_len = 0;
	glGetShaderiv(m_fragment,GL_INFO_LOG_LENGTH,&error_len);
	if(error_len > 1)
	{
		char* infoLog = (char *)malloc(error_len);
		glGetShaderInfoLog(m_fragment, error_len, &error_len, infoLog);
		wxString str(wxString("ERROR : Shader Complier Error Log,File '")+ fileName + wxString("' ---> \n"));
		str += wxString(infoLog);
		str += wxString("\n");
		CLED_LOG::LogWrite(str);
		free(infoLog);
	}
	fclose(fp);
}

CLED_ShaderFragment::~CLED_ShaderFragment()
{
	if(m_fragment != -1)
		glDeleteShader(m_fragment);
}

GLuint CLED_ShaderFragment::getShaderFragment()
{
	return m_fragment;
}

GLuint CLED_ShaderFragment::getShaderFragmentType()
{
	return m_type;
}