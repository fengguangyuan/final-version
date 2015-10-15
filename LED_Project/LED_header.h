#pragma once

#include "LED_Log.h"
#include <wx/app.h>

#define MAIN_SPLITTER_ID wxID_HIGHEST+1		//定义Hightest+1位splitter的标识符
#define MAIN_SPLITTER_SIZE 5
#define MAIN_SPLITTER_POS 250

#define MAIN_GL_CANVAS wxID_HIGHEST+2		//OpenGL 画布的ID
#define MAIN_GL_WINDOW wxID_HIGHEST+3		//右半边显示场景的窗口的ID
#define MAIN_CONTROL_WINDOW wxID_HIGHEST+4	//左半边窗口的ID
#define MAIN_FRAME	wxID_HIGHEST+5			//主窗口的ID
#define MAIN_TIMER_ID wxID_HIGHEST+6		//主定时器的ID

#define LED_LIGHT_SPOT	0		//聚光灯
#define LED_LIGHT_DIRECTIONAL 1	//方向光源
#define LED_LIGHT_POINT 2		//点光源

#define OBJECT_UNSELECTED 0
#define OBJECT_SELECTED 1

#define MAIN_FRAME_wxSpinCtrl 5501
#define MAIN_FRAME_light_wxSpinCtrl 5502
#define MAIN_FRAME_wxRadioBox 5500
#define MAIN_FRAME_wxtree 5600
#define MAIN_FRAME_wxlighttree 5601
#define MAIN_FRAME_checkbox 5608
#define MAIN_FRAME_movelight_button 5609
#define MAIN_LIGHTCHOOSE_button1 8100
#define MAIN_LIGHTCHOOSE_button2 8101
#define MAIN_LIGHTCHOOSE_button3 8102
#define MAIN_LIGHTCHOOSE_slider 5002
#define MAIN_LIGHTCHOOSE_spinctrl 5502
#define MAIN_MODELCHOOSE_button1 6200
#define MAIN_MODELCHOOSE_button2 6201
#define MAIN_MODELCHOOSE_button3 6202
#define MAIN_MODELCHOOSE_slider1 5200
#define MAIN_MODELCHOOSE_slider2 5201
#define MAIN_MODELCHOOSE_slider3 5202
#define MAIN_MODELCHOOSE_spinctrl 5702

//在object的绘制中用到
#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((void*)(offset))
#endif

//caches
#define LED_MODEL_CACHE wxString("Models")
#define LED_TEXTURE_CACHE wxString("Textures")
#define LED_SOUND_CACHE wxString("Sounds")
#define LED_SHADER_CACHE wxString("Shaders")
#define LED_MATERIAL_CACHE wxString("Materials")
#define LED_LIGHT_CACHE wxString("Lights")
#define LED_CAMERA_CACHE wxString("Cameras")

#define LED_MAIN_CAMERA wxString("MainCamera")
#define LED_SCENE_ROOT wxString("ROOT")

#define SHADER_RENDER_POSITION "RenderPosition"
#define SHADER_RENDER_NORMAL "RenderNormal"
#define SHADER_RENDER_DEPTH "RednerDepth"
#define SHADER_RENDER_TEXTURE "RenderTexture"
#define SHADER_RENDER_LIGHT "RednerLight"
#define SHADER_RENDER_FINAL_QUAD "Render_Quad"
#define SHADER_RENDER_DS "DS_Shading"

#define FRAMEBUFFER_POSITION "FramebufferPosition"
#define FRAMEBUFFER_NORMAL "FramebufferNormal"
#define FRAMEBUFFER_TEXTURE "FramebufferTexture"
#define FRAMEBUFFER_DEPTH "FramebufferDepth"

#define RENDER_POSITION 1
#define RENDER_NORMAL 2
#define RENDER_LIGHTING 0
#define RENDER_TEXTURE 3
#define RENDER_DS 4

#define RED_LIGHT "red_light"
#define GREEN_LIGHT "green_light"
#define BLUE_LIGHT "yellow_light"
#define WHITE_LIGHT "white_light"
#define CYAN_LIGHT "cyan_light"
#define MAROON_LIGHT "maroon_light"
#define OLIVE_LIGHT "olive_light"
#define PURPLE_LIGHT "purple_light"
#define GOLD_LIGHT "gold_light"
#define GHOSTWHITE_LIGHT "ghostwhite_light"

#define RED glm::vec3(1.0,0.0,0.0)
#define GREEN glm::vec3(0.0,1.0,0.0)
#define BLUE glm::vec3(0.0,0.0,1.0)
#define WHITE glm::vec3(1.0,1.0,1.0)
#define CYAN glm::vec3(0.0,1.0,1.0)
#define MAROON glm::vec3(0.5,0.0,0.0)
#define OLIVE glm::vec3(0.5,0.5,0.0)
#define PURPLE glm::vec3(0.5,0.0,0.5)
#define TEAL glm::vec3(0.0,0.5,0.5)
#define GOLD glm::vec3(1.0,0.843,0.0)
#define GHOSTWHITE glm::vec3(0.972,0.972,1.0)

#define LED_Obj_To_Light(name)  dynamic_cast<CLED_Light*>(wxGetApp().LightCache().QueryObject(name))


/*
class CLED_MainApplication;
DECLARE_APP(CLED_MainApplication)
*/ 