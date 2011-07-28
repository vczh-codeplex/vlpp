#include "ModelEditorRenderer.h"

namespace modeleditor
{

/***********************************************************************
ModelEditorRenderer
***********************************************************************/

	void ModelEditorRenderer::CallbackRebuildModels()
	{
		for(int i=0;i<models.Count();i++)
		{
			models[i]->Rebuild(Env());
		}
	}

	void ModelEditorRenderer::CallbackRenderModels(bool onlySelected, DirectXShader<VertexObject>* normalObjectShader, DirectXShader<VertexObject>* selectedObjectShader)
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

	void ModelEditorRenderer::CallbackRenderSelectorSelected()
	{
		RenderSelectorSelected();
	}

	bool ModelEditorRenderer::CallbackRenderLocalAxis(D3DXMATRIX& worldMatrix)
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

	void ModelEditorRenderer::RenderSelectorModelIndexIncremented()
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

	void ModelEditorRenderer::RenderSelectorSelected()
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

	ModelEditorRenderer::ModelEditorRenderer(HWND _editorControl, const WString& _workingDirectory)
		:ModelSceneRenderer(_editorControl, _workingDirectory)
		,mainSelectedModel(0)
	{
	}

	ModelEditorRenderer::~ModelEditorRenderer()
	{
	}

	void ModelEditorRenderer::AddModel(Model* model)
	{
		D3DXVECTOR3 at=GetViewAt();
		models.Add(model);
		D3DXMatrixTranslation(&model->worldMatrix, at.x, at.y, at.z);
		SelectModel(models.Count()-1);
	}

	void ModelEditorRenderer::RemoveModel(Model* model)
	{
		models.Remove(model);
		if(mainSelectedModel==model)
		{
			mainSelectedModel=0;
		}
	}

	int ModelEditorRenderer::ModelCount()
	{
		return models.Count();
	}

	Model* ModelEditorRenderer::GetModel(int index)
	{
		return models[index].Obj();
	}

	int ModelEditorRenderer::QueryModel(int x, int y)
	{
		RenderSelectorModelIndexIncremented();
		unsigned __int32 result=ToolGetSelectorResult(x, y);
		return (int)result-1;
	}

	void ModelEditorRenderer::SelectModel(int index)
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

	Model* ModelEditorRenderer::GetMainSelectedModel()
	{
		return mainSelectedModel;
	}
}