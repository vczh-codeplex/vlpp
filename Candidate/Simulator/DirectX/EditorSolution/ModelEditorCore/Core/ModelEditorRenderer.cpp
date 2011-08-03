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
			bool selected=model->editorInfo.IsGeneralSelected();
			if(!onlySelected || selected)
			{
				ToolSetWorldMatrix(model->editorInfo.worldMatrix);
				model->Geometry()->SetCurrentAndRender(selected?selectedObjectShader:normalObjectShader);
			}
		}
	}

	void ModelEditorRenderer::CallbackRenderVertexHighlights()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedFaces.Count()>0)
			{
				for(int j=0;j<model->editorInfo.selectedFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[j]].Obj();
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						Model::Vertex* v=model->modelVertices[face->vertexIndices[k]].Obj();
						ToolDrawVertexHighlight(
							model->editorInfo.worldMatrix,
							v->position
							);
					}
				}
			}
			if(model->editorInfo.selectedVertices.Count()>0)
			{
				for(int j=0;j<model->editorInfo.selectedVertices.Count();j++)
				{
					Model::Vertex* v=model->modelVertices[model->editorInfo.selectedVertices[j]].Obj();
					ToolDrawVertexHighlight(
						model->editorInfo.worldMatrix,
						v->position
						);
				}
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
			worldMatrix=mainSelectedModel->editorInfo.worldMatrix;
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
			for(int j=0;j<model->vertexBufferVertices.Count();j++)
			{
				model->vertexBufferVertices[j].id=i+1;
			}
			model->UpdateVertexBuffer();
		}
		ToolRenderSelector(false);
	}

	void ModelEditorRenderer::RenderSelectorFaceIndexIncremented()
	{
		int faceIndex=1;
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			for(int j=0;j<model->modelFaces.Count();j++)
			{
				Model::Face* face=model->modelFaces[j].Obj();
				int vertexCount=(face->vertexIndices.Count()-2)*3;
				for(int k=0;k<vertexCount;k++)
				{
					model->vertexBufferVertices[face->referencedStartVertexBufferVertex+k].id=faceIndex;
				}
				faceIndex++;
			}
			model->UpdateVertexBuffer();
		}
		ToolRenderSelector(false);
	}

	void ModelEditorRenderer::RenderSelectorSelected()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			bool selected=model->editorInfo.IsGeneralSelected();
			unsigned __int32 id=selected?1:0;
			for(int j=0;j<model->vertexBufferVertices.Count();j++)
			{
				model->vertexBufferVertices[j].id=id;
			}
			model->UpdateVertexBuffer();
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
		D3DXMatrixTranslation(&model->editorInfo.worldMatrix, at.x, at.y, at.z);
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
		int result=(int)ToolGetSelectorResult(x, y);
		return result-1;
	}

	bool ModelEditorRenderer::QueryFace(int x, int y, int& modelIndex, int& faceIndex)
	{
		RenderSelectorFaceIndexIncremented();
		int result=(int)ToolGetSelectorResult(x, y);
		if(result)
		{
			int currentFirstIndex=1;
			for(int i=0;i<models.Count();i++)
			{
				Model* model=models[i].Obj();
				if(currentFirstIndex<=result && result<currentFirstIndex+model->modelFaces.Count())
				{
					modelIndex=i;
					faceIndex=result-currentFirstIndex;
					break;
				}
			}
			return true;
		}
		else
		{
			modelIndex=-1;
			faceIndex=-1;
			return false;
		}
	}

	bool ModelEditorRenderer::QueryVertex(int x, int y, int& modelIndex, int& vertexIndex)
	{
		if(!QueryFace(x, y, modelIndex, vertexIndex))
		{
			return false;
		}
		int distance=-1;
		Model* model=models[modelIndex].Obj();
		Model::Face* face=model->modelFaces[vertexIndex].Obj();
		vertexIndex=-1;

		for(int i=0;i<face->vertexIndices.Count();i++)
		{
			int fx=0;
			int fy=0;
			ToolCalculateVertexHighlight(model->editorInfo.worldMatrix, model->modelVertices[face->vertexIndices[i]]->position, fx, fy);
			int fdistance=(x-fx)*(x-fx)+(y-fy)*(y-fy);
			if(fdistance<25 && (fdistance<distance || distance==-1))
			{
				vertexIndex=face->vertexIndices[i];
			}
		}

		if(vertexIndex==-1)
		{
			modelIndex=-1;
			return false;
		}
		else
		{
			return true;
		}
	}

	void ModelEditorRenderer::SelectModel(int index)
	{
		mainSelectedModel=0;
		for(int i=0;i<models.Count();i++)
		{
			models[i]->editorInfo.selected=i==index;
			models[i]->editorInfo.selectedFaces.Clear();
			models[i]->editorInfo.selectedVertices.Clear();
			if(i==index)
			{
				mainSelectedModel=models[i].Obj();
			}
		}
	}

	void ModelEditorRenderer::SelectFace(int index, int faceIndex)
	{
		mainSelectedModel=0;
		for(int i=0;i<models.Count();i++)
		{
			models[i]->editorInfo.selected=false;
			models[i]->editorInfo.selectedFaces.Clear();
			models[i]->editorInfo.selectedVertices.Clear();
			if(i==index)
			{
				models[i]->editorInfo.selectedFaces.Add(faceIndex);
				mainSelectedModel=models[i].Obj();
			}
		}
	}

	void ModelEditorRenderer::SelectFaceFromSelectedModels(int x, int y, int w, int h)
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.IsGeneralSelected())
			{
				model->editorInfo.selected=false;
				model->editorInfo.selectedFaces.Clear();
				model->editorInfo.selectedVertices.Clear();
				for(int j=0;j<model->modelFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[j].Obj();
					int counter=0;
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						int px=0, py=0;
						ToolCalculateVertexHighlight(model->editorInfo.worldMatrix, model->modelVertices[face->vertexIndices[k]]->position, px, py);
						if(x<=px && px<x+w && y<=py && py<y+h)
						{
							counter++;
						}
					}
					if(counter==face->vertexIndices.Count())
					{
						model->editorInfo.selectedFaces.Add(j);
					}
				}
			}
		}
	}

	void ModelEditorRenderer::SelectVertex(int index, int vertexIndex)
	{
		mainSelectedModel=0;
		for(int i=0;i<models.Count();i++)
		{
			models[i]->editorInfo.selected=false;
			models[i]->editorInfo.selectedFaces.Clear();
			models[i]->editorInfo.selectedVertices.Clear();
			if(i==index)
			{
				models[i]->editorInfo.selectedVertices.Add(vertexIndex);
				mainSelectedModel=models[i].Obj();
			}
		}
	}

	void ModelEditorRenderer::SelectVertexFromSelectedModels(int x, int y, int w, int h)
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.IsGeneralSelected())
			{
				model->editorInfo.selected=false;
				model->editorInfo.selectedFaces.Clear();
				model->editorInfo.selectedVertices.Clear();
				for(int j=0;j<model->modelVertices.Count();j++)
				{
					int px=0, py=0;
					ToolCalculateVertexHighlight(model->editorInfo.worldMatrix, model->modelVertices[j]->position, px, py);
					if(x<=px && px<x+w && y<=py && py<y+h)
					{
						model->editorInfo.selectedVertices.Add(j);
					}
				}
			}
		}
	}

	Model* ModelEditorRenderer::GetMainSelectedModel()
	{
		return mainSelectedModel;
	}
}