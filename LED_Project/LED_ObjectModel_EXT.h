#pragma once

#include "LED_ObjectModel.h"

class CLED_ObjectModel_EXT : public CLED_Object
{
public:
	CLED_ObjectModel_EXT(const wxString& filename);
	CLED_ObjectModel_EXT(){}
	virtual ~CLED_ObjectModel_EXT();
	
	void loadModels();

	inline const wxString getFileName() const { return fileName; }
	friend class CLoad3DS;
	std::vector<CLED_ObjectModel*> models;
private:
	wxString fileName;
};