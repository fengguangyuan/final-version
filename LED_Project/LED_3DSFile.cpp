// 3DS.cpp: implementation of the 3DS class.
//
//////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "LED_3DSFile.h"
#include "LED_ObjectModel_EXT.h"

GLuint CLoad3DS::CountModels = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ���캯���Ĺ����ǳ�ʼ��tChunk����
CLoad3DS::CLoad3DS()
{
	Model3DS.Textured = 1;
	Model3DS.numOfObjects = 0;
	Model3DS.numOfMaterials = 0;
	m_FilePointer = NULL;
	m_CurrentChunk = new tChunk();      // ��ʼ����Ϊ��ǰ�Ŀ����ռ�
    m_TempChunk = new tChunk();          // ��ʼ��һ����ʱ�鲢����ռ�

  	ModelPosition.set(0,0,0);
    ModelRotation.set(0,0,0);
	ModelSize = 1.0;

    Model_ID = 0;
	++CountModels;
}

CLoad3DS::~CLoad3DS()
{	
	CleanUp();// �ͷ��ڴ�ռ�
	for(int i = 0; i < Model3DS.numOfObjects; i++)
	{	delete [] Model3DS.pObject[i].pFaces;// ɾ�����еı���
		delete [] Model3DS.pObject[i].pNormals;
		delete [] Model3DS.pObject[i].pVerts;
		delete [] Model3DS.pObject[i].pTexVerts;
	}
	//delete [] &Model3DS;
	    --CountModels;
}

// ����ĺ����ͷ����е��ڴ�ռ䣬���ر��ļ�
void CLoad3DS::CleanUp()
{
  fclose(m_FilePointer);            // �رյ�ǰ���ļ�ָ��
  delete m_CurrentChunk;            // �ͷŵ�ǰ��
  delete m_TempChunk;              // �ͷ���ʱ��
}

// ����һ������
int CLoad3DS::BuildTexture(char *szPathName, GLuint &texid)
{

  HDC      hdcTemp;                        // The DC To Hold Our Bitmap
  HBITMAP    hbmpTemp;                        // Holds The Bitmap Temporarily
  IPicture  *pPicture;                        // IPicture Interface
  OLECHAR    wszPath[MAX_PATH+1];                  // Full Path To Picture (WCHAR)
  char    szPath[MAX_PATH+1];                    // Full Path To Picture
  long    lWidth;                          // Width In Logical Units
  long    lHeight;                        // Height In Logical Units
  long    lWidthPixels;                      // Width In Pixels
  long    lHeightPixels;                      // Height In Pixels
  GLint    glMaxTexDim ;                      // Holds Maximum Texture Size

  if (strstr(szPathName, "http://"))                  // If PathName Contains http:// Then...
  {
    strcpy(szPath, szPathName);                    // Append The PathName To szPath
  }
  else                                // Otherwise... We Are Loading From A File
  {
    GetCurrentDirectory(MAX_PATH, (LPWSTR)szPath);              // Get Our Working Directory
    strcat(szPath, "\\3DS\\");                      // Append "\" After The Working Directory
    strcat(szPath, szPathName);                    // Append The PathName
  }

  MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);    // Convert From ASCII To Unicode
  HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

  if(FAILED(hr))                            // If Loading Failed
    return FALSE;                          // Return False

  hdcTemp = CreateCompatibleDC(GetDC(0));                // Create The Windows Compatible Device Context
  if(!hdcTemp)                            // Did Creation Fail?
  {
    pPicture->Release();                      // Decrements IPicture Reference Count
    return FALSE;                          // Return False (Failure)
  }

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);          // Get Maximum Texture Size Supported
  
  pPicture->get_Width(&lWidth);                    // Get IPicture Width (Convert To Pixels)
  lWidthPixels  = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
  pPicture->get_Height(&lHeight);                    // Get IPicture Height (Convert To Pixels)
  lHeightPixels  = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

  // Resize Image To Closest Power Of Two
  if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
    lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
  else // Otherwise Set Width To "Max Power Of Two" That The Card Can Handle
    lWidthPixels = glMaxTexDim;
 
  if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
    lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
  else // Otherwise Set Height To "Max Power Of Two" That The Card Can Handle
    lHeightPixels = glMaxTexDim;
  
  //  Create A Temporary Bitmap
  BITMAPINFO  bi = {0};                        // The Type Of Bitmap We Request
  DWORD    *pBits = 0;                        // Pointer To The Bitmap Bits

  bi.bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);        // Set Structure Size
  bi.bmiHeader.biBitCount    = 32;                  // 32 Bit
  bi.bmiHeader.biWidth    = lWidthPixels;              // Power Of Two Width
  bi.bmiHeader.biHeight    = lHeightPixels;            // Make Image Top Up (Positive Y-Axis)
  bi.bmiHeader.biCompression  = BI_RGB;                // RGB Encoding
  bi.bmiHeader.biPlanes    = 1;                  // 1 Bitplane

  //  Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
  hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
  
  if(!hbmpTemp)                            // Did Creation Fail?
  {
    DeleteDC(hdcTemp);                        // Delete The Device Context
    pPicture->Release();                      // Decrements IPicture Reference Count
    return FALSE;                          // Return False (Failure)
  }

  SelectObject(hdcTemp, hbmpTemp);                  // Select Handle To Our Temp DC And Our Temp Bitmap Object

  // Render The IPicture On To The Bitmap
  pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

  // Convert From BGR To RGB Format And Add An Alpha Value Of 255
  for(long i = 0; i < lWidthPixels * lHeightPixels; i++)        // Loop Through All Of The Pixels
  {
    BYTE* pPixel  = (BYTE*)(&pBits[i]);              // Grab The Current Pixel
    BYTE temp    = pPixel[0];                  // Store 1st Color In Temp Variable (Blue)
    pPixel[0]    = pPixel[2];                  // Move Red Value To Correct Position (1st)
    pPixel[2]    = temp;                      // Move Temp Value To Correct Blue Position (3rd)

    // This Will Make Any Black Pixels, Completely Transparent    (You Can Hardcode The Value If You Wish)
    if ((pPixel[0]==0) && (pPixel[1]==0) && (pPixel[2]==0))      // Is Pixel Completely Black
      pPixel[3]  = 0;                      // Set The Alpha Value To 0
    else                              // Otherwise
      pPixel[3]  = 255;                      // Set The Alpha Value To 255
  }

  glGenTextures(1, &texid);                      // Create The Texture

  // Typical Texture Generation Using Data From The Bitmap
  glBindTexture(GL_TEXTURE_2D, texid);                // Bind To The Texture ID
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    // (Modify This For The Type Of Filtering You Want)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // (Modify This For The Type Of Filtering You Want)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );    // (Modify This For The Type Of Filtering You Want)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR ); // (Modify This For The Type Of Filtering You Want)
 
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);  // (Modify This If You Want Mipmaps)
  
  DeleteObject(hbmpTemp);                        // Delete The Object
  DeleteDC(hdcTemp);                          // Delete The Device Context

  pPicture->Release();                        // Decrements IPicture Reference Count

  return TRUE;                            // Return True (All Good)

}


// ��һ��3ds�ļ����������е����ݣ����ͷ��ڴ�
bool CLoad3DS::ImportModel(GLuint Model_id, char *strFileName)
{
	Model_ID = Model_id;
	fileName = strFileName;

	char strMessage[80] = {0};
	m_FilePointer = fopen(strFileName, "rb");
  // ȷ������õ��ļ�ָ��Ϸ�
  if(!m_FilePointer) 
  {
    sprintf(strMessage, "�޷��ҵ��ļ�: %s!", strFileName);
	MessageBox(NULL, (LPCWSTR)strMessage, LPCWSTR("Error"), MB_OK);
    return false;
  }
  // ���ļ���֮������Ӧ�ý��ļ��ʼ�����ݿ�������ж��Ƿ���һ��3ds�ļ�
  // �����3ds�ļ��Ļ�����һ����IDӦ����PRIMARY

  // ���ļ��ĵ�һ��������ж��Ƿ���3ds�ļ�
  ReadChunk(m_CurrentChunk);

  // ȷ����3ds�ļ�
  if (m_CurrentChunk->ID != PRIMARY)
  {
    sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
	MessageBox(NULL, (LPCWSTR)strMessage, LPCWSTR("Error"), MB_OK);
    return false;
  }

  // ���ڿ�ʼ�������ݣ�ProcessNextChunk()��һ���ݹ麯��

  // ͨ����������ĵݹ麯�������������
  ProcessNextChunk(&Model3DS, m_CurrentChunk);

  // �ڶ�������3ds�ļ�֮�󣬼��㶥��ķ���
  ComputeNormals(&Model3DS);

   	Model3DS.Textured = true;

	for(int i =0; i<Model3DS.numOfMaterials;i++)
	{	
		Model3DS.pMaterials[i].texureId = i;// ���ò��ʵ�����ID  ///****�ɳ�////
	}

  // �ͷ��ڴ�ռ�
//  CleanUp();            

 
  return true;
}

// ���溯��������ID�ź������ֽڳ���
void CLoad3DS::ReadChunk(tChunk *pChunk)
{
  // ������ID�ţ�ռ����2���ֽڡ����ID����OBJECT��MATERIALһ����˵�����ڿ���������������
  pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

  // Ȼ������ռ�õĳ��ȣ��������ĸ��ֽ�
  pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

// ����ĺ�������3ds�ļ�����Ҫ����
void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
  t3DObject newObject = {0};          // �������ӵ���������
  tMaterialInfo newTexture = {0};        // �������ӵ���������
  unsigned int version = 0;          // �����ļ��汾
  int buffer[50000] = {0};          // ������������Ҫ������

  m_CurrentChunk = new tChunk;        // Ϊ�µĿ����ռ�    

  // ����ÿ��һ���¿飬��Ҫ�ж�һ�¿��ID������ÿ�����Ҫ�Ķ���ģ����������
  // ����ǲ���Ҫ����Ŀ飬���Թ�

  // ���������ӿ飬ֱ���ﵽԤ���ĳ���
  while (pPreviousChunk->bytesRead < pPreviousChunk->length)
  {
    // ������һ����
    ReadChunk(m_CurrentChunk);

    // �жϿ��ID��
    switch (m_CurrentChunk->ID)
    {
    case VERSION:              // �ļ��汾��
      
      // �ڸÿ�����һ���޷��Ŷ��������������ļ��İ汾
      
      // �����ļ��İ汾�ţ������ֽ������ӵ�bytesRead������
      m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);

      // ����ļ��汾�Ŵ���3������һ��������Ϣ
      if (version > 0x03)
		  MessageBox(NULL, LPCWSTR("This 3DS file is over version 3 so it may load incorrectly"), LPCWSTR("Warning"), MB_OK);
      break;

    case OBJECTINFO:            // ����汾��Ϣ
      
      // ������һ����
      ReadChunk(m_TempChunk);

      // �������İ汾��
      m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

      // ���Ӷ�����ֽ���
      m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;

      // ������һ����
      ProcessNextChunk(pModel, m_CurrentChunk);
      break;

    case MATERIAL:              // ������Ϣ

      // ���ʵ���Ŀ����
      pModel->numOfMaterials++;

      // ����������������һ���հ������ṹ
      pModel->pMaterials.push_back(newTexture);

      // �������װ�뺯��
      ProcessNextMaterialChunk(pModel, m_CurrentChunk);
      break;

    case OBJECT:              // ���������
        
      // �ÿ��Ƕ�����Ϣ���ͷ���������˶���������

      // ����������
      pModel->numOfObjects++;
    
      // ����һ���µ�tObject�ڵ㵽����������
      pModel->pObject.push_back(newObject);
      
      // ��ʼ������������������ݳ�Ա
      memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

      // ��ò������������ƣ�Ȼ�����Ӷ�����ֽ���
      m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
      
      // ��������Ķ�����Ϣ�Ķ���
      ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
      break;

    case EDITKEYFRAME:

      // �����ؼ�֡��Ķ��룬������Ҫ������ֽ���
      m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;

    default: 
      
      // �������к��ԵĿ�����ݵĶ��룬������Ҫ������ֽ���
      m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;
    }

    // ���Ӵ����������ֽ���
    pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
  }

  // �ͷŵ�ǰ����ڴ�ռ�
  delete m_CurrentChunk;
  m_CurrentChunk = pPreviousChunk;
}

// ����ĺ����������е��ļ��ж������Ϣ
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
  int buffer[50000] = {0};          // ���ڶ��벻��Ҫ������

  // ���µĿ����洢�ռ�
  m_CurrentChunk = new tChunk;

  // ��������������ֱ�����ӿ����
  while (pPreviousChunk->bytesRead < pPreviousChunk->length)
  {
    // ������һ����
    ReadChunk(m_CurrentChunk);

    // ������������ֿ�
    switch (m_CurrentChunk->ID)
    {
    case OBJECT_MESH:          // ���������һ���¿�
    
      // ʹ�õݹ麯�����ã��������¿�
      ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
      break;

    case OBJECT_VERTICES:        // �����Ƕ��󶥵�
      ReadVertices(pObject, m_CurrentChunk);
      break;

    case OBJECT_FACES:          // ������Ƕ������
      ReadVertexIndices(pObject, m_CurrentChunk);
      break;

    case OBJECT_MATERIAL:        // ������Ƕ���Ĳ�������
      
      // �ÿ鱣���˶�����ʵ����ƣ�������һ����ɫ��Ҳ������һ������ӳ�䡣ͬʱ�ڸÿ���Ҳ������
      // �����������������

      // ����������Ĳ�������
      ReadObjectMaterial(pModel, pObject, m_CurrentChunk);      
      break;

    case OBJECT_UV:            // ��������UV��������

      // ��������UV��������
      ReadUVCoordinates(pObject, m_CurrentChunk);
      break;

    default: 

      // �Թ�����Ҫ����Ŀ�
      m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;
    }

    // ���Ӵ������ж�����ֽ�����ǰ��Ķ�����ֽ���
    pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
  }

  // �ͷŵ�ǰ����ڴ�ռ䣬���ѵ�ǰ������Ϊǰ���
  delete m_CurrentChunk;
  m_CurrentChunk = pPreviousChunk;
}

// ����ĺ����������еĲ�����Ϣ
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
  int buffer[50000] = {0};          // ���ڶ��벻��Ҫ������

  // ����ǰ�����洢�ռ�
  m_CurrentChunk = new tChunk;

  // ����������Щ�飬֪�����ӿ����
  while (pPreviousChunk->bytesRead < pPreviousChunk->length)
  {
    // ������һ��
    ReadChunk(m_CurrentChunk);

    // �ж϶������ʲô��
    switch (m_CurrentChunk->ID)
    {
    case MATNAME:              // ���ʵ�����
      
      // ������ʵ�����
      m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;

    case MATDIFFUSE:            // �����R G B��ɫ
      ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
      break;
    
    case MATMAP:              // ������Ϣ��ͷ��
      
      // ������һ�����ʿ���Ϣ
      ProcessNextMaterialChunk(pModel, m_CurrentChunk);
      break;

    case MATMAPFILE:            // �����ļ�������

      // ������ʵ��ļ�����
      m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;
    
    default: 

      // �ӹ�����Ҫ����Ŀ�
      m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
      break;
    }

    // ���Ӵ������ж�����ֽ���
    pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
  }

  // ɾ����ǰ�飬����ǰ��Ŀ�����Ϊ��ǰ��
  delete m_CurrentChunk;
  m_CurrentChunk = pPreviousChunk;
}



// ����ĺ�������һ���ַ���
int CLoad3DS::GetString(char *pBuffer)
{
  int index = 0;

  // ����һ���ֽڵ�����
  fread(pBuffer, 1, 1, m_FilePointer);

  // ֱ������
  while (*(pBuffer + index++) != 0) {

    // ����һ���ַ�ֱ��NULL
    fread(pBuffer + index, 1, 1, m_FilePointer);
  }

  // �����ַ����ĳ���
  return strlen(pBuffer) + 1;
}

// ����ĺ�������RGB��ɫ
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
  // ������ɫ����Ϣ
  ReadChunk(m_TempChunk);

  // ����RGB��ɫ
  m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

  // ���Ӷ�����ֽ���
  pChunk->bytesRead += m_TempChunk->bytesRead;
}

// ����ĺ������붥������
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
  unsigned short index = 0;          // ���ڶ��뵱ǰ�������

  // ����ö����������Ŀ
  pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

  // ����������Ĵ洢�ռ䣬����ʼ���ṹ
  pObject->pFaces = new tFace [pObject->numOfFaces];
  memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

  // �������������е���
  for(int i = 0; i < pObject->numOfFaces; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      // ���뵱ǰ��ĵ�һ���� 
      pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

      if(j < 3)
      {
        // ��������������Ľṹ��
        pObject->pFaces[i].vertIndex[j] = index;
      }
    }
  }

}

//////////////////////////////////////////////////////





// ����ĺ�����������UV����
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
  // Ϊ�˶�������UV���꣬������Ҫ����UV�����������Ȼ��Ŷ�����������

  // ����UV���������
  pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

  // ���䱣��UV������ڴ�ռ�
  pObject->pTexVerts = new CVector2D [pObject->numTexVertex];

  // ������������
  pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

// �������Ķ���
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
  // �ڶ���ʵ�ʵĶ���֮ǰ�����ȱ���ȷ����Ҫ������ٸ����㡣
  
  // ���붥�����Ŀ
  pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

  // ���䶥��Ĵ洢�ռ䣬Ȼ���ʼ���ṹ��
  pObject->pVerts = new CVector3D [pObject->numOfVerts];
  memset(pObject->pVerts, 0, sizeof(CVector3D) * pObject->numOfVerts);

  // ���붥������
  pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

  // �����Ѿ����������еĶ��㡣
  // ��Ϊ3D Studio Max��ģ�͵�Z����ָ���ϵģ������Ҫ��y���z�ᷭת������
  // ����������ǽ�Y���Z�ύ����Ȼ��Z�ᷴ��

  // �������еĶ���
  for(int i = 0; i < pObject->numOfVerts; i++)
  {
    // ����Y���ֵ
    float fTempY = pObject->pVerts[i].y;

    // ����Y���ֵ����Z���ֵ
    pObject->pVerts[i].y = pObject->pVerts[i].z;

    // ����Z���ֵ����-Y���ֵ 
    pObject->pVerts[i].z = -fTempY;
  }
}

// ����ĺ����������Ĳ�������
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
  char strMaterial[255] = {0};      // �����������Ĳ�������
  int buffer[50000] = {0};        // �������벻��Ҫ������

  // ���ʻ�������ɫ�������Ƕ����������Ҳ���ܱ������������ȡ�����ȵ���Ϣ��

  // ������븳�赱ǰ����Ĳ�������
  pPreviousChunk->bytesRead += GetString(strMaterial);

  // �������е�����
  for(int i = 0; i < pModel->numOfMaterials; i++)
  {
    //�������������뵱ǰ����������ƥ��
    if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
    {
      // ���ò���ID
      pObject->materialID = i;

      // �ж��Ƿ�������ӳ�䣬���strFile��һ�����ȴ���1���ַ�������������
      if(strlen(pModel->pMaterials[i].strFile) > 0) {
        //��������
        BuildTexture(pModel->pMaterials[i].strFile, pModel->texture[pObject->materialID]);   /* *******////
		// ���ö��������ӳ���־
        pObject->bHasTexture = true;
      }  
      break;
    }
    else
    {
      // ����ö���û�в��ʣ�������IDΪ-1
      pObject->materialID = -1;
    }
  }

  pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}      

// �������Щ������Ҫ�������㶥��ķ�����������ķ�������Ҫ�����������
// ����ĺ궨�����һ��ʸ���ĳ���
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// ����ĺ��������������ʸ��
CVector3D Vector(CVector3D vPoint1, CVector3D vPoint2)
{
  CVector3D vVector;              

  vVector.x = vPoint1.x - vPoint2.x;      
  vVector.y = vPoint1.y - vPoint2.y;      
  vVector.z = vPoint1.z - vPoint2.z;      

  return vVector;                
}

// ����ĺ�������ʸ�����
CVector3D AddVector(CVector3D vVector1, CVector3D vVector2)
{
  CVector3D vResult;              
  
  vResult.x = vVector2.x + vVector1.x;    
  vResult.y = vVector2.y + vVector1.y;    
  vResult.z = vVector2.z + vVector1.z;    

  return vResult;                
}

// ����ĺ�������ʸ��������
CVector3D DivideVectorByScaler(CVector3D vVector1, float Scaler)
{
  CVector3D vResult;              
  
  vResult.x = vVector1.x / Scaler;      
  vResult.y = vVector1.y / Scaler;      
  vResult.z = vVector1.z / Scaler;      

  return vResult;                
}

// ����ĺ�����������ʸ���Ĳ��
CVector3D Cross(CVector3D vVector1, CVector3D vVector2)
{
  CVector3D vCross;                
                        
  vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
                        
  vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
                        
  vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

  return vCross;                
}

// ����ĺ����淶��ʸ��
CVector3D Normalize(CVector3D vNormal)
{
  double Magnitude;              

  Magnitude = Mag(vNormal);          // ���ʸ���ĳ���

  vNormal.x /= (float)Magnitude;        
  vNormal.y /= (float)Magnitude;        
  vNormal.z /= (float)Magnitude;        

  return vNormal;                
}

// ����ĺ������ڼ������ķ�����
void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
  CVector3D vVector1, vVector2, vNormal, vPoly[3];

  // ���ģ����û�ж����򷵻�
  if(pModel->numOfObjects <= 0)
    return;

  // ����ģ�������еĶ���
  for(int index = 0; index < pModel->numOfObjects; index++)
  {
    // ��õ�ǰ�Ķ���
    t3DObject *pObject = &(pModel->pObject[index]);

    // ������Ҫ�Ĵ洢�ռ�
    CVector3D *pNormals    = new CVector3D [pObject->numOfFaces];
    CVector3D *pTempNormals  = new CVector3D [pObject->numOfFaces];
    pObject->pNormals    = new CVector3D [pObject->numOfVerts];

    // ���������������
    for(int i=0; i < pObject->numOfFaces; i++)
    {                        
      vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
      vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
      vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

      // ������ķ�����

      vVector1 = Vector(vPoly[0], vPoly[2]);    // ��ö���ε�ʸ��
      vVector2 = Vector(vPoly[2], vPoly[1]);    // ��ö���εĵڶ���ʸ��

      vNormal = Cross(vVector1, vVector2);    // �������ʸ���Ĳ��
      pTempNormals[i] = vNormal;          // ����ǹ淶��������
      vNormal = Normalize(vNormal);        // �淶����õĲ��

      pNormals[i] = vNormal;            // �����������ӵ��������б���
    }

    // �����󶥵㷨����
    CVector3D vSum = {0.0, 0.0, 0.0};
    CVector3D vZero = vSum;
    int shared=0;
    // �������еĶ���
    for (int i = 0; i < pObject->numOfVerts; i++)      
    {
      for (int j = 0; j < pObject->numOfFaces; j++)  // �������е���������
      {                        // �жϸõ��Ƿ����������湲��
        if (pObject->pFaces[j].vertIndex[0] == i || 
          pObject->pFaces[j].vertIndex[1] == i || 
          pObject->pFaces[j].vertIndex[2] == i)
        {
          vSum = AddVector(vSum, pTempNormals[j]);
          shared++;                
        }
      } 
      
      pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));
      // �淶�����Ķ��㷨��
      pObject->pNormals[i] = Normalize(pObject->pNormals[i]);  

      vSum = vZero;                
      shared = 0;                    
    }
  
    // �ͷŴ洢�ռ䣬��ʼ��һ������
    delete [] pTempNormals;
    delete [] pNormals;
  }
}	

///////////////////////////////////////////////////����ģ��
void CLoad3DS::DrawModel()
{
  // ����ģ�������еĶ���
  for(int i = 0; i < Model3DS.numOfObjects; i++) 
  {
    // �������Ĵ�СС��0�����˳�
    if(Model3DS.pObject.size() <= 0) break;

    // ��õ�ǰ��ʾ�Ķ���
    t3DObject *pObject = &Model3DS.pObject[i];
      
    // �жϸö����Ƿ�������ӳ��
    if(pObject->bHasTexture && Model3DS.Textured==true) {  

      // ������ӳ��
      glEnable(GL_TEXTURE_2D);
	  glColor3ub(255, 255, 255);
      glBindTexture(GL_TEXTURE_2D, Model3DS.texture[pObject->materialID]);
    } else {

      // �ر�����ӳ��
      glDisable(GL_TEXTURE_2D);
	  glColor3ub(255, 255, 255);
    }
    // ��ʼ��GL_TRIANGLESģʽ����
    glBegin(GL_TRIANGLES);          
      // �������е���
    for(int j = 0; j < pObject->numOfFaces; j++)
      {
        // ���������ε����е�
        for(int whichVertex = 0; whichVertex < 3; whichVertex++)
        {
            // ������ÿ���������
          int index = pObject->pFaces[j].vertIndex[whichVertex];
      
          // ����������
          glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
        
          // ��������������
          if(pObject->bHasTexture) {

            // ȷ���Ƿ���UVW��������
            if(pObject->pTexVerts) {
              glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
            }
          } else {

            if(Model3DS.pMaterials.size() && pObject->materialID >= 0) 
            {
              BYTE *pColor = Model3DS.pMaterials[pObject->materialID].color;
              glColor3ub(pColor[0], pColor[1], pColor[2]);
            }
          }
         glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
        }
    }

    glEnd();                // ���ƽ���
  }

}

void CLoad3DS::RenderModel()
{
	glPushAttrib(GL_CURRENT_BIT);//������������
  	glPushMatrix();

	glTranslatef(ModelPosition.x, ModelPosition.y, ModelPosition.z);
	glScalef(ModelSize,ModelSize,ModelSize);
    glRotatef(1, 1, 0, 0);
	glRotatef(ModelRotation.x,1,0,0);
	glRotatef(ModelRotation.z,0,0,1);
	glRotatef(ModelRotation.y,0,1,0);

	DrawModel();

  	glPopMatrix();
	glPopAttrib();          //�ָ�ǰһ����

}
/***************************** ת��Ϊ��׼ģ�� *******************************************/
CLED_ObjectModel_EXT* CLoad3DS::transStdModel()
{
	CLED_ObjectModel_EXT *stdModels = new CLED_ObjectModel_EXT();
	stdModels->fileName = fileName;

	for (int i = 0; i < Model3DS.numOfObjects; i++)
	{
		vector<t3DObject> &objs = Model3DS.pObject;
		for (auto it : objs)
		{
			CLED_ObjectModel *new_model = new CLED_ObjectModel();
			stdModels->models.push_back(new_model);

			new_model->objName = it.strName;
			new_model->m_faceCount = it.numOfFaces;
			new_model->obj.planes_n = it.numOfFaces;
			new_model->obj.points_n = it.numOfVerts;
			//���ø�������Ĵ�С�Դ������
			//new_model->m_pos.resize(3 * it.numOfVerts);
			//new_model->m_normals.resize(3 * it.numOfVerts);
			//new_model->m_UVs.resize(3 * it.numTexVertex);
			new_model->obj.points.resize(it.numOfVerts);
			for (int k = 0; k < it.numOfFaces; k++)
			{
				new_model->obj.planes[k].p[0] = it.pFaces[k].vertIndex[0];
				new_model->obj.planes[k].p[1] = it.pFaces[k].vertIndex[1];
				new_model->obj.planes[k].p[2] = it.pFaces[k].vertIndex[2];
				//����ÿ�����ϵ�ÿ������ķ�������Ϣ
				new_model->obj.planes[k].normals[0].x = it.pNormals[it.pFaces[k].vertIndex[0]].x;
				new_model->obj.planes[k].normals[0].y = it.pNormals[it.pFaces[k].vertIndex[0]].y;
				new_model->obj.planes[k].normals[0].z = it.pNormals[it.pFaces[k].vertIndex[0]].z;

				new_model->obj.planes[k].normals[1].x = it.pNormals[it.pFaces[k].vertIndex[1]].x;
				new_model->obj.planes[k].normals[1].y = it.pNormals[it.pFaces[k].vertIndex[1]].y;
				new_model->obj.planes[k].normals[1].z = it.pNormals[it.pFaces[k].vertIndex[1]].z;

				new_model->obj.planes[k].normals[2].x = it.pNormals[it.pFaces[k].vertIndex[2]].x;
				new_model->obj.planes[k].normals[2].y = it.pNormals[it.pFaces[k].vertIndex[2]].y;
				new_model->obj.planes[k].normals[2].z = it.pNormals[it.pFaces[k].vertIndex[2]].z;

				//����ÿ����Ķ�����Ϣ����׼ģ����
				glm::vec3 pos;
				pos.x = it.pVerts[it.pFaces[k].vertIndex[0]].x;
				pos.y = it.pVerts[it.pFaces[k].vertIndex[0]].y;
				pos.z = it.pVerts[it.pFaces[k].vertIndex[0]].z;
				new_model->m_pos.push_back(pos);
				//new_model->m_pos[it.pFaces[k].vertIndex[0]].x = it.pVerts[it.pFaces[k].vertIndex[0]].x;
				//new_model->m_pos[it.pFaces[k].vertIndex[0]].y = it.pVerts[it.pFaces[k].vertIndex[0]].y;
				//new_model->m_pos[it.pFaces[k].vertIndex[0]].z = it.pVerts[it.pFaces[k].vertIndex[0]].z;
				pos.x = it.pVerts[it.pFaces[k].vertIndex[1]].x;
				pos.y = it.pVerts[it.pFaces[k].vertIndex[1]].y;
				pos.z = it.pVerts[it.pFaces[k].vertIndex[1]].z;
				new_model->m_pos.push_back(pos);
				//new_model->m_pos[it.pFaces[k].vertIndex[1]].x = it.pVerts[it.pFaces[k].vertIndex[1]].x;
				//new_model->m_pos[it.pFaces[k].vertIndex[1]].y = it.pVerts[it.pFaces[k].vertIndex[1]].y;
				//new_model->m_pos[it.pFaces[k].vertIndex[1]].z = it.pVerts[it.pFaces[k].vertIndex[1]].z;
				pos.x = it.pVerts[it.pFaces[k].vertIndex[2]].x;
				pos.y = it.pVerts[it.pFaces[k].vertIndex[2]].y;
				pos.z = it.pVerts[it.pFaces[k].vertIndex[2]].z;
				new_model->m_pos.push_back(pos);
				//new_model->m_pos[it.pFaces[k].vertIndex[2]].x = it.pVerts[it.pFaces[k].vertIndex[2]].x;
				//new_model->m_pos[it.pFaces[k].vertIndex[2]].y = it.pVerts[it.pFaces[k].vertIndex[2]].y;
				//new_model->m_pos[it.pFaces[k].vertIndex[2]].z = it.pVerts[it.pFaces[k].vertIndex[2]].z;
				//����ÿ�����ϵ�ÿ����ķ�����
				pos.x = it.pNormals[it.pFaces[k].vertIndex[0]].x;
				pos.y = it.pNormals[it.pFaces[k].vertIndex[0]].y;
				pos.z = it.pNormals[it.pFaces[k].vertIndex[0]].z;
				new_model->m_normals.push_back(pos);
				//new_model->m_normals[it.pFaces[k].vertIndex[0]].x = it.pNormals[it.pFaces[k].vertIndex[0]].x;
				//new_model->m_normals[it.pFaces[k].vertIndex[0]].y = it.pNormals[it.pFaces[k].vertIndex[0]].y;
				//new_model->m_normals[it.pFaces[k].vertIndex[0]].z = it.pNormals[it.pFaces[k].vertIndex[0]].z;
				pos.x = it.pNormals[it.pFaces[k].vertIndex[1]].x;
				pos.y = it.pNormals[it.pFaces[k].vertIndex[1]].y;
				pos.z = it.pNormals[it.pFaces[k].vertIndex[1]].z;
				new_model->m_normals.push_back(pos);
				//new_model->m_normals[it.pFaces[k].vertIndex[1]].x = it.pNormals[it.pFaces[k].vertIndex[1]].x;
				//new_model->m_normals[it.pFaces[k].vertIndex[1]].y = it.pNormals[it.pFaces[k].vertIndex[1]].y;
				//new_model->m_normals[it.pFaces[k].vertIndex[1]].z = it.pNormals[it.pFaces[k].vertIndex[1]].z;
				pos.x = it.pNormals[it.pFaces[k].vertIndex[2]].x;
				pos.y = it.pNormals[it.pFaces[k].vertIndex[2]].y;
				pos.z = it.pNormals[it.pFaces[k].vertIndex[2]].z;
				new_model->m_normals.push_back(pos);
				//new_model->m_normals[it.pFaces[k].vertIndex[2]].x = it.pNormals[it.pFaces[k].vertIndex[2]].x;
				//new_model->m_normals[it.pFaces[k].vertIndex[2]].y = it.pNormals[it.pFaces[k].vertIndex[2]].y;
				//new_model->m_normals[it.pFaces[k].vertIndex[2]].z = it.pNormals[it.pFaces[k].vertIndex[2]].z;

				//����������Ϣ
				glm::vec2 uv;
				uv.x = it.pTexVerts[it.pFaces[k].coordIndex[0]].x; 
				uv.y = it.pTexVerts[it.pFaces[k].coordIndex[0]].y;
				new_model->m_UVs.push_back(uv);
				//new_model->m_UVs[it.pFaces[k].coordIndex[0]].x = it.pTexVerts[it.pFaces[k].coordIndex[0]].x;
				//new_model->m_UVs[it.pFaces[k].coordIndex[0]].y = it.pTexVerts[it.pFaces[k].coordIndex[0]].y;
				uv.x = it.pTexVerts[it.pFaces[k].coordIndex[1]].x;
				uv.y = it.pTexVerts[it.pFaces[k].coordIndex[1]].y;
				new_model->m_UVs.push_back(uv);
				//new_model->m_UVs[it.pFaces[k].coordIndex[1]].x = it.pTexVerts[it.pFaces[k].coordIndex[1]].x;
				//new_model->m_UVs[it.pFaces[k].coordIndex[1]].y = it.pTexVerts[it.pFaces[k].coordIndex[1]].y;
				uv.x = it.pTexVerts[it.pFaces[k].coordIndex[2]].x;
				uv.y = it.pTexVerts[it.pFaces[k].coordIndex[2]].y;
				new_model->m_UVs.push_back(uv);
				//new_model->m_UVs[it.pFaces[k].coordIndex[2]].x = it.pTexVerts[it.pFaces[k].coordIndex[2]].x;
				//new_model->m_UVs[it.pFaces[k].coordIndex[2]].y = it.pTexVerts[it.pFaces[k].coordIndex[2]].y;
				//����ÿ�����ϵĶ��㡢����������������ֵ
				new_model->m_faceCoors.push_back(glm::vec3(it.pFaces[k].vertIndex[0],
					it.pFaces[k].coordIndex[0], it.pFaces[k].vertIndex[0]));
				new_model->m_faceCoors.push_back(glm::vec3(it.pFaces[k].vertIndex[1],
					it.pFaces[k].coordIndex[1], it.pFaces[k].vertIndex[1]));
				new_model->m_faceCoors.push_back(glm::vec3(it.pFaces[k].vertIndex[2],
					it.pFaces[k].coordIndex[2], it.pFaces[k].vertIndex[2]));
				//ÿ������ڽ�����Ϊ��
				new_model->obj.planes[k].adjFace_index[0] = 0;
				new_model->obj.planes[k].adjFace_index[1] = 0;
				new_model->obj.planes[k].adjFace_index[2] = 0;
				new_model->obj.planes[k].visible = false;
			}

			for (int k = 0; k < it.numOfVerts; k++)
			{
				new_model->m_normals.push_back(glm::vec3(it.pNormals[k].x, it.pNormals[k].y, it.pNormals[k].z));
				new_model->obj.points[k].x = it.pVerts[k].x;
				new_model->obj.points[k].y = it.pVerts[k].y;
				new_model->obj.points[k].z = it.pVerts[k].z;
			}
		}
	}
	return stdModels;
}