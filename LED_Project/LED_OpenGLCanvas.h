#pragma once

#include <wx/glcanvas.h>
#include <wx/wx.h>
#include "LED_RenderToTexture.h"
#include "LED_OpenGLEnv.h"
#include "LED_Camera.h"
#include "LED_ShaderProgram.h"
#include "LED_Light.h"
#include "LED_DefferredRendering.h"

#define TEX_RTT 0
#define POS_RTT 1
#define NOR_RTT 2
#define LIGHT_RTT 3

class CLED_OpenGLCanvas : public wxGLCanvas
{
public:
	CLED_OpenGLCanvas(wxWindow* parent,int id,const wxPoint& pos,const wxSize& size);
	virtual ~CLED_OpenGLCanvas();

	const COpenGLEnv& getOpenGLEnv();

	wxSizeEvent* m_glSizeEvt;
	wxPaintEvent* m_glPaintEvt;

protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void Init();

	void resetRender();

	void rebuildDS();

	void renderQuad(CLED_Light* light);

	void calculateLighting();
private:
	COpenGLEnv m_glEnv;
	CLED_Camera* m_mainCamera;
	bool	   m_canvasSizeChanged;

	GLuint		m_quadVB;
	GLuint		m_quadUVB;

	//延迟渲染用的几个纹理
// 	CLED_RenderToTexture* m_PosTex;
// 	CLED_RenderToTexture* m_NorTex;
// 	CLED_RenderToTexture* m_TexTex;

	CLED_ShaderProgram*   m_DSShader;//延迟渲染用的shader
	CLED_DSRender* m_ds;
	CLED_ShaderProgram*	m_LightCalSahder;

	GLuint texRTT[4];
	GLuint fboRTT[4];
	GLuint rboRTT[4];

	DECLARE_EVENT_TABLE();
};