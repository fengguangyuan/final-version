#include <wx/spinctrl.h>
#include "updateEvent.h"
#include "LED_header.h"
#include "LED_MainApplication.h"
#include "LED_EditMenuEVT.h"

void LightsMenuEVT::lightchoose()
{

}

//open light or close
//void LightsMenuEVT::lightOperates (wxCommandEvent &event)
//{
//	wxObject *obj = event.GetEventObject ( );
//	const wxChar *c = event.GetEventObject ( )->GetClassInfo ( )->GetClassName ( );
//	int i = event.GetId ( ) - 8000;
//	lightCtrl (i, c, obj);
//}
void LightsMenuEVT::delObject (wxCommandEvent &event)
{
	const wxTreeItemId itemId = tree->GetFocusedItem ( );
	wxString str = tree->GetItemText (itemId);
	if (!(str.IsEmpty ( )) || "Root" == str)
	{
		wxGetApp ( ).SceneManager ( )->deleteNode (str);
		wxGetApp ( ).ModelCache ( ).DeleteObject (str, TRUE);
		tree->DeleteChildren (itemId);
		tree->Delete (itemId);
		wxMessageBox ("ɾ������ɹ�", "ɾ������",
			5l, NULL, wxSYS_SCREEN_X / 2, wxSYS_SCREEN_Y / 2);
	}
	else
	{
		wxMessageBox ("����ȷѡ�����壡", "tips",
			5l, NULL, wxSYS_SCREEN_X / 2, wxSYS_SCREEN_Y / 2);
	}
}

void LightsMenuEVT::abnewfileUs (wxCommandEvent &event)
{
	wxMessageBox (wxString ("���й���ѧ������ѧ���ѧԺLEDС��������"), wxString ("��������...."), 
		5L, 0, wxCENTER_ON_SCREEN, wxCENTER_ON_SCREEN);
}

/***************************functions as tools**************************/
//void LightsMenuEVT::lightCtrl (const int id, const wxChar *name,wxObject *obj) const
//{
//	if (wxGetApp ( ).MyLights.at (id)->getStatus ( ))
//	{
//		wxGetApp ( ).MyLights.at (id)->closeLight ( );
//		if (!wxStrcmp (name, wxString ("wxButton")))
//			(dynamic_cast<wxButton *>(obj))->SetLabelText (wxString ("NO"));
//	}
//	else
//	{
//		wxGetApp ( ).MyLights.at (id)->openLight ( );
//		if (!wxStrcmp (name, wxString ("wxButton")))
//			(dynamic_cast<wxButton *>(obj))->SetLabelText (wxString ("YES"));
//	}
//}