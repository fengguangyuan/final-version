#include "LED_ObjectModel_EXT.h"
#include "LED_Log.h"
#include <limits>

CLED_ObjectModel_EXT::CLED_ObjectModel_EXT(const wxString& filename) :CLED_Object()
{
	//models = new std::vector<CLED_ObjectModel>;
	fileName = filename;
}

CLED_ObjectModel_EXT::~CLED_ObjectModel_EXT()
{
	
}

void CLED_ObjectModel_EXT::loadModels(){
	

	CLED_LOG::LogWrite(wxString("Load .obj Model: ->") + fileName + wxString(" ...\n"));

	FILE* fp = fopen(fileName.ToStdString().c_str(), "r");
	if (!fp)
	{
		CLED_LOG::LogWrite(wxString("Error : Open file: '") + fileName + wxString("' Failed\n"));
	}

	fseek(fp, 0, SEEK_END);
	unsigned int uiEnd = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned int uiStart = ftell(fp);
	unsigned int uiFileSize = uiEnd - uiStart;
	char* buffer = new char[uiFileSize];

	if (!buffer)
	{
		CLED_LOG::LogWrite(wxString("Error : Load file: '") + fileName + wxString("' Failed -> Not Enough Memory!\n"));
	}

	if (fread(buffer, 1, uiFileSize, fp) != uiFileSize)
	{
		CLED_LOG::LogWrite(wxString("Error : Read file: '") + fileName + wxString("' Failed\n"));
	}

	std::vector<glm::vec3> temp_pos;
	std::vector<glm::vec3> temp_normal;
	std::vector<glm::vec2> temp_uv;
	char linebuf[256];
	unsigned int chars_read = 0;
	unsigned int chars_line = 0;

	bool have_normal = false;
	bool have_uv = false;

	glm::vec3 minPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	glm::vec3 maxPos(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	CLED_ObjectModel *newModel = new CLED_ObjectModel();
	while (chars_read < uiFileSize)
	{
		do
		{
			linebuf[chars_line++] = buffer[chars_read];
		} while (buffer[chars_read++] != '\n');

		chars_line = 0;
		if (linebuf[0] == 'o' && linebuf[1] == ' ')
		{
			if (newModel->m_faceCount > 0)
			{
				models.push_back(newModel);
				/***************************************************************************/
				newModel->obj.planes_n = newModel->m_faceCount;
				newModel->obj.points_n = temp_pos.size() + 1;
				newModel->obj.points = temp_pos;
				newModel->obj.points.insert(newModel->obj.points.begin(), glm::vec3(0, 0, 0));
				newModel->m_Position = temp_pos;
				newModel->m_UVCooors = temp_uv;
				newModel->m_NormalCooors = temp_normal;
				/***************************************************************************/

				if (have_uv == false)
					CLED_LOG::LogWrite(wxString("Error : ") + fileName + wxString("-> Does NOT Contain TexCoord! Informations\n"));
				if (have_normal == false)
					CLED_LOG::LogWrite(wxString("Error : ") + fileName + wxString("-> Does NOT Contain Normal Informations!\n"));

				newModel->m_boundingSphereCenter = (maxPos - minPos)*0.5f;
				newModel->m_boundingSphereRadius = (maxPos - minPos).length()*0.5f;
				temp_pos.clear();
				temp_normal.clear();
				temp_uv.clear();
			}
			newModel = new CLED_ObjectModel();
			for (int i = 2;; i++)
			{
				if (linebuf[i] == '\n')
					break;
				newModel->objName += linebuf[i];
			}
		}
		if (linebuf[0] == 'v' && linebuf[1] == ' ')
		{
			glm::vec3 vertex;
			sscanf(linebuf + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_pos.push_back(vertex);
			if (vertex.x > maxPos.x)
				maxPos.x = vertex.x;
			if (vertex.y > maxPos.y)
				maxPos.y = vertex.y;
			if (vertex.z > maxPos.z)
	
				maxPos.z = vertex.z;

			if (vertex.x < minPos.x)
				maxPos.x = vertex.x;
			if (vertex.y < minPos.y)
				maxPos.y = vertex.y;
			if (vertex.z < minPos.z)
				maxPos.z = vertex.z;
		}
		else if (linebuf[0] == 'v' && linebuf[1] == 'n')
		{
			glm::vec3 normal;
			sscanf(linebuf + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normal.push_back(normal);
			have_normal = true;
		}
		else if (linebuf[0] == 'v' && linebuf[1] == 't')
		{
			glm::vec2 uv;
			sscanf(linebuf + 3, "%f %f", &uv.x, &uv.y);
			temp_uv.push_back(uv);
			have_uv = true;
		}
		else if (linebuf[0] == 'f' && linebuf[1] == ' ')
		{
			int v1_p, v1_t, v1_n;
			int v2_p, v2_t, v2_n;
			int v3_p, v3_t, v3_n;

			sscanf(linebuf + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&v1_p, &v1_t, &v1_n,
				&v2_p, &v2_t, &v2_n,
				&v3_p, &v3_t, &v3_n);

			//m_faceCoors.push_back ({ v1_p, v1_t, v1_n });
			newModel->m_faceCoors.push_back(glm::vec3(v1_p, v1_t, v1_n));
			newModel->m_faceCoors.push_back(glm::vec3(v2_p, v2_t, v2_n));
			newModel->m_faceCoors.push_back(glm::vec3(v3_p, v3_t, v3_n));

			//m_faceCoors.push_back ({ v2_p, v2_t, v2_n });
			//m_faceCoors.push_back ({ v3_p, v3_t, v3_n });
			newModel->m_pos.push_back(temp_pos[v1_p - 1]);
			newModel->m_UVs.push_back(temp_uv[v1_t - 1]);
			newModel->m_normals.push_back(temp_normal[v1_n - 1]);

			newModel->m_pos.push_back(temp_pos[v2_p - 1]);
			newModel->m_UVs.push_back(temp_uv[v2_t - 1]);
			newModel->m_normals.push_back(temp_normal[v2_n - 1]);

			newModel->m_pos.push_back(temp_pos[v3_p - 1]);
			newModel->m_UVs.push_back(temp_uv[v3_t - 1]);
			newModel->m_normals.push_back(temp_normal[v3_n - 1]);
			/********************* new operations **********************************/
			newModel->obj.planes[newModel->m_faceCount].p[0] = v1_p;
			newModel->obj.planes[newModel->m_faceCount].p[1] = v2_p;
			newModel->obj.planes[newModel->m_faceCount].p[2] = v3_p;
			newModel->obj.planes[newModel->m_faceCount].normals[0].x = temp_normal[v1_n - 1].x;
			newModel->obj.planes[newModel->m_faceCount].normals[0].y = temp_normal[v1_n - 1].y;
			newModel->obj.planes[newModel->m_faceCount].normals[0].z = temp_normal[v1_n - 1].z;

			newModel->obj.planes[newModel->m_faceCount].normals[1].x = temp_normal[v2_n - 1].x;
			newModel->obj.planes[newModel->m_faceCount].normals[1].y = temp_normal[v2_n - 1].y;
			newModel->obj.planes[newModel->m_faceCount].normals[1].z = temp_normal[v2_n - 1].z;

			newModel->obj.planes[newModel->m_faceCount].normals[2].x = temp_normal[v3_n - 1].x;
			newModel->obj.planes[newModel->m_faceCount].normals[2].y = temp_normal[v3_n - 1].y;
			newModel->obj.planes[newModel->m_faceCount].normals[2].z = temp_normal[v3_n - 1].z;

			newModel->obj.planes[newModel->m_faceCount].adjFace_index[0] = 0;
			newModel->obj.planes[newModel->m_faceCount].adjFace_index[1] = 0;
			newModel->obj.planes[newModel->m_faceCount].adjFace_index[2] = 0;
			newModel->obj.planes[newModel->m_faceCount].visible = false;
			/***********************************************************************/
			++newModel->m_faceCount;
		}
	}

	/***************************************************************************/
	newModel->obj.planes_n = newModel->m_faceCount;
	newModel->obj.points_n = temp_pos.size() + 1;
	newModel->obj.points = temp_pos;
	newModel->obj.points.insert(newModel->obj.points.begin(), glm::vec3(0, 0, 0));
	newModel->m_Position = temp_pos;
	newModel->m_UVCooors = temp_uv;
	newModel->m_NormalCooors = temp_normal;
	/***************************************************************************/
	if (have_uv == false)
		CLED_LOG::LogWrite(wxString("Error : ") + fileName + wxString("-> Does NOT Contain TexCoord! Informations\n"));
	if (have_normal == false)
		CLED_LOG::LogWrite(wxString("Error : ") + fileName + wxString("-> Does NOT Contain Normal Informations!\n"));

	newModel->m_boundingSphereCenter = (maxPos - minPos)*0.5f;
	newModel->m_boundingSphereRadius = (maxPos - minPos).length()*0.5f;
	//add new model
	models.push_back(newModel);

	temp_pos.clear();
	temp_normal.clear();
	temp_uv.clear();

	fclose(fp);
	delete[] buffer;
}
