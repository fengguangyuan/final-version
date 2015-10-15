#pragma once

#include <wx/wx.h>

class CLED_Object
{
public:
	unsigned int	m_refCount;

	CLED_Object():m_refCount(1)
	{

	}

	virtual ~CLED_Object()
	{

	}
};