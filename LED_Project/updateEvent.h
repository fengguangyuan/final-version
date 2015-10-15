#ifndef _UPDATEEVENT_H
#define _UPDATEEVENT_H

#include <wx/wx.h>
#include <wx/treectrl.h>

extern wxTreeCtrl*         tree;      
extern wxTreeCtrl*         lighttree;

wxTreeCtrl*                getTreeCtrl();

bool                       updateTreeCtrl(wxString str);

extern wxTreeItemId rootid;              //ÎÄ¼þmainframe¡£cpp

extern wxScrolledWindow*   m_windowA;

extern wxFlexGridSizer*    sizer_model;
extern wxFlexGridSizer*    sizer_light;



#endif