#pragma once

#include <wx/file.h>


/*
	����ᵼ��76Byte���ڴ�й¶��ԭ�����ڴ򿪵��ļ�������û���ڳ����˳�֮ǰ�رա�
	��ʵ�ϣ�������־�ļ������ڳ��������ڼ�򿪵ģ����������ڰ�������������еĹ��̣���˲���Ϊ���Ǹ�bug
*/
class CLED_LOG
{
private:
	static wxFile m_logFile;
public:
	static void LogWrite(const wxString& info);
};