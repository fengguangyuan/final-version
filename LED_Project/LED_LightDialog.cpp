#pragma once

#include "LED_SceneNode.h"
#include "LED_SceneManager.h"
#include "LED_MainApplication.h"
#include "LED_MainFrame.h"
#include "updateEvent.h"
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include "LED_header.h"
#include "LED_camera.h"
#include "LED_LightDialog.h"

BEGIN_EVENT_TABLE(CLED_LightDialog, wxDialog)
    EVT_SPINCTRL(MAIN_FRAME_light_wxSpinCtrl, CLED_LightDialog::OnChangeLightSpinctrl)
	EVT_CHECKBOX(MAIN_FRAME_checkbox, CLED_LightDialog::OnCheckBox)
	EVT_SLIDER(MAIN_LIGHTCHOOSE_slider, CLED_LightDialog::OnSliderChange)
END_EVENT_TABLE()


CLED_LightDialog::CLED_LightDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, const wxCommandEvent& event,int test)
                         :wxDialog(parent, id, caption, pos, size, style)

{

	

	xx_value = 0;
	yy_value = 0;
	zz_value = 0;
	if (test == 0){
		wxObject* obj = event.GetEventObject();
		int aa = event.GetId();
		c = (dynamic_cast<wxMenu *>(obj))->GetLabelText(aa);
		
	}
	else
	{
		const wxTreeItemId itemId = lighttree->GetFocusedItem();
		c = lighttree->GetItemText(itemId);
	}
	m_lightChoosed = LED_Obj_To_Light(c);
	bool owneit = m_lightChoosed->m_lightOn;
	wxFlexGridSizer* sizer_light = new wxFlexGridSizer(0, 1, 0, 0);

	CheckBox = new wxCheckBox(this, MAIN_FRAME_checkbox, wxT("点亮灯光"), wxDefaultPosition, wxSize(100, 30));
	CheckBox->SetValue(owneit);

	wxStaticText* light_x_text = new wxStaticText(this, wxID_ANY, "x轴平移", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* light_y_text = new wxStaticText(this, wxID_ANY, "y轴平移", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* light_z_text = new wxStaticText(this, wxID_ANY, "z轴平移", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* light_r_text = new wxStaticText(this, wxID_ANY, "r", wxDefaultPosition, wxSize(10, 30));
	wxStaticText* light_b_text = new wxStaticText(this, wxID_ANY, "b", wxDefaultPosition, wxSize(10, 30));
	wxStaticText* light_g_text = new wxStaticText(this, wxID_ANY, "g", wxDefaultPosition, wxSize(10, 30));

	light_spinctrl1 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	light_spinctrl2 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	light_spinctrl3 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);

	light_spinctrl1->SetLabel(wxString("x"));   
	light_spinctrl2->SetLabel(wxString("y"));   
	light_spinctrl3->SetLabel(wxString("z"));   

	slider1 = new wxSlider(this, MAIN_LIGHTCHOOSE_slider, 0, 0, 255, wxDefaultPosition, wxSize(160, 25), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
	slider2 = new wxSlider(this, MAIN_LIGHTCHOOSE_slider, 0, 0, 255, wxDefaultPosition, wxSize(160, 25), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
	slider3 = new wxSlider(this, MAIN_LIGHTCHOOSE_slider, 0, 0, 255, wxDefaultPosition, wxSize(160, 25), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);

	slider1->SetLabel(wxString("r"));
	slider2->SetLabel(wxString("b"));
	slider3->SetLabel(wxString("g"));

	float lightr = m_lightChoosed->m_diffuse.r;
	float lightb = m_lightChoosed->m_diffuse.b;
	float lightg = m_lightChoosed->m_diffuse.g;

	
	//为rbg三色赋值

	slider1->SetValue(lightr*100);
	slider2->SetValue(lightb*100);
	slider3->SetValue(lightg*100);
	
	wxFlexGridSizer* text = new wxFlexGridSizer(1, 3, 0, 0);
	wxFlexGridSizer* spin = new wxFlexGridSizer(1, 3, 0, 0);
	wxFlexGridSizer* lider_r = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* lider_b = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* lider_g = new wxFlexGridSizer(1, 2, 0, 0);

	text->Add(light_x_text, 0, wxALIGN_CENTER | wxALL, 10);
	text->Add(light_y_text, 0, wxALIGN_CENTER | wxALL, 10);
	text->Add(light_z_text, 0, wxALIGN_CENTER | wxALL, 10);

	spin->Add(light_spinctrl1, 0, wxALIGN_CENTER | wxALL, 10);
	spin->Add(light_spinctrl2, 0, wxALIGN_CENTER | wxALL, 10);
	spin->Add(light_spinctrl3, 0, wxALIGN_CENTER | wxALL, 10);

	lider_r->Add(light_r_text, 0, wxALIGN_CENTER | wxALL, 10);
	lider_r->Add(slider1, 0, wxALIGN_CENTER | wxALL, 10);
	lider_b->Add(light_b_text, 0, wxALIGN_CENTER | wxALL, 10);
	lider_b->Add(slider2, 0, wxALIGN_CENTER | wxALL, 10);
	lider_g->Add(light_g_text, 0, wxALIGN_CENTER | wxALL, 10);
	lider_g->Add(slider3, 0, wxALIGN_CENTER | wxALL, 10);

	sizer_light->Add(CheckBox, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(text, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(spin, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(lider_r, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(lider_b, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(lider_g, 0, wxALIGN_CENTER | wxALL, 10);

  
	
	light_spinctrl1->SetValue(0);
	light_spinctrl2->SetValue(0);
	light_spinctrl3->SetValue(0);

	this->SetSizer(sizer_light);
	sizer_light->Fit(this);
	sizer_light->SetSizeHints(this);

	this->ShowModal();

	
}





void CLED_LightDialog::OnChangeLightSpinctrl(wxSpinEvent& event)
{
	    
	CLED_SceneManager* light_treeTran = wxGetApp().SceneManager();
	CLED_SceneNode* light_treeTranNode = light_treeTran->getNode(c);
		
	wxSpinCtrl* a = dynamic_cast<wxSpinCtrl*>(event.GetEventObject());
	char b = a->GetLabel().at(0);

	switch (b)
	{
	case 'x':
	{
				float x_value_change = light_spinctrl1->GetValue();
				light_treeTranNode->rightLeft((x_value_change - xx_value) / 2);
				m_lightChoosed->light_rightLeft((x_value_change - xx_value) / 2);
				xx_value = x_value_change;
				break;
	}
	case 'y':
	{
				float y_value_change = light_spinctrl2->GetValue();
				light_treeTranNode->upDown((y_value_change - yy_value) / 2);
				m_lightChoosed->light_upDown((y_value_change - yy_value) / 2);
				yy_value = y_value_change;
				break;
	}
	case 'z':
	{
				float z_value_change = light_spinctrl3->GetValue();
				light_treeTranNode->forwardBack((z_value_change - zz_value) / 2);
				m_lightChoosed->light_forwardBack((z_value_change - zz_value) / 2);
				zz_value = z_value_change;
				break;
	}
	}
}


void CLED_LightDialog::OnCheckBox(wxCommandEvent& event)
{
	bool ischecked=CheckBox->IsChecked();
	if (ischecked == true)
	{
	//light
		m_lightChoosed->m_lightOn = true;
	}
	else
	{
	//close light
		m_lightChoosed->m_lightOn = false;
	}
}


void CLED_LightDialog::OnSliderChange(wxCommandEvent& event)
{
	test = true;
	wxSlider* a = dynamic_cast<wxSlider*>(event.GetEventObject());
	char b = a->GetLabel().at(0);
	float clr = a->GetValue();

	switch (b)
	{
	case 'r':
	{
				m_lightChoosed->m_diffuse.r = clr*0.01;
				break;
	}
	case 'b':
	{
				m_lightChoosed->m_diffuse.b = clr*0.01;
				break;
	}
	case 'g':
	{
				m_lightChoosed->m_diffuse.g = clr*0.01;
				break;
	}
	}
}






