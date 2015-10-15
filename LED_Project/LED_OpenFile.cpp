#include "LED_BMPTexture.h"
#include "openfile.h"
#include "updateEvent.h"
#include "LED_MainApplication.h"
#include "LED_RenderObject.h"
#include "LED_MainFrame.h"
#include "LED_Log.h"
#include <wx\progdlg.h>
#include "LED_3DSFile.h"

Openfile::Openfile (const wxString &title)
	: wxFrame (NULL,wxID_ANY, title, wxDefaultPosition, wxSize(300,200))
{
	tc = new wxTextCtrl (this, wxID_ANY, _T (' '), wxDefaultPosition,
		wxDefaultSize, wxTE_MULTILINE);

	wxFrame::Center ( );
}

void Openfile::OnOpen (wxCommandEvent & event)
{
	wxFileDialog * openFileDlg = new wxFileDialog (NULL);
	openFileDlg->SetWildcard ("Object files (*.obj)|*.obj|GoF Source file(*.gof)|*.gof|3DS file(*.3ds)|*.3ds");

	if (openFileDlg->ShowModal() == wxID_OK)
	{
		wxString fileName = openFileDlg->GetPath();
		//加载模型资源
		CLED_ObjectModel_EXT *new_models;
		if (fileName.SubString(fileName.size() - 3, fileName.size()) == "3DS")
		{
			CLoad3DS *model3d = new CLoad3DS();
			model3d->ImportModel(wxGetApp().ModelCache().m_queue.size(), fileName.char_str());
			new_models = model3d->transStdModel();
		}
		else
		{
			//处理obj文件
			new_models = new CLED_ObjectModel_EXT(fileName);
			new_models->loadModels();
		}

		//CLED_ObjectModel_EXT *new_models = new CLED_ObjectModel_EXT(fileName);
		//new_models->loadModels();
		for (auto new_model : new_models->models)
		{
			wxGetApp().ModelCache().AddObject(new_model->objName, new_model);
			std::vector<wxString> simpleSample;
			//simpleSample.push_back(wxString("SimpleTransVS"));
			//simpleSample.push_back(wxString("SimpleColorPS"));
			//wxGetApp().ShaderCache().AddObject(simpleSample[0], simpleTransShader);
			//wxGetApp().ShaderCache().AddObject(simpleSample[1], simpleColorShader);
			//CLED_ShaderProgram* simpleShader = new CLED_ShaderProgram(simpleSample);
			//wxGetApp().ShaderCache().AddObject("simpleSample", simpleShader);

			/***************************************************************************************************************************/
			CLED_RenderObject* simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
			//设置该节点用到的资源
			simpleNode->setModel(new_model->objName);
			//加入Shader的时候应注意顺序，参考LED_header中的列表
			simpleSample.clear();
			simpleSample.push_back(wxString("simpleSample"));
			simpleSample.push_back(wxString(SHADER_RENDER_POSITION));
			simpleSample.push_back(wxString(SHADER_RENDER_NORMAL));
			simpleSample.push_back(wxString(SHADER_RENDER_TEXTURE));
			simpleSample.push_back(wxString(SHADER_RENDER_DS));
			simpleNode->setShaders(simpleSample);

			simpleSample.clear();
			simpleSample.push_back(wxString("TestMaterial"));
			simpleNode->setMaterials(simpleSample);

			simpleSample.clear();
			simpleSample.push_back(wxString("WHITETexture"));
			simpleNode->setTextures(simpleSample);

			wxGetApp().SceneManager()->addNode(new_model->objName, simpleNode);//把节点加入到场景树中
			/***************************************************************************************************************************/
			
			////向场景中加入需要绘制的节点
			//CLED_RenderObject* simpleNode = new CLED_RenderObject();//创建一个使用Obj文件的节点
			////设置该节点用到的资源
			//simpleNode->setModel(new_model->objName);
			////设置该节点用到的资源
			//simpleSample.clear();
			//simpleSample.push_back(wxString("simpleSample"));
			//simpleNode->setShaders(simpleSample);

			////put it into the scene
			//wxGetApp().SceneManager()->addNode(new_model->objName, simpleNode);


			//update the tree control in the UI
			updateTreeCtrl(new_model->objName);
			
		}
		tree->ExpandAll();
	}
}

void Openfile::OnOpenTexture (wxCommandEvent &event)
{
	wxFileDialog *myTextureDlg = new wxFileDialog (NULL,"" ,wxEmptyString ,"" , "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif");
	
	if (myTextureDlg->ShowModal ( ) == wxID_OK)
	{
		wxString fileName = myTextureDlg->GetPath ( );
		CLED_BMPTexture *mytexture = new CLED_BMPTexture (fileName);
		wxGetApp ( ).TextureCache ( ).AddObject (fileName, mytexture);
		wxMessageBox ("Add successfully!!!");
	}
}

void Openfile::OnSave (wxCommandEvent &event)
{
	wxString str;
	wxFileDialog *save = new wxFileDialog (NULL, wxT ("save file to location"), wxEmptyString, "source.gof",
		wxT ("GoF Source file(*.gof)|*.gof|text file(*.txt)|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	save->SetDirectory("C:\\Users\\Administrator\\Desktop\\");
	//if (save->ShowModal() == wxID_CANCEL)
	//{
	//	return;
	//}
	str = save->GetPath ( );
	//str = str + save->GetFilename ( );
	if (!(wxFile::Exists(str)))
	{
		event.SetId(wxID_SAVEAS);
		OnSaveAs(event);
		return;
	}

	wxFile *newfile = new wxFile (str, wxFile::write);
	if (!(newfile->IsOpened ( )))
	{
		wxMessageBox ("New file open failed...");
		newfile->Close ( );
		return;
	}
	if (newfile->Error ( ))
	{
		wxMessageBox ("Has an error before write..");
		newfile->Close ( );
		return;
	}
	writeFile (newfile);
	if (newfile->Error ( ))
	{
		wxMessageBox ("Has an error after write..");
		newfile->Close ( );
		return;
	}
	newfile->Close ( );
	wxMessageBox ("Save successfully!!!!!!!");
}

void Openfile::OnSaveAs (wxCommandEvent &event)
{
	wxString str;
	wxFileDialog *save = new wxFileDialog (NULL, wxT ("save file to location"), wxEmptyString, "source",
		wxT ("GoF Source file(*.gof)|*.gof|text file(*.txt)|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (save->ShowModal ( ) == wxID_CANCEL)
		return;
	//tc->SaveFile (save->GetPath ( ));
	str = save->GetPath ( );
	wxFile *newfile = new wxFile (str, wxFile::write_append);
	if (!(newfile->IsOpened ( )))
	{
		wxMessageBox ("New file open failed...");
		newfile->Close ( );
		return;
	}
	if (newfile->Error ( ))
	{
		wxMessageBox ("Has an error before write..");
		newfile->Close ( );
		return;
	}
	writeFile (newfile);
	if (newfile->Error ( ))
	{
		wxMessageBox ("Has an error after write..");
		newfile->Close ( );
		return;
	}
	newfile->Close ( );
	wxMessageBox ("Save successfully!!!!!!!");
}

/*******************************functions as tools*****************************/
void Openfile::writeFile (wxFile *newfile) const
{
	
	wxMessageDialog *md = new wxMessageDialog(nullptr, "File created successfully...confirm to store!", "File confirmed to  be stored!", wxCANCEL | wxCENTRE, wxDefaultPosition);
	//wxDialog *md = new wxDialog();
	//wxProgressDialog *mdd = new wxProgressDialog("Saving file..", "saving file...");
	if (md->ShowModal() == wxID_OK)
	{
		md->Close();
		
	}
	else{
		return;
	}
	//md->Close();
	//md->Destroy();

	wxTreeItemId root = tree->GetRootItem ( );
	if (tree->IsEmpty())
	{
		newfile->Write("# Copy right by LED_Team!!!\n");
		return;
	}
	if (!(tree->HasChildren(root)))
	{
		newfile->Write("# Copy right by LED_Team!!!\n");
		return;
	}
	//save as
	wxTreeItemIdValue cookie;
	wxTreeItemId child = tree->GetFirstChild (root, cookie);
	wxString name;
	int count = tree->GetChildrenCount (root);
	int i = 1;
	while (child != nullptr)
	{
		name = tree->GetItemText(child);
		CLED_ObjectModel *obj = dynamic_cast<CLED_ObjectModel *>(wxGetApp ( ).ModelCache ( ).QueryObject (name));
		newfile->Write("# Copy right by LED_Team!!!\n");
		//write object name
		newfile->Write ("o " + name + "\n");
		//update the progress bar
		/*mdd->Update((i++)*(100/count),"saving "+name);
		int x = mdd->GetValue();*/

		for (auto it : obj->getPoints())
		{
			newfile->Write ("v ");
			newfile->Write (wxString::FromCDouble ((it.x), sizeof (double)));
			newfile->Write ("\t");
			newfile->Write (wxString::FromCDouble ((it.y), sizeof (double)));
			newfile->Write ("\t");
			newfile->Write (wxString::FromCDouble ((it.z), sizeof (double)));
			newfile->Write ("\n");
		}
		for (auto it : obj->getNormalCooors())
		{
			newfile->Write ("vn ");
			newfile->Write (wxString::FromCDouble ((it.x), sizeof (double)));
			newfile->Write ("\t");
			newfile->Write (wxString::FromCDouble ((it.y), sizeof (double)));
			newfile->Write ("\t");
			newfile->Write (wxString::FromCDouble ((it.z), sizeof (double)));
			newfile->Write ("\n");
		}
		for (auto it : obj->getUVCooors())
		{
			newfile->Write ("vt ");
			newfile->Write (wxString::FromCDouble ((it.x), sizeof (double)));
			newfile->Write ("\t");
			newfile->Write (wxString::FromCDouble ((it.y), sizeof (double)));
			newfile->Write ("\n");
		}
		const std::vector<glm::vec3> &face = obj->getFaceCoor ( );
		char str[10];
		int ss;
		for (auto it = face.cbegin ( ); it != face.cend ( ); it += 3)
		{
			newfile->Write ("f ");
			//newfile->Write (wxString::FromCDouble (it->x, sizeof (double)));
			ss = it->x;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));

			newfile->Write ("/");
			ss = it->y;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));

			newfile->Write ("/");
			ss = it->z;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));

			newfile->Write (" ");
			ss = (it+1)->x;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));

			newfile->Write ("/");
			ss = (it + 1)->y;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));
			newfile->Write ("/");
			ss = (it + 1)->z;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));
			newfile->Write (" ");
			ss = (it + 2)->x;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));
			newfile->Write ("/");
			ss = (it + 2)->y;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));
			newfile->Write ("/");
			ss = (it + 2)->z;
			sprintf(str, "%d", (ss));
			newfile->Write(str, strlen(str));
			newfile->Write ("\n");
		}
		newfile->Write ("\n");
		child = tree->GetNextChild (root, cookie);
	}
	//mdd->Close();
	//mdd->Destroy();
	md->Destroy();
}
wxString Openfile::getRealName (wxString & str)
{
	wxString realname;
	realname.append (str.substr (str.find_last_of (wxString("\\")) + 1));

	return realname;
}