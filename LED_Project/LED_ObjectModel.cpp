#include "LED_ObjectModel.h"
#include "LED_Log.h"
#include <limits>

// CLED_ObjectModelVertexFMT::CLED_ObjectModelVertexFMT()
// {
// 
// }
// 
// CLED_ObjectModelVertexFMT::CLED_ObjectModelVertexFMT(const glm::vec3& pos,const glm::vec3& normal,const glm::vec2& uv):
// 	m_normal(normal),m_position(pos),m_uv(uv)
// {
// 
// }
// 
// CLED_ObjectModelVertexFMT::~CLED_ObjectModelVertexFMT()
// {
// 
// }

CLED_ObjectModel::CLED_ObjectModel() : CLED_Object(), location(0.0,0.0,0.0,1.0f)
{
	m_faceCount = 0;
	obj.planes_n = 0;
	obj.points_n = 0;
}

CLED_ObjectModel::CLED_ObjectModel(const wxString& filename) :CLED_Object(), location(0.0, 0.0, 0.0, 1.0f)
{
	m_faceCount = 0;
	obj.planes_n = 0;
	obj.points_n = 0;

	CLED_LOG::LogWrite(wxString("Load .obj Model: ->") + filename + wxString(" ...\n"));

	FILE* fp = fopen(filename.ToStdString().c_str(),"r");
	if(!fp)
	{
		CLED_LOG::LogWrite(wxString("Error : Open file: '")+ filename + wxString("' Failed\n"));
	}

	fseek(fp, 0, SEEK_END);
	unsigned int uiEnd = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned int uiStart = ftell(fp);
	unsigned int uiFileSize = uiEnd - uiStart;
	char* buffer = new char[uiFileSize];

	if(!buffer)
	{
		CLED_LOG::LogWrite(wxString("Error : Load file: '")+ filename + wxString("' Failed -> Not Enough Memory!\n"));
	}

	if(fread(buffer,1,uiFileSize,fp) != uiFileSize)
	{
		CLED_LOG::LogWrite(wxString("Error : Read file: '")+ filename + wxString("' Failed\n"));
	}

	std::vector<glm::vec3> temp_pos;
	std::vector<glm::vec3> temp_normal;
	std::vector<glm::vec2> temp_uv;
	char linebuf[256];
	unsigned int chars_read = 0;
	unsigned int chars_line = 0;

	bool have_normal = false;
	bool have_uv = false;

	glm::vec3 minPos(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
	glm::vec3 maxPos(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());

	while (chars_read < uiFileSize)
	{
		do
		{
			linebuf[chars_line++] = buffer[chars_read];
		}while (buffer[chars_read++] != '\n');

		chars_line = 0;
		if (linebuf[0] == 'o' && linebuf[1] == ' ')
		{
			for (int i = 2;; i++)
			{
				if (linebuf[i] == '\n')
					break;
				objName += linebuf[i];
			}
		}
		if(linebuf[0] == 'v' && linebuf[1] == ' ')
		{
			glm::vec3 vertex;
			sscanf(linebuf+2,"%f %f %f",&vertex.x,&vertex.y,&vertex.z);
			temp_pos.push_back(vertex);
			if(vertex.x > maxPos.x)
				maxPos.x = vertex.x;
			if(vertex.y > maxPos.y)
				maxPos.y = vertex.y;
			if(vertex.z > maxPos.z)
				maxPos.z = vertex.z;

			if(vertex.x < minPos.x)
				maxPos.x = vertex.x;
			if(vertex.y < minPos.y)
				maxPos.y = vertex.y;
			if(vertex.z < minPos.z)
				maxPos.z = vertex.z;
		}
		else if (linebuf[0] == 'v' && linebuf[1] == 'n')
		{
			glm::vec3 normal;
			sscanf(linebuf+3,"%f %f %f",&normal.x,&normal.y,&normal.z);
			temp_normal.push_back(normal);
			have_normal = true;
		}
		else if(linebuf[0] == 'v' && linebuf[1] == 't')
		{
			glm::vec2 uv;
			sscanf(linebuf+3,"%f %f",&uv.x,&uv.y);
			temp_uv.push_back(uv);
			have_uv = true;
		}
		else if(linebuf[0] == 'f' && linebuf[1] == ' ')
		{
			int v1_p,v1_t,v1_n;
			int v2_p,v2_t,v2_n;
			int v3_p,v3_t,v3_n;

			sscanf(linebuf+2,"%d/%d/%d %d/%d/%d %d/%d/%d",
				&v1_p,&v1_t,&v1_n,
				&v2_p,&v2_t,&v2_n,
				&v3_p,&v3_t,&v3_n);

			//m_faceCoors.push_back ({ v1_p, v1_t, v1_n });
			m_faceCoors.push_back(glm::vec3(v1_p, v1_t, v1_n));
			m_faceCoors.push_back(glm::vec3(v2_p, v2_t, v2_n));
			m_faceCoors.push_back(glm::vec3(v3_p, v3_t, v3_n));

			//m_faceCoors.push_back ({ v2_p, v2_t, v2_n });
			//m_faceCoors.push_back ({ v3_p, v3_t, v3_n });
			m_pos.push_back(temp_pos[v1_p-1]);
			m_UVs.push_back(temp_uv[v1_t-1]);
			m_normals.push_back(temp_normal[v1_n-1]);

			m_pos.push_back(temp_pos[v2_p-1]);
			m_UVs.push_back(temp_uv[v2_t-1]);
			m_normals.push_back(temp_normal[v2_n-1]);

			m_pos.push_back(temp_pos[v3_p-1]);
			m_UVs.push_back(temp_uv[v3_t-1]);
			m_normals.push_back(temp_normal[v3_n-1]);
			/********************* new operations **********************************/
			obj.planes[m_faceCount].p[0] = v1_p;
			obj.planes[m_faceCount].p[1] = v2_p;
			obj.planes[m_faceCount].p[2] = v3_p;
			obj.planes[m_faceCount].normals[0].x = temp_normal[v1_n - 1].x;
			obj.planes[m_faceCount].normals[0].y = temp_normal[v1_n - 1].y;
			obj.planes[m_faceCount].normals[0].z = temp_normal[v1_n - 1].z;

			obj.planes[m_faceCount].normals[1].x = temp_normal[v2_n - 1].x;
			obj.planes[m_faceCount].normals[1].y = temp_normal[v2_n - 1].y;
			obj.planes[m_faceCount].normals[1].z = temp_normal[v2_n - 1].z;
	
			obj.planes[m_faceCount].normals[2].x = temp_normal[v3_n - 1].x;
			obj.planes[m_faceCount].normals[2].y = temp_normal[v3_n - 1].y;
			obj.planes[m_faceCount].normals[2].z = temp_normal[v3_n - 1].z;

			obj.planes[m_faceCount].adjFace_index[0] = 0;
			obj.planes[m_faceCount].adjFace_index[1] = 0;
			obj.planes[m_faceCount].adjFace_index[2] = 0;
			obj.planes[m_faceCount].visible = false;
			/***********************************************************************/
			++m_faceCount;
		}
	}
	/***************************************************************************/
	obj.planes_n = m_faceCount;
	obj.points_n = temp_pos.size() + 1;
	obj.points = temp_pos;
	obj.points.insert(obj.points.begin(), glm::vec3(0, 0, 0));
	m_Position = temp_pos;
	m_UVCooors = temp_uv;
	m_NormalCooors = temp_normal;
	/***************************************************************************/

	if(have_uv == false)
		CLED_LOG::LogWrite(wxString("Error : ") + filename + wxString("-> Does NOT Contain TexCoord! Informations\n"));
	if(have_normal == false)
		CLED_LOG::LogWrite(wxString("Error : ") + filename + wxString("-> Does NOT Contain Normal Informations!\n"));

	m_boundingSphereCenter = (maxPos - minPos)*0.5f;
	m_boundingSphereRadius = (maxPos - minPos).length()*0.5f;

	fclose(fp);
	delete [] buffer;
}

CLED_ObjectModel::~CLED_ObjectModel()
{

}

float CLED_ObjectModel::getBoundingSphereRadius()
{
	return m_boundingSphereRadius;
}

const glm::vec3& CLED_ObjectModel::getBoundingSphereCenter()
{
	return m_boundingSphereCenter;
}

unsigned int CLED_ObjectModel::faceCount()
{
	return m_faceCount;
}

const std::vector<glm::vec3>& CLED_ObjectModel::getVertices() const
{
	return m_pos;
}

const std::vector<glm::vec2>& CLED_ObjectModel::getUVs() const
{
	return m_UVs;
}

const std::vector<glm::vec3>& CLED_ObjectModel::getNormals() const
{
	return m_normals;
}


const std::vector<glm::vec3>& CLED_ObjectModel::getPoints() const
{
	return m_Position;
}

const std::vector<glm::vec2>& CLED_ObjectModel::getUVCooors() const
{
	return m_UVCooors;
}

const std::vector<glm::vec3>& CLED_ObjectModel::getNormalCooors() const
{
	return m_NormalCooors;
}

const void CLED_ObjectModel::setLocation(glm::vec4 &pos)
{
	location = pos;
}