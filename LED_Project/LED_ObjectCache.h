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
	unsigned int m_maxObjCount;//最大可容纳的对象数目
	std::list<CLRUListNode> m_queue;
	std::map<wxString,CLED_Object*> m_cache;
	wxString	m_name;
public:
	CLED_ObjectCache(const wxString& name,unsigned int maxCount = MAX_OBJ_NUMBER);
	~CLED_ObjectCache();
public:
	unsigned int getObjectNumbers();
	unsigned int getMaxObjectNumber();
	CLED_Object* QueryObject(const wxString& name);//对对象的查询会导致对象引用计数的加一
	void AddObject(const char* name,CLED_Object* obj);
	void DeleteObject(const wxString& name,bool del = false);//del 用来说明是否立即删除引用为0的对象，删除对象会导致对象的引用计数减一
	void ClearCache();
};