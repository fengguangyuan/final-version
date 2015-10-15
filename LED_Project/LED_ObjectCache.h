#pragma once

#include "LED_Object.h"

#include <list>
#include <map>

#define MAX_OBJ_NUMBER 256

class CLRUListNode
{
public:
	wxString		m_name;
	CLED_Object*	m_object;

	CLRUListNode();
	CLRUListNode(const wxString& name,CLED_Object* obj);
	~CLRUListNode();

	bool operator==(const CLRUListNode& r);
};

class CLED_ObjectCache
{
public:
//private:
	unsigned int m_maxObjCount;//�������ɵĶ�����Ŀ
	std::list<CLRUListNode> m_queue;
	std::map<wxString,CLED_Object*> m_cache;
	wxString	m_name;
public:
	CLED_ObjectCache(const wxString& name,unsigned int maxCount = MAX_OBJ_NUMBER);
	~CLED_ObjectCache();
public:
	unsigned int getObjectNumbers();
	unsigned int getMaxObjectNumber();
	CLED_Object* QueryObject(const wxString& name);//�Զ���Ĳ�ѯ�ᵼ�¶������ü����ļ�һ
	void AddObject(const char* name,CLED_Object* obj);
	void DeleteObject(const wxString& name,bool del = false);//del ����˵���Ƿ�����ɾ������Ϊ0�Ķ���ɾ������ᵼ�¶�������ü�����һ
	void ClearCache();
};