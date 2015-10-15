#include "LED_Log.h"

wxFile CLED_LOG::m_logFile(wxString("LED_Designer.log"),wxFile::write);

void CLED_LOG::LogWrite(const wxString& info)
{
	m_logFile.Write(info);
}