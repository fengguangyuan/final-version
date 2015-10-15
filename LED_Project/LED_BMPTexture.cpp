#include "LED_BMPTexture.h"
#include "LED_Log.h"

CLED_BMPTexture::CLED_BMPTexture(const wxString& filename):CLED_Texture(0x80E0/* 这里其实就是GL_BGR */)
{
	/*/BMP 文件头 
	struct SBMPFHeader
	{
		unsigned char m_ucType[2];	
		unsigned int m_uiSize;		
		unsigned int m_uiReserved;	
		unsigned int m_uiOffset;	
	};
	//BMP 信息头
	struct SBMPIHeader
	{
		unsigned int m_uiSize;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;	
		unsigned short m_uiPlanes;
		unsigned short m_uiBpp;
		unsigned int m_uiCompression;
		unsigned int m_uiXPPM;
		unsigned int m_uiYPPM;
		unsigned int m_uiClrUsed;
		unsigned int m_uiClrImp;
	};

	CLED_LOG::LogWrite(wxString("Load BMP File '") + filename + wxString("' ...\n"));

	FILE* f = fopen(filename,"rb");
	if(f == NULL)
		CLED_LOG::LogWrite(wxString("Error : Open File '")+ filename + wxString("' Failed\n"));

	fseek(f,0,SEEK_END);
	int fend = ftell(f);
	fseek(f,0,SEEK_SET);
	int fstart = ftell(f);
	int fsize = fend - fstart;

	char* buffer = new char[fsize];
	char* tbuffer_ptr = buffer;

	if(buffer == NULL)
		CLED_LOG::LogWrite(wxString("Error : Load File '") + filename + wxString("' Failed -> Not Enough Memory!\n"));
	if(fread(buffer,1,fsize,f) != fsize)
		CLED_LOG::LogWrite(wxString("Error : Read File '") + filename + wxString("' Failed\n"));
	fclose(f);

	if(buffer[0] != 'B' || buffer [1] != 'M')
		CLED_LOG::LogWrite(wxString("Error : File '") + filename + wxString("' Is NOT A .BMP File!\n"));

	SBMPFHeader* fheader = (SBMPFHeader*)buffer;
	tbuffer_ptr += 14;
	SBMPIHeader* iheader = (SBMPIHeader*)tbuffer_ptr;
	tbuffer_ptr += 40;
	if(iheader->m_uiBpp != 24 || iheader->m_uiHeight == 0 || iheader->m_uiWidth == 0)
		CLED_LOG::LogWrite(wxString("Error : BMP File '") + filename + wxString("' Does Not 24 Bits/Pixel Format\n"));
	m_size.x = iheader->m_uiWidth;
	m_size.y = iheader->m_uiHeight;

	unsigned int texDataSize = iheader->m_uiHeight*iheader->m_uiWidth*3;
	m_data = new char[texDataSize];
	if(m_data == NULL)
		CLED_LOG::LogWrite(wxString("Error : Copy File '") + filename + wxString("' BMP Data Failed -> Not Enough Memory!\n"));
	memcpy(m_data,tbuffer_ptr,texDataSize);
	delete[] buffer;

	glGenTextures(1,&m_texID);

	glBindTexture(GL_TEXTURE_2D,m_texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_size.x, m_size.y, 0, GL_BGR, GL_UNSIGNED_BYTE, m_data);
	*/
	HBITMAP hBMP;
	BITMAP BMP;
	glGenTextures(1,&m_texID);
	hBMP = (HBITMAP)LoadImageA(GetModuleHandle(NULL),filename.ToStdString().c_str(),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	if(!hBMP)
		CLED_LOG::LogWrite(wxString("Error : Open File '") + filename + wxString("' Failed!\n"));
	
	GetObject(hBMP,sizeof(BMP),&BMP);

	m_size.x = BMP.bmWidth;
	m_size.y = BMP.bmHeight;
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBindTexture(GL_TEXTURE_2D, m_texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);  //Delete The Object
}

CLED_BMPTexture::~CLED_BMPTexture()
{

}