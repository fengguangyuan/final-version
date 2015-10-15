#pragma once


#include <wx/wx.h>
#include "wx/window.h"
#include "wx/sizer.h"
#include "LED_header.h"
#include "wx/checkbox.h"
#include "LED_ObjectModel.h"


class CLED_ObjDialog : public wxDialog
{
public:
	CLED_ObjDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);

	void Onlightdd(wxCommandEvent &event);

	


private:
	wxSpinCtrl*     obj_spinctrl1;
	wxSpinCtrl*     obj_spinctrl2;
	wxSpinCtrl*     obj_spinctrl3;
	wxSpinCtrl*     obj_spinctrl4;
	wxSpinCtrl*     obj_spinctrl5;
	wxSpinCtrl*     obj_spinctrl6;
	float           x_value;
	float           y_value;
	float           z_value;
	float           p_value;
	float           w_value;
	float           r_value;
	wxString        c;
	wxSlider*       slider1;
	wxSlider*       slider2;
	wxSlider*       slider3;
	bool            test;
	wxString        str;


	void OnChangeObjSpinctrl(wxSpinEvent& event);

	DECLARE_EVENT_TABLE()
};