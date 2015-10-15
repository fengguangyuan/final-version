#pragma once

#include "LED_MainApplication.h"
#include "LED_MainFrame.h"

#include "updateEvent.h"
#include "LED_WinMenuBar.h"
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include "LED_header.h"
#include "LED_camera.h"

#include "light.xpm"
#include "wenjian.xpm"
#include "xiaowenjian.xpm"





BEGIN_EVENT_TABLE(CLED_MainFrame,wxFrame)
	EVT_MENU(wxID_EXIT,CLED_MainFrame::OnExit)
	EVT_PAINT(CLED_MainFrame::OnPaint)
	EVT_SIZE(CLED_MainFrame::OnSize)
	EVT_TIMER(MAIN_TIMER_ID,CLED_MainFrame::OnTimer)
	EVT_KEY_DOWN(CLED_MainFrame::OnKeyDown)
	EVT_SPLITTER_SASH_POS_CHANGING(MAIN_SPLITTER_ID,CLED_MainFrame::OnSplitterChanged)
	EVT_TREE_SEL_CHANGED(MAIN_FRAME_wxtree, CLED_MainFrame::OnChoose)
	EVT_TREE_ITEM_RIGHT_CLICK(MAIN_FRAME_wxtree, CLED_MainFrame::OnRightClick)
	EVT_TREE_ITEM_RIGHT_CLICK(MAIN_FRAME_wxlighttree, CLED_MainFrame::OnLightRightClick)
	EVT_SPINCTRL(MAIN_FRAME_wxSpinCtrl, CLED_MainFrame::OnChangeSpinctrl)
END_EVENT_TABLE()




CLED_MainFrame::CLED_MainFrame(wxFrame *frame, 
								wxWindowID id,
								const wxString& title, 
								const wxPoint& pos /* = wxDefaultPosition */, 
								const wxSize& size /* = wxDefaultSize */, 
								long style /* = wxDEFAULT_FRAME_STYLE */):
									wxFrame(frame,id,title,pos,size),m_timer(this,MAIN_TIMER_ID)
{


	//设置定时刷新
	m_timer.Start(40);//40ms/frame = 25fps
	ifchoose = false;

	//窗口划分
	wxRect clientRect = GetClientRect ( );

	m_splitter = new wxSplitterWindow( this , MAIN_SPLITTER_ID ,
		wxPoint(clientRect.GetTop(),clientRect.GetLeft()),
		wxSize(clientRect.GetWidth(),clientRect.GetHeight()),
		wxSP_NO_XP_THEME );

	m_windowA = new wxScrolledWindow(m_splitter, MAIN_CONTROL_WINDOW, wxPoint(0, 0), wxSize(400, 400), wxVSCROLL);
	m_windowA->SetScrollbars(10,10,1000,1000);
	m_windowB = new wxWindow(m_splitter,MAIN_GL_WINDOW);

	m_windowA->Show(true);
	m_windowB->Show(true);

	m_splitter->Initialize(m_windowA);
	m_splitter->Initialize(m_windowB);

	m_splitter->SplitVertically(m_windowA,m_windowB,180);

	m_splitter -> SetSashSize(MAIN_SPLITTER_SIZE);


	//对应窗口的OpenGL环境

	CLED_LOG::LogWrite(wxString("Initialize OpenGL...\n"));

	m_glCanvas = new CLED_OpenGLCanvas(m_windowB,MAIN_GL_CANVAS,wxPoint(0,0),m_windowB->GetSize());
	wxSizeEvent sizeEvt(m_windowB->GetSize(),MAIN_GL_CANVAS);
	wxPostEvent(m_glCanvas,sizeEvt);
	
	Show(true);

	CLED_LOG::LogWrite(wxString("Initialize OpenGL Successfully\n"));

	m_preRect = GetRect();

	//wxImageList* imagelist = new wxImageList()
	lighttree = new wxTreeCtrl(m_windowA, MAIN_FRAME_wxlighttree, wxPoint(0, 0), wxSize(120, 250), wxTR_HAS_BUTTONS | wxTR_SINGLE);
	wxTreeItemId lightrootId = lighttree->AddRoot(wxT("灯光"), 0, 0, new MyTreeItemData(wxT("Root item")));
	
	
	wxTreeItemId itemId1 = lighttree->AppendItem(lightrootId, wxT("red_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId2 = lighttree->AppendItem(lightrootId, wxT("Cylinder02"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId3 = lighttree->AppendItem(lightrootId, wxT("yellow_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId4 = lighttree->AppendItem(lightrootId, wxT("white_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId5 = lighttree->AppendItem(lightrootId, wxT("cyan_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId6 = lighttree->AppendItem(lightrootId, wxT("maroon_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId7 = lighttree->AppendItem(lightrootId, wxT("olive_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId8 = lighttree->AppendItem(lightrootId, wxT("purple_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId9 = lighttree->AppendItem(lightrootId, wxT("gold_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId10 = lighttree->AppendItem(lightrootId, wxT("ghostwhite_light"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	lighttree->ExpandAll();

	wxImageList* lightlist = new wxImageList(16, 16);
	lightlist->Add(wxIcon(wenjian_xpm));
	lightlist->Add(wxIcon(light_xpm));
	lighttree->AssignImageList(lightlist);


	tree = new wxTreeCtrl(m_windowA, MAIN_FRAME_wxtree, wxPoint(0, 200), wxSize(120, 180), wxTR_HAS_BUTTONS | wxTR_SINGLE);
	
	wxTreeItemId rootId = tree->AddRoot(wxT("物体"), 0, 0,new MyTreeItemData(wxT("Root item")));
	rootid = rootId;

	//updateTreeCtrl(wxString("SOFA"));
	//updateTreeCtrl(wxString("DESK"));
	//tree->SetImageList();
	wxTreeItemId itemId00 = lighttree->AppendItem(rootId, wxT("SOFA"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));
	wxTreeItemId itemId11 = lighttree->AppendItem(rootId, wxT("DESK"), 1, 1, new MyTreeItemData(wxT(" File item 1 ")));


	tree->ExpandAll();
	
	wxImageList* obglist = new wxImageList(16, 16);
	obglist->Add(wxIcon(wenjian_xpm));
	obglist->Add(wxIcon(xiaowenjian_xpm));
	tree->AssignImageList(obglist);

	

	wxStaticText* x_text1 = new wxStaticText(m_windowA, wxID_ANY, "x轴平移", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* x_text2 = new wxStaticText(m_windowA, wxID_ANY, "x轴旋转", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* y_text1 = new wxStaticText(m_windowA, wxID_ANY, "y轴平移", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* y_text2 = new wxStaticText(m_windowA, wxID_ANY, "y轴旋转", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* z_text1 = new wxStaticText(m_windowA, wxID_ANY, "z轴平移", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* z_text2 = new wxStaticText(m_windowA, wxID_ANY, "z轴旋转", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	wxStaticText* bigsmall_text = new wxStaticText(m_windowA, wxID_ANY, "物体缩放", wxDefaultPosition, wxSize(50, 30), wxST_NO_AUTORESIZE);
	

	spinctrl1 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl2 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl3 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl4 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl5 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl6 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);
	spinctrl7 = new wxSpinCtrl(m_windowA, MAIN_FRAME_wxSpinCtrl, wxT("0"), wxDefaultPosition, wxSize(50, 30), wxSP_ARROW_KEYS, -10000, 10000, 0);

	spinctrl1->SetLabel(wxString("x"));
	spinctrl2->SetLabel(wxString("p"));  //x轴旋转
	spinctrl3->SetLabel(wxString("y"));
	spinctrl4->SetLabel(wxString("w"));  //y轴旋转
	spinctrl5->SetLabel(wxString("z"));
	spinctrl6->SetLabel(wxString("r"));  //z轴旋转
	spinctrl7->SetLabel(wxString("s"));  //z轴旋转

	

	
	sizer = new wxFlexGridSizer(0, 1, 0, 0);
	wxFlexGridSizer* sizer_text_x = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* sizer_x = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* sizer_text_y = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* sizer_y = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* sizer_text_z = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* sizer_z = new wxFlexGridSizer(1, 2, 0, 0);
	wxFlexGridSizer* show = new wxFlexGridSizer(1, 2, 0, 0);

	show->Add(bigsmall_text, 0, wxALIGN_CENTER | wxALL, 10);
	show->Add(spinctrl7, 0, wxALIGN_CENTER | wxALL, 10);

	sizer_x->Add(spinctrl1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_x->Add(spinctrl2, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_y->Add(spinctrl3, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_y->Add(spinctrl4, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_z->Add(spinctrl5, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_z->Add(spinctrl6, 0, wxALIGN_CENTER | wxALL, 10);

	sizer_text_x->Add(x_text1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_text_x->Add(x_text2, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_text_y->Add(y_text1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_text_y->Add(y_text2, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_text_z->Add(z_text1, 0, wxALIGN_CENTER | wxALL, 10);
	sizer_text_z->Add(z_text2, 0, wxALIGN_CENTER | wxALL, 10);

	sizer->Add(lighttree, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(tree, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_text_x, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_x, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_text_y, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_y, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_text_z, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(sizer_z, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->Add(show, 0, wxALIGN_CENTER | wxALL, 10);
	
	

	m_windowA->SetSizer(sizer);
	sizer->Fit(m_windowA);
	sizer->SetSizeHints(m_windowA);
	
	//在这里添加菜单栏
	LED_WinMenuBar *menuBar = new LED_WinMenuBar ( );
	SetMenuBar (menuBar->initMenuBars ( ));

	//CLED_MainFrame::DrawPicture();
	
}


CLED_MainFrame::~CLED_MainFrame()
{
	if(m_glCanvas)
		delete m_glCanvas;
}

const COpenGLEnv& CLED_MainFrame::getOpenGLEnv()
{
	if(m_glCanvas)
		return m_glCanvas->getOpenGLEnv();
}

void CLED_MainFrame::OnPaint(wxPaintEvent& event)
{
	wxFrame::OnPaint(event);
	m_glCanvas->m_glPaintEvt = new wxPaintEvent(MAIN_GL_CANVAS);
	wxQueueEvent(m_glCanvas, m_glCanvas->m_glPaintEvt);
	//OnSplitterChanged(wxSplitterEvent());
}

void CLED_MainFrame::OnSize(wxSizeEvent& event)
{

	bool size_changed = false;
	wxFrame::OnSize(event);

	wxRect curRect = GetRect();

	if (curRect.GetWidth() != m_preRect.GetWidth() ||
		curRect.GetHeight() != m_preRect.GetHeight())
		size_changed = true;
	m_preRect = curRect;
	OnSplitterChanged(wxSplitterEvent());
	if (size_changed)
		OnPaint(wxPaintEvent(MAIN_FRAME));
}

void CLED_MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void CLED_MainFrame::OnSplitterChanged(wxSplitterEvent& event)
{
	int splitterPosition = m_splitter->GetSashPosition();
	int offset = splitterPosition + MAIN_SPLITTER_SIZE;
	int canvasWidth = GetClientRect().GetWidth() - offset;

	// 	wxSize& size = const_cast<wxSize&>(wxGetApp().getOpenGLEnv().m_size);
	// 	size.Set(canvasWidth,GetClientRect().GetHeight());

	m_glCanvas->m_glSizeEvt = new wxSizeEvent(wxSize(canvasWidth, GetClientRect().GetHeight()), MAIN_GL_CANVAS);
	m_glCanvas->m_glPaintEvt = new wxPaintEvent(MAIN_GL_CANVAS);

	wxQueueEvent(m_glCanvas, m_glCanvas->m_glSizeEvt);
	wxQueueEvent(m_glCanvas, m_glCanvas->m_glPaintEvt);
}

void CLED_MainFrame::OnTimer(wxTimerEvent& event)
{
	//OnSplitterChanged(wxSplitterEvent());
	OnPaint(wxPaintEvent());
}

//这里处理按键事件
void CLED_MainFrame::OnKeyDown(wxKeyEvent& event)
{
	CLED_Camera* mainCamera = dynamic_cast<CLED_Camera*>(wxGetApp().CamerasCache().QueryObject(LED_MAIN_CAMERA));
	int code = event.GetKeyCode();
	if (code == 'w' || code == 'W')
		mainCamera->stepForwardBack(1.0f);
	if (code == 'S' || code == 's')
		mainCamera->stepForwardBack(-1.0f);
	if (code == 'A' || code == 'a')
		mainCamera->stepRightLeft(-1.0f);
	if (code == 'D' || code == 'd')
		mainCamera->stepRightLeft(1.0f);
	if (code == 'Q' || code == 'q')
		mainCamera->stepUpDown(1.0f);
	if (code == 'e' || code == 'E')
		mainCamera->stepUpDown(-1.0f);

	if (code == 'J' || code == 'j')
		mainCamera->roll(1.0f);                //Z轴旋转
	if (code == 'L' || code == 'l')
		mainCamera->roll(-1.0f);
	if (code == 'U' || code == 'u')
		mainCamera->yaw(1.0f);                //Y轴旋转
	if (code == 'O' || code == 'o')
		mainCamera->yaw(-1.0f);
	if (code == 'I' || code == 'i')
		mainCamera->pitch(1.0f);              //X轴旋转
	if (code == 'K' || code == 'k')
		mainCamera->pitch(-1.0f);

	wxGetApp().CamerasCache().DeleteObject(LED_MAIN_CAMERA);
}



void CLED_MainFrame::OnChangeSpinctrl(wxSpinEvent& event)
{
	if (ifchoose == true)
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
					float x_value_change = spinctrl1->GetValue();
					treeTranNode->rightLeft((x_value_change - x_value)/2);
					x_value = x_value_change;
					break;
		}
		case 'y':
		{
					float y_value_change = spinctrl3->GetValue();
					treeTranNode->upDown((y_value_change - y_value)/2);
					y_value = y_value_change;
					break;
		}
		case 'z':
		{
					float z_value_change = spinctrl5->GetValue();
					treeTranNode->forwardBack((z_value_change - z_value)/2);
					z_value = z_value_change;
					break;
		}
		case 'p':
		{
					float p_value_change = spinctrl2->GetValue();
					treeTranNode->pitch(p_value_change - p_value);
					p_value = p_value_change;
					break;
		}
		case 'w':
		{
					float w_value_change = spinctrl4->GetValue();
					treeTranNode->yaw(w_value_change - w_value);
					w_value = w_value_change;
					break;
		}
		case 'r':
		{
					float r_value_change = spinctrl6->GetValue();
					treeTranNode->roll(r_value_change - r_value);
					r_value = r_value_change;
					break;
		}
		case 's':
		{
					float s_value_change = spinctrl7->GetValue();
					float change_ = s_value_change - s_value;
					if (change_ > 0){
						treeTranNode->scale(1.1);
					}
					else
					{
						treeTranNode->scale(0.9);
					}
					s_value = s_value_change;
					break;
		}
		}
	}
	else
	{
		//wxMessageBox(wxT("请右键选择物体后再进行位置变换操作！"), wxT("错误"), wxNO_DEFAULT | wxYES | wxICON_INFORMATION, this);
	}
}

void CLED_MainFrame::OnChoose(wxTreeEvent& event)
{
	ifchoose = true;
	wxTreeItemId id = tree->GetSelection();
	wxString wxname = tree->GetItemText(id);
	CLED_SceneManager* treeTran = wxGetApp().SceneManager();
    CLED_SceneNode* treeTranNode = treeTran->getNode(wxname);

	x_value = 0;
	y_value = 0;
	z_value = 0;
	p_value = 0;
	w_value = 0;
	r_value = 0;
	s_value = 0;

	spinctrl1->SetValue(0);
	spinctrl3->SetValue(0);
	spinctrl5->SetValue(0);
	spinctrl2->SetValue(0);
	spinctrl4->SetValue(0);
	spinctrl6->SetValue(0);
	spinctrl7->SetValue(0);

	//CheckBox->SetValue(false);
	
}


void CLED_MainFrame::OnDeleteObj(wxCommandEvent& event)
{
	
	const wxTreeItemId itemId = tree->GetFocusedItem();
	wxString str = tree->GetItemText(itemId);
	if (!(str.IsEmpty()) || "Root" == str)
	{
		wxGetApp().SceneManager()->deleteNode(str);
		wxGetApp().ModelCache().DeleteObject(str, TRUE);
		tree->DeleteChildren(itemId);
		tree->Delete(itemId);
		wxMessageBox("Delete object successfully!!", "DELETE OBJECT",
			5l, NULL, wxSYS_SCREEN_X / 2, wxSYS_SCREEN_Y / 2);
	}
	else
	{
		wxMessageBox("Please choose the object correctly!!", "tips",
			5l, NULL, wxSYS_SCREEN_X / 2, wxSYS_SCREEN_Y / 2);
	}
	
}



void CLED_MainFrame::OnRightClick(wxTreeEvent& event)
{
	wxMenu* chooseMenu = new wxMenu;
	chooseMenu->Append(10001, wxT("物体控制面板"));
	chooseMenu->Connect(10001, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CLED_MainFrame::OnChonObj));
	chooseMenu->Append(10000, wxT("删除物体"));
	chooseMenu->Connect(10000, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CLED_MainFrame::OnDeleteObj));
	
	PopupMenu(chooseMenu, wxDefaultPosition);
	
}

void CLED_MainFrame::OnLightRightClick(wxTreeEvent& event)
{
	wxMenu* chooseMenu = new wxMenu;
	chooseMenu->Append(10002, wxT("灯光控制面板"));
	chooseMenu->Connect(10002, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CLED_MainFrame::OnChonlight));
	PopupMenu(chooseMenu, wxDefaultPosition);
}

void CLED_MainFrame::OnChonObj(wxCommandEvent& event)
{
	CLED_ObjDialog* objDialog = new CLED_ObjDialog(NULL, wxID_ANY, wxT("物体控制面板"), wxPoint(150, 150), wxDefaultSize, wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX);
}

void CLED_MainFrame::OnChonlight(wxCommandEvent& event)
{
	CLED_LightDialog* lightDialog = new CLED_LightDialog(NULL, wxID_ANY, wxT("灯光控制面板"), wxPoint(150, 150), wxDefaultSize, wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX, event,1);
}


