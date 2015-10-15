#pragma once

#include <wx\wx.h>
#include <wx\menu.h>
#include "LED_LightDialog.h"
#include "LED_EditMenuEVT.h"

class LED_WinMenuBar : public wxFrame
{
public:
	LED_WinMenuBar () :menuBar (new wxMenuBar)
	{
	}

	wxMenuBar* initMenuBars ();
	
private:
	wxMenuBar *menuBar;
	void OnEnAbleLight(wxCommandEvent& event);
	CLED_LightDialog* lightDialog;
	wxMenu*           add_lights;
	wxMenuItem*       tmp;
	wxMenu*           edit_menu;
};