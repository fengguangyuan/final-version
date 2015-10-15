#pragma once

#include <wx/file.h>


/*
	这里会导致76Byte的内存泄露，原因在于打开的文件描述符没有在程序退出之前关闭。
	事实上，由于日志文件总是在程序运行期间打开的，其生命周期伴随整个软件运行的过程，因此不认为这是个bug
*/
class CLED_LOG
{
private:
	static wxFile m_logFile;
public:
	static void LogWrite(const wxString& info);
};