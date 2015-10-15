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
#include "LED_ObjDialog.h"







BEGIN_EVENT_TABLE(CLED_ObjDialog, wxDialog)
   EVT_SPINCTRL(MAIN_FRAME_light_wxSpinCtrl, CLED_ObjDialog::OnChangeObjSpinctrl)
END_EVENT_TABLE()


CLED_ObjDialog::CLED_ObjDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
                     :wxDialog(parent, id, caption, pos, size, style)

{
	x_value = 0;
	y_value = 0;
	z_value = 0;
	p_value = 0;
	w_value = 0;
	r_value = 0;

	//const wxTreeItemId itemId = tree->GetFocusedItem();
	//str = tree->GetItemText(itemId);



	wxFlexGridSizer* sizer_light = new wxFlexGridSizer(0, 1, 0, 0);



	wxStaticText* _x_text = new wxStaticText(this, wxID_ANY, "xÖáÆ½ÒÆ", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* _y_text = new wxStaticText(this, wxID_ANY, "yÖáÆ½ÒÆ", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* _z_text = new wxStaticText(this, wxID_ANY, "zÖáÆ½ÒÆ", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* q_x_text = new wxStaticText(this, wxID_ANY, "xÖáÐý×ª", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* q_y_text = new wxStaticText(this, wxID_ANY, "yÖáÐý×ª", wxDefaultPosition, wxSize(50, 30));
	wxStaticText* q_z_text = new wxStaticText(this, wxID_ANY, "zÖáÐý×ª", wxDefaultPosition, wxSize(50, 30));

	obj_spinctrl1 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	obj_spinctrl2 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	obj_spinctrl3 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	obj_spinctrl4 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	obj_spinctrl5 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	obj_spinctrl6 = new wxSpinCtrl(this, MAIN_FRAME_light_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);

	obj_spinctrl1->SetLabel(wxString("x"));
	obj_spinctrl2->SetLabel(wxString("y"));
	obj_spinctrl3->SetLabel(wxString("z"));
	obj_spinctrl4->SetLabel(wxString("p"));
	obj_spinctrl5->SetLabel(wxString("w"));
	obj_spinctrl6->SetLabel(wxString("r"));





	

	wxFlexGridSizer* text1 = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* text2 = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* text3 = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* spin1 = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* spin2 = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* spin3 = new wxFlexGridSizer(1, 2, 0, 0);

	text1->Add(_x_text, 0, wxALIGN_CENTER | wxALL, 10);
	text1->Add(q_x_text, 0, wxALIGN_CENTER | wxALL, 10);

	text2->Add(_y_text, 0, wxALIGN_CENTER | wxALL, 10);
	text2->Add(q_y_text, 0, wxALIGN_CENTER | wxALL, 10);

	text3->Add(_z_text, 0, wxALIGN_CENTER | wxALL, 10);
	text3->Add(q_z_text, 0, wxALIGN_CENTER | wxALL, 10);

	spin1->Add(obj_spinctrl1, 0, wxALIGN_CENTER | wxALL, 10);
	spin1->Add(obj_spinctrl4, 0, wxALIGN_CENTER | wxALL, 10);

	spin2->Add(obj_spinctrl2, 0, wxALIGN_CENTER | wxALL, 10);
	spin2->Add(obj_spinctrl5, 0, wxALIGN_CENTER | wxALL, 10);

	spin3->Add(obj_spinctrl3, 0, wxALIGN_CENTER | wxALL, 10);
	spin3->Add(obj_spinctrl6, 0, wxALIGN_CENTER | wxALL, 10);


	sizer_light->Add(text1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(spin1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(text2, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(spin2, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(text3, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_light->Add(spin3, 0, wxALIGN_CENTER | wxALL, 10);



	

	this->SetSizer(sizer_light);
	sizer_light->Fit(this);
	sizer_light->SetSizeHints(this);

	this->ShowModal();


}


void CLED_ObjDialog::OnChangeObjSpinctrl(wxSpinEvent& event)
{
	
		wxSpinCtrl* a = dynamic_cast<wxSpinCtrl*>(event.GetEventObject());
		char b = a->GetLabel().at(0);

		wxTreeItemId id = tree->GetSelection();
		wxString wxname = tree->GetItemText(id);

		CLED_SceneManager* treeTran = wxGetApp().SceneManager();
		CLED_SceneNode* treeTranNode = treeTran->getNode(wxname);

		switch (b)
		{
		case 'x':
		{
					float x_value_change = obj_spinctrl1->GetValue();
					treeTranNode->rightLeft((x_value_change - x_value) / 2);
					x_value = x_value_change;
					break;
		}
		case 'y':
		{
					float y_value_change = obj_spinctrl2->GetValue();
					treeTranNode->upDown((y_value_change - y_value) / 2);
					y_value = y_value_change;
					break;
		}
		case 'z':
		{
					float z_value_change = obj_spinctrl3->GetValue();
					treeTranNode->forwardBack((z_value_change - z_value) / 2);
					z_value = z_value_change;
					break;
		}
		case 'p':
		{
					float p_value_change = obj_spinctrl4->GetValue();
					treeTranNode->pitch(p_value_change - p_value);
					p_value = p_value_change;
					break;
		}
		case 'w':
		{
					float w_value_change = obj_spinctrl5->GetValue();
					treeTranNode->yaw(w_value_change - w_value);
					w_value = w_value_change;
					break;
		}
		case 'r':
		{
					float r_value_change = obj_spinctrl6->GetValue();
					treeTranNode->roll(r_value_change - r_value);
					r_value = r_value_change;
					break;
		}
		}
	
}