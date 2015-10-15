#include "updateEvent.h"
wxTreeCtrl* tree = NULL;
wxTreeCtrl* lighttree = NULL;
wxTreeItemId rootid = NULL;
wxScrolledWindow* m_windowA = NULL;
wxFlexGridSizer* sizer_light = NULL;
wxFlexGridSizer* sizer_model = NULL;
wxTreeCtrl*    getTreeCtrl()
{
	return tree;
}

bool updateTreeCtrl(wxString name)
{
	if (tree->AppendItem(rootid, name, 1, 1))
		return TRUE;
	else
		return FALSE;
}


