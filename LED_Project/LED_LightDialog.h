#pragma once


#include <wx/wx.h>
#include "wx/window.h"
#include "wx/sizer.h"
#include "LED_header.h"
#include "wx/checkbox.h"
#include "LED_ObjectModel.h"
#include "LED_Light.h"




class CLED_LightDialog : public wxDialog
{
public:
	CLED_LightDialog(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, const wxCommandEvent& event,int test);
	
	void Onlightdd(wxCommandEvent &event);

	CLED_Light*  m_lightChoosed;

	
private:
	wxSpinCtrl*     light_spinctrl1;
	wxSpinCtrl*     light_spinctrl2;
	wxSpinCtrl*     light_spinctrl3;
	float           xx_value;
	float           yy_value;
	float           zz_value;
	wxCheckBox*     CheckBox;
	wxString        c;
	wxSlider*       slider1;
	wxSlider*       slider2;
	wxSlider*       slider3;
	bool            test;


    void OnChangeLightSpinctrl(wxSpinEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnSliderChange(wxCommandEvent& event);
	
    DECLARE_EVENT_TABLE()
};
