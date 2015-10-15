#pragma once

#include "LED_SceneNode.h"
#include "LED_Camera.h"
#include <map>
#include <vector>
#include <wx/wx.h>

//场景树根节点名字为 ROOT

class CLED_SceneManager 
{
protected:
	std::map<wxString,CLED_SceneNode*> m_renderObjs;
	CLED_SceneNode* m_sceneRoot;
public:
	CLED_SceneManager();
	~CLED_SceneManager();

	void addNode(const wxString& name,const CLED_SceneNode* newNode,const wxString& parent = wxString("ROOT"));//默认加入到根场景节点中
	void deleteNode(const wxString& name);

	CLED_SceneNode* getNode(const wxString& name);
	void drawScene(const CLED_Camera& camera,int shaderIndex);
};