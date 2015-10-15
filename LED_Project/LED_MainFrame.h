#pragma once

#include "LED_SceneNode.h"
#include <wx/wx.h>
#include <wx/image.h>
#include "wx/window.h"
#include <wx/splitter.h>
#include "wx/scrolwin.h"
#include "wx/sizer.h"
#include "LED_OpenGLCanvas.h"
#include "LED_header.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "LED_SceneManager.h"
#include "LED_ObjectModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "wx/checkbox.h "
#include "LED_LightDialog.h"
#include "LED_ObjDialog.h"



class CLED_MainFrame : public wxFrame
{
public:
	CLED_MainFrame(wxFrame* frame,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style);
	virtual ~CLED_MainFrame();

	const COpenGLEnv& getOpenGLEnv();
	friend class CLED_MainApplication;
    CLED_OpenGLCanvas*	m_glCanvas;
	
	
private:

	wxSplitterWindow*	m_splitter;
	int					m_splitterPosition;
	wxWindow*			m_windowB;
	
	wxRect				m_preRect;			//Frame大小改变之前的Frame区域
	wxTimer				m_timer;
	wxRadioBox*         radiobox;
	wxFlexGridSizer*    sizer;
	wxSpinCtrl*         spinctrl1;
	wxSpinCtrl*         spinctrl2;
	wxSpinCtrl*         spinctrl3;
	wxSpinCtrl*         spinctrl4;
	wxSpinCtrl*         spinctrl5;
	wxSpinCtrl*         spinctrl6;
	wxSpinCtrl*         spinctrl7;
    float               x_value;
	float               y_value;
	float               z_value;
	float               p_value;
	float               w_value; 
	float               r_value;
	float               s_value;
	bool                ifchoose;
	wxPaintEvent*       a;
	wxString            msg;
	wxCheckBox*         CheckBox;
	CLED_LightDialog*   lightDialog;
	
	
	

	void OnExit(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSplitterChanged(wxSplitterEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnChoose(wxTreeEvent& event);
	void OnRightClick(wxTreeEvent& event);
	void OnLightRightClick(wxTreeEvent& event);
	void OnChangeSpinctrl(wxSpinEvent& event);
	void OnDeleteObj(wxCommandEvent& event);
	void OnChonObj(wxCommandEvent& event);
	void OnChonlight(wxCommandEvent& event);
	void DrawPicture();

	DECLARE_EVENT_TABLE();
};


class MyTreeItemData :public wxTreeItemData
{
public :
	MyTreeItemData(const wxString& desc) :m_desc(desc){}
	const wxString& GetDesc() const { return m_desc; }
private:
	wxString m_desc;   //绑定项是字符串，作为说明
};