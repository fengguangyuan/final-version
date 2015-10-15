#pragma once

#include "LED_WinMenuBar.h"
#include "openfile.h"

wxMenuBar* LED_WinMenuBar :: initMenuBars ()
{
	//����������ļ������˵�
	wxMenu* file_menu = new wxMenu;
	file_menu->Append (wxID_OPEN, wxT ("�������..."));	
	file_menu->Append (wxID_FILE, wxT ("��Ӳ����ļ�..."));
	file_menu->AppendSeparator ( );
	file_menu->Append (wxID_SAVE, wxT ("����"));
	file_menu->Append (wxID_SAVEAS, wxT ("���Ϊ..."));

	file_menu->Connect (wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (Openfile::OnOpen));
	file_menu->Connect (wxID_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (Openfile::OnOpenTexture));
	file_menu->Connect (wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (Openfile::OnSave));
	file_menu->Connect (wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (Openfile::OnSaveAs));
	file_menu->Append (wxID_EXIT, wxT ("�˳����"));
	menuBar->Append (file_menu, wxT ("�ļ�"));

	//��������ӱ༭�����˵���
	edit_menu = new wxMenu;
	add_lights = new wxMenu;
	//wxMenuItem *tmp;
	edit_menu->AppendSubMenu (add_lights, wxT ("�ƹ�ѡ��"));



	tmp = add_lights->Append(8000, RED_LIGHT, "enable RED_LIGHT");
	add_lights->Append(8001, GREEN_LIGHT, "enable GREEN_LIGHT");
	add_lights->Append(8002, BLUE_LIGHT, "enable BLUE_LIGHT");
	add_lights->Append(8003, WHITE_LIGHT, "enable WHITE_LIGHT");
	add_lights->Append(8004, CYAN_LIGHT, "enable CYAN_LIGHT");
	add_lights->Append(8005, MAROON_LIGHT, "enable MAROON_LIGHT");
	add_lights->Append(8006, OLIVE_LIGHT, "enable OLIVE_LIGHT");
	add_lights->Append(8007, PURPLE_LIGHT, "enable PURPLE_LIGHT");
	add_lights->Append(8008, GOLD_LIGHT, "enable GOLD_LIGHT");
	add_lights->Append(8009, GHOSTWHITE_LIGHT, "enable GHOSTWHITE_LIGHT");
	
	


	add_lights->Connect(8000, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8001, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8002, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8003, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8004, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8005, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8006, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8007, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8008, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));
	add_lights->Connect(8009, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LED_WinMenuBar::OnEnAbleLight));

	menuBar->Append (edit_menu, wxT ("�༭"));
	//delete menu item
	edit_menu->Append (wxID_DELETE, "ɾ������");
	edit_menu->Connect (wxID_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (LightsMenuEVT::delObject));
	wxMenu* tool = new wxMenu;
	menuBar->Append(tool, wxT("����"));
	wxMenu* CAD = new wxMenu;
	menuBar->Append(CAD, wxT("CAD"));
	wxMenu* view = new wxMenu;
	menuBar->Append(view, wxT("��ͼ"));
	wxMenu* windo = new wxMenu;
	menuBar->Append(windo, wxT("����"));
	wxMenu* listlist = new wxMenu;
	menuBar->Append(listlist, wxT("����"));
	//���������Help�˵�
	wxMenu *help_menu = new wxMenu;
	tmp = help_menu->Append (wxID_ANY, wxString ("��������.."));
	help_menu->Connect (wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (LightsMenuEVT::abnewfileUs));
	menuBar->Append (help_menu, wxT ("����"));
	//end
	return menuBar;
}


void LED_WinMenuBar::OnEnAbleLight(wxCommandEvent& event)
{
	
	lightDialog = new CLED_LightDialog(NULL, wxID_ANY, wxT("�ƹ�������"), wxPoint(150, 150), wxDefaultSize, wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX,event,0);
	
}


