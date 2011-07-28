#include "ModelEditorRenderer.h"

namespace modeleditor
{

/***********************************************************************
ModelEditorData
***********************************************************************/

	ModelEditorData::ModelEditorData()
		:originX(0)
		,originY(0)
		,modelEditorOperation(ModelEditorOperation::None)
		,modelEditorOperationActivated(false)
		,modelEditorMode(ModelEditorMode::ObjectSelection)
		,modelEditorAxis(ModelEditorAxis::AxisGlobal)
		,modelEditorAxisDirection(ModelEditorAxisDirection::None)
	{
	}

	ModelEditorData::~ModelEditorData()
	{
	}

/***********************************************************************
ModelEditorWindow
***********************************************************************/

	void ModelEditorWindow::CallbackRebuildModels()
	{
		for(int i=0;i<models.Count();i++)
		{
			models[i]->Rebuild(Env());
		}
	}

	void ModelEditorWindow::CallbackDrawEditorMode(WinDC* dc)
	{
		WString axisDirection;
		switch(modelEditorData.modelEditorAxisDirection)
		{
		case ModelEditorAxisDirection::X:
			axisDirection=L"(X)";
			break;
		case ModelEditorAxisDirection::Y:
			axisDirection=L"(Y)";
			break;
		case ModelEditorAxisDirection::Z:
			axisDirection=L"(Z)";
			break;
		}

		dc->DrawString(10, 10, L"Change View\t\t\t\t\t{RBUTTON + (CTRL|SHIFT|NONE)}", 32, 10);
		switch(modelEditorData.modelEditorMode)
		{
		case ModelEditorMode::ObjectSelection:
			dc->DrawString(10, 30, L"Editor Mode[CTRS]\t: Select\t\t{LBUTTON}", 32, 10);
			break;
		case ModelEditorMode::ObjectTranslation:
			dc->DrawString(10, 30, L"Editor Mode[CTRS]\t: Move"+axisDirection+L"\t\t{LBUTTON + [XYZ]}", 32, 10);
			break;
		case ModelEditorMode::ObjectRotation:
			dc->DrawString(10, 30, L"Editor Mode[CTRS]\t: Rotate"+axisDirection+L"\t\t{LBUTTON + [XYZ]}", 32, 10);
			break;
		case ModelEditorMode::ObjectScaling:
			dc->DrawString(10, 30, L"Editor Mode[CTRS]\t: Scale\t\t{LBUTTON}", 32, 10);
			break;
		}
		switch(modelEditorData.modelEditorAxis)
		{
		case ModelEditorAxis::AxisGlobal:
			dc->DrawString(10, 50, L"Relative Axis[GL]\t\t: Global", 32, 10);
			break;
		case ModelEditorAxis::AxisLocal:
			dc->DrawString(10, 50, L"Relative Axis[GL]\t\t: Local", 32, 10);
			break;
		}
	}

	void ModelEditorWindow::CallbackRenderModels(bool onlySelected, DirectXShader<VertexObject>* normalObjectShader, DirectXShader<VertexObject>* selectedObjectShader)
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(!onlySelected || model->selected)
			{
				ToolSetWorldMatrix(model->worldMatrix);
				model->Geometry()->SetCurrentAndRender(model->selected?selectedObjectShader:normalObjectShader);
			}
		}
	}

	void ModelEditorWindow::CallbackRenderSelectorSelected()
	{
		RenderSelectorSelected();
	}

	bool ModelEditorWindow::CallbackRenderLocalAxis(D3DXMATRIX& worldMatrix)
	{
		if(mainSelectedModel)
		{
			worldMatrix=mainSelectedModel->worldMatrix;
			return true;
		}
		else
		{
			return false;
		}
	}

	void ModelEditorWindow::RenderSelectorModelIndexIncremented()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			for(int j=0;j<model->vertices.Count();j++)
			{
				model->vertices[j].id=i+1;
			}
			model->Update();
		}
		ToolRenderSelector(false);
	}

	void ModelEditorWindow::RenderSelectorSelected()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			unsigned __int32 id=model->selected?1:0;
			for(int j=0;j<model->vertices.Count();j++)
			{
				model->vertices[j].id=id;
			}
			model->Update();
		}
		ToolRenderSelector(true);
	}

	ModelEditorWindow::ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory)
		:ModelSceneRenderer(_editorControl, _workingDirectory)
		,mainSelectedModel(0)
	{
		Constructor();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		Destructor();
	}

	void ModelEditorWindow::AddModel(Model* model)
	{
		D3DXVECTOR3 at=GetViewAt();
		models.Add(model);
		D3DXMatrixTranslation(&model->worldMatrix, at.x, at.y, at.z);
		SelectModel(models.Count()-1);
	}

	void ModelEditorWindow::RemoveModel(Model* model)
	{
		models.Remove(model);
		if(mainSelectedModel==model)
		{
			mainSelectedModel=0;
		}
	}

	int ModelEditorWindow::ModelCount()
	{
		return models.Count();
	}

	Model* ModelEditorWindow::GetModel(int index)
	{
		return models[index].Obj();
	}

	int ModelEditorWindow::QueryModel(int x, int y)
	{
		RenderSelectorModelIndexIncremented();
		unsigned __int32 result=ToolGetSelectorResult(x, y);
		return (int)result-1;
	}

	void ModelEditorWindow::SelectModel(int index)
	{
		mainSelectedModel=0;
		for(int i=0;i<models.Count();i++)
		{
			models[i]->selected=i==index;
			models[i]->mainSelected=i==index;
			if(i==index)
			{
				mainSelectedModel=models[i].Obj();
			}
		}
	}

	Model* ModelEditorWindow::GetMainSelectedModel()
	{
		return mainSelectedModel;
	}

	void ModelEditorWindow::SetEditorMode(ModelEditorMode::Enum value)
	{
		modelEditorData.modelEditorMode=value;
		ToolDrawEditorMode();
	}

	void ModelEditorWindow::SetEditorAxis(ModelEditorAxis::Enum value)
	{
		modelEditorData.modelEditorAxis=value;
		ToolDrawEditorMode();
	}

	void ModelEditorWindow::SetEditorAxisDirection(ModelEditorAxisDirection::Enum value)
	{
		modelEditorData.modelEditorAxisDirection=value;
		ToolDrawEditorMode();
	}
}