#pragma once

#include <wx/wx.h>
#include "LED_Texture.h"
#include "LED_Camera.h"
#include "LED_ShaderFragment.h"
#include "LED_ShaderProgram.h"
#include "LED_MainFrame.h"
#include "LED_ObjectCache.h"
#include "LED_SceneManager.h"


class CLED_MainApplication : public wxApp
{
public:
	CLED_MainApplication();
	virtual ~CLED_MainApplication();
public:
	virtual bool OnInit();
	virtual int OnExit();
protected:
	virtual int FilterEvent(wxEvent& event);
public:
	const COpenGLEnv& getOpenGLEnv();
	CLED_ObjectCache& ModelCache();
	CLED_ObjectCache& TextureCache();
	CLED_ObjectCache& SoundCache();
	CLED_ObjectCache& ShaderCache();
	CLED_ObjectCache& MaterialCache();
	CLED_ObjectCache& LightCache();
	CLED_ObjectCache& CamerasCache();
	CLED_SceneManager* SceneManager();
private:
	CLED_MainFrame* m_mainFrame;
private:
	//这里是相关数据
	CLED_ObjectCache* m_models;
	CLED_ObjectCache* m_textures;
	CLED_ObjectCache* m_sounds;
	CLED_ObjectCache* m_shaders;
	CLED_ObjectCache* m_materials;
	CLED_ObjectCache* m_lights;
	CLED_ObjectCache* m_cameras;
	CLED_SceneManager* m_scene;

};

DECLARE_APP(CLED_MainApplication)