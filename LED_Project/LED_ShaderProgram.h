#pragma once

#include "LED_ShaderFragment.h"
#include <vector>
class CLED_ShaderProgram : public CLED_Object
{
public:
	CLED_ShaderProgram(const std::vector<wxString>& fragments);
	~CLED_ShaderProgram();

	GLuint getShaderProgram();
	void linkShaderProgram();
	void useShaderProgram();
private:

	std::vector<wxString> m_fragments;
	GLuint m_program;
};