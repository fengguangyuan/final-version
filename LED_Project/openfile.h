
#pragma once

#include <wx/wx.h>
#include <wx\file.h>
#include "LED_ObjectModel_EXT.h"

class Openfile : public wxFrame
{
public:
	Openfile (const wxString & title);

	void OnOpen (wxCommandEvent &event);

	void OnOpenTexture (wxCommandEvent &event);

	void OnSave (wxCommandEvent &event);
	void OnSaveAs (wxCommandEvent &event);
	
	wxTextCtrl * tc;
	
private:
	wxString getRealName (wxString & str);
	void writeFile (wxFile *file) const;
	CLED_ObjectModel* m_newModel;
};
