#pragma once

#include <wx/wx.h>
#include "LED_header.h"

class COpenGLEnv
{
public:
	wxPoint m_mouse;	//�������λ��
	wxSize m_size;		//���ڴ�С
	float m_aspect;

	COpenGLEnv():m_aspect(0.0)
	{

	}
};