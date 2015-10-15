#pragma once

#include <wx/wx.h>
#include "LED_header.h"

class COpenGLEnv
{
public:
	wxPoint m_mouse;	//鼠标坐在位置
	wxSize m_size;		//窗口大小
	float m_aspect;

	COpenGLEnv():m_aspect(0.0)
	{

	}
};