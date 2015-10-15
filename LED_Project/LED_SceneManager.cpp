#include "LED_SceneManager.h"

CLED_SceneManager::CLED_SceneManager()
{
	m_sceneRoot = new CLED_SceneNode();
	std::pair<wxString,CLED_SceneNode*> p = std::make_pair(wxString("ROOT"),m_sceneRoot);
	m_renderObjs.insert(p);
}

CLED_SceneManager::~CLED_SceneManager()
{
	std::map<wxString,CLED_SceneNode*>::iterator it = m_renderObjs.begin();
	for (;it != m_renderObjs.end(); ++it)
	{
		delete it->second;
	}
}

CLED_SceneNode* CLED_SceneManager::getNode(const wxString& name)
{
	std::map<wxString,CLED_SceneNode*>::iterator it = m_renderObjs.find(name);
	if(it != m_renderObjs.end())
		return it->second;
	else
		return NULL;
}

void CLED_SceneManager::drawScene(const CLED_Camera& camera,int shaderIndex)
{
	m_sceneRoot->Draw(camera,shaderIndex);
}

void CLED_SceneManager::addNode(const wxString& name,const CLED_SceneNode* newNode,const wxString& parent)
{
	std::pair<wxString,CLED_SceneNode*> p = std::make_pair(name,const_cast<CLED_SceneNode*>(newNode));
	m_renderObjs.insert(p);
	CLED_SceneNode* parentNode = const_cast<CLED_SceneNode*>(getNode(parent));
	parentNode->addChild(newNode);
	const_cast<CLED_SceneNode*>(newNode)->setParent(parentNode);
}

void CLED_SceneManager::deleteNode(const wxString& name)
{
	CLED_SceneNode* node = getNode(name);
	CLED_SceneNode* parentNode = const_cast<CLED_SceneNode*>(node->getParent());
	std::map<wxString,CLED_SceneNode*>::iterator it = m_renderObjs.find(name);
	if(it != m_renderObjs.end())
	{
		m_renderObjs.erase(it);
	}
	else
	{
		return;
	}
	parentNode->deleteChild(node);
}