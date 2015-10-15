#pragma once

#include <wx/wx.h>

class LightsMenuEVT : public wxFrame
{
public:
	LightsMenuEVT ( ) 
	{
	}

	static void lightchoose ( );
	void lightOperates (wxCommandEvent &event);
	void abnewfileUs (wxCommandEvent &event);
	void delObject ( wxCommandEvent &event);
	~LightsMenuEVT ( )
	{
	}
private:
	void lightCtrl ( const int id, const wxChar *name, wxObject *obj) const;
};
