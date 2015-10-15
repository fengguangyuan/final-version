#include "LED_ObjectCache.h"
#include "LED_Log.h"

CLRUListNode::CLRUListNode()
{

}

CLRUListNode::CLRUListNode(const wxString& name,CLED_Object* obj):m_name(name),m_object(obj)
{

}

CLRUListNode::~CLRUListNode()
{

}

bool CLRUListNode::operator==(const CLRUListNode& r)
{
	return this->m_name.CmpNoCase(r.m_name) == 0;
}

CLED_ObjectCache::CLED_ObjectCache(const wxString& name,unsigned int maxCount /* = MAX_OBJ_NUMBER */):m_maxObjCount(maxCount),m_name(name)
{
	CLED_LOG::LogWrite(wxString("Cache: ")+ name + wxString(" -> Builed '") + name + wxString("' SUCCESSED\n"));
}

CLED_ObjectCache::~CLED_ObjectCache()
{
	ClearCache();
}

unsigned int CLED_ObjectCache::getObjectNumbers()
{
	return m_cache.size();
}

unsigned int CLED_ObjectCache::getMaxObjectNumber()
{
	return m_maxObjCount;
}

void CLED_ObjectCache::AddObject(const char* name,CLED_Object* obj)
{
	if(obj == NULL)
		return;
	//已经达到最大数目
	wxString target;
	if(m_cache.size() == m_maxObjCount)
	{
		std::list<CLRUListNode>::iterator it = m_queue.begin();
		for(;it != m_queue.end(); ++it)
		{
			if((*it).m_object->m_refCount == 0)
			{
				target = (*it).m_name;
				break;
			}
		}
		if(it != m_queue.end())
		{
			std::map<wxString,CLED_Object*>::iterator mit = m_cache.find(it->m_name);
			m_cache.erase(mit);
			m_queue.erase(it);
			CLED_LOG::LogWrite(wxString("Cache: ")+ m_name + wxString(" -> Replace '") + it->m_name + wxString("'\n"));
		}
		else
		{
			m_maxObjCount*=2;
			CLED_LOG::LogWrite(wxString("Cache: ") + m_name + wxString(" -> Doubled Capacity\n"));
		}
	}
	std::pair<wxString,CLED_Object*> p(wxString(name),obj);
	m_cache.insert(p);
	m_queue.push_back(CLRUListNode(wxString(name),obj));
	CLED_LOG::LogWrite(wxString("Cache: ")+ m_name + wxString(" -> Add '") + name + wxString("'\n"));
}

void CLED_ObjectCache::DeleteObject(const wxString& name,bool del /* = false */)
{
	std::map<wxString,CLED_Object*>::iterator it = m_cache.find(name);
	if(it != m_cache.end())
	{
		CLED_Object* objPtr = it->second;
		objPtr->m_refCount--;
		if(objPtr->m_refCount != 0)
			return;
		if(del)
		{
			m_cache.erase(it);
			std::list<CLRUListNode>::iterator lit = std::find(m_queue.begin(),m_queue.end(),CLRUListNode(name,objPtr));
			m_queue.erase(lit);
		}
	}
}

void CLED_ObjectCache::ClearCache()
{
	for (std::map<wxString,CLED_Object*>::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	{
		delete it->second;
	}
	m_cache.clear();
	m_queue.clear();

	CLED_LOG::LogWrite(wxString("Cache: ")+ m_name + wxString(" -> Cleared\n"));
}

CLED_Object* CLED_ObjectCache::QueryObject(const wxString& name)
{
	std::map<wxString,CLED_Object*>::iterator it = m_cache.find(name);
	if(it != m_cache.end())
	{
		CLED_Object* objPtr = it->second;
		objPtr->m_refCount++;

		std::list<CLRUListNode>::iterator lit = std::find(m_queue.begin(),m_queue.end(),CLRUListNode(name,objPtr));
		//从原有队列中删除，并加入到队尾
		m_queue.erase(lit);
		m_queue.push_back(CLRUListNode(name,objPtr));

		CLED_LOG::LogWrite(wxString("Cache: ")+ m_name + wxString(" -> Query '") + name + wxString("' SUCCESSED\n"));

		return objPtr;
	}
	CLED_LOG::LogWrite(wxString("Cache: ")+ m_name + wxString(" -> Query '") + name + wxString("' FAILED\n"));
	return NULL;
}