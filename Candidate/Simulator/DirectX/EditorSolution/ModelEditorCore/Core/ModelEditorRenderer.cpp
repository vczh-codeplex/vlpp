#include "ModelEditorRenderer.h"
#include "..\..\..\..\..\..\Library\Collections\OperationCopyFrom.h"

using namespace vl::collections;

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
		SelectModel(models.Count()-1, false);
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

	void ModelEditorRenderer::SelectModel(int index, bool append)
	{
		if(append && mainSelectedModel)
		{
			if(0<=index && index<models.Count())
			{
				models[index]->editorInfo.selected=true;
				return;
			}
		}
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

	void ModelEditorRenderer::SelectFace(int index, int faceIndex, bool append)
	{
		if(append && mainSelectedModel)
		{
			if(0<=index && index<models.Count())
			{
				Model* model=models[index].Obj();
				if(0<=faceIndex && faceIndex<model->modelFaces.Count())
				{
					model->editorInfo.selected=false;
					model->editorInfo.selectedVertices.Clear();

					int selectedIndex=model->editorInfo.selectedFaces.IndexOf(faceIndex);
					if(selectedIndex==-1)
					{
						model->editorInfo.selectedFaces.Add(faceIndex);
					}
					else
					{
						model->editorInfo.selectedFaces.RemoveAt(selectedIndex);
					}
				}
			}
		}
		else
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
	}

	void ModelEditorRenderer::SelectFaceFromSelectedModels(int x, int y, int w, int h, bool append, bool includeBackFaces)
	{
		D3DXMATRIX viewMatrix=GetViewMatrix();
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.IsGeneralSelected())
			{
				model->editorInfo.selected=false;
				if(!append)
				{
					model->editorInfo.selectedFaces.Clear();
				}
				model->editorInfo.selectedVertices.Clear();

				D3DXMATRIX finalMatrix;
				D3DXMatrixMultiply(&finalMatrix, &model->editorInfo.worldMatrix, &viewMatrix);
				for(int j=0;j<model->modelFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[j].Obj();
					int counter=0;
					bool available=true;
					if(!includeBackFaces)
					{
						int start=face->referencedStartVertexBufferVertex;
						int count=(face->vertexIndices.Count()-2)*3;
						for(int k=start;k<start+count;k++)
						{
							D3DXVECTOR3 p=model->vertexBufferVertices[k].position;
							D3DXVECTOR3 n=model->vertexBufferVertices[k].normal;

							D3DXVECTOR4 p4;
							D3DXVec3Transform(&p4, &p, &finalMatrix);
							p=D3DXVECTOR3(p4.x/p4.w, p4.y/p4.w, p4.z/p4.w);
							D3DXVec3TransformNormal(&n, &n, &finalMatrix);
							if(D3DXVec3Dot(&p, &n)>=0)
							{
								available=false;
								break;
							}
						}
					}
					if(available)
					{
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
							int selectedIndex=model->editorInfo.selectedFaces.IndexOf(j);
							if(selectedIndex==-1)
							{
								model->editorInfo.selectedFaces.Add(j);
							}
							else
							{
								model->editorInfo.selectedFaces.RemoveAt(j);
							}
						}
					}
				}
			}
		}
	}

	void ModelEditorRenderer::SelectVertex(int index, int vertexIndex, bool append)
	{
		if(append && mainSelectedModel)
		{
			if(0<=index && index<models.Count())
			{
				Model* model=models[index].Obj();
				if(0<=vertexIndex && vertexIndex<model->modelVertices.Count())
				{
					model->editorInfo.selected=false;
					model->editorInfo.selectedFaces.Clear();

					int selectedIndex=model->editorInfo.selectedVertices.IndexOf(vertexIndex);
					if(selectedIndex==-1)
					{
						model->editorInfo.selectedVertices.Add(vertexIndex);
					}
					else
					{
						model->editorInfo.selectedVertices.RemoveAt(selectedIndex);
					}
				}
			}
		}
		else
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
	}

	void ModelEditorRenderer::SelectVertexFromSelectedModels(int x, int y, int w, int h, bool append, bool includeBackFaces)
	{
		D3DXMATRIX viewMatrix=GetViewMatrix();
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.IsGeneralSelected())
			{
				model->editorInfo.selected=false;
				model->editorInfo.selectedFaces.Clear();
				if(!append)
				{
					model->editorInfo.selectedVertices.Clear();
				}

				D3DXMATRIX finalMatrix;
				D3DXMatrixMultiply(&finalMatrix, &model->editorInfo.worldMatrix, &viewMatrix);
				for(int j=0;j<model->modelVertices.Count();j++)
				{
					Model::Vertex* vertex=model->modelVertices[j].Obj();
					bool available=true;
					if(!includeBackFaces)
					{
						available=false;
						for(int k=0;k<vertex->referencedVertexBufferVertices.Count();k++)
						{
							D3DXVECTOR3 p=model->vertexBufferVertices[vertex->referencedVertexBufferVertices[k]].position;
							D3DXVECTOR3 n=model->vertexBufferVertices[vertex->referencedVertexBufferVertices[k]].normal;

							D3DXVECTOR4 p4;
							D3DXVec3Transform(&p4, &p, &finalMatrix);
							p=D3DXVECTOR3(p4.x/p4.w, p4.y/p4.w, p4.z/p4.w);
							D3DXVec3TransformNormal(&n, &n, &finalMatrix);
							if(D3DXVec3Dot(&p, &n)<0)
							{
								available=true;
								break;
							}
						}
					}
					if(available)
					{
						int px=0, py=0;
						ToolCalculateVertexHighlight(model->editorInfo.worldMatrix, vertex->position, px, py);
						if(x<=px && px<x+w && y<=py && py<y+h)
						{
							int selectedIndex=model->editorInfo.selectedVertices.IndexOf(j);
							if(selectedIndex==-1)
							{
								model->editorInfo.selectedVertices.Add(j);
							}
							else
							{
								model->editorInfo.selectedVertices.RemoveAt(j);
							}
						}
					}
				}
			}
		}
	}

	Model* ModelEditorRenderer::GetMainSelectedModel()
	{
		return mainSelectedModel;
	}

	void ModelEditorRenderer::DeleteSelectedFaces(Model* model)
	{
		SortedList<int> sortedFaces, removedVertices;
		CopyFrom(sortedFaces.Wrap(), model->editorInfo.selectedFaces.Wrap());
		for(int j=sortedFaces.Count()-1;j>=0;j--)
		{
			int faceIndex=sortedFaces[j];
			Model::Face* face=model->modelFaces[faceIndex].Obj();
			for(int k=0;k<face->vertexIndices.Count();k++)
			{
				Model::Vertex* vertex=model->modelVertices[face->vertexIndices[k]].Obj();
				vertex->referencedFaces.Remove(faceIndex);
				if(vertex->referencedFaces.Count()==0)
				{
					removedVertices.Add(face->vertexIndices[k]);
				}
			}
			model->modelFaces.RemoveAt(faceIndex);
		}
		model->editorInfo.selectedFaces.Clear();

		if(removedVertices.Count()>0)
		{
			Array<int> verticesMap(model->modelVertices.Count());
			removedVertices.Add(model->modelVertices.Count());

			int current=0;
			for(int j=0;j<removedVertices.Count();j++)
			{
				int last=removedVertices[j];
				for(int k=current;k<last;k++)
				{
					verticesMap[k]=k-j;
				}
				current=last+1;
			}

			for(int j=model->modelFaces.Count()-1;j>=0;j--)
			{
				Model::Face* face=model->modelFaces[j].Obj();
				for(int k=0;k<face->vertexIndices.Count();k++)
				{
					int vertexIndex=face->vertexIndices[k];
					int resultIndex=verticesMap[vertexIndex];
					face->vertexIndices[k]=resultIndex;
				}
			}

			for(int j=removedVertices.Count()-2;j>=0;j--)
			{
				model->modelVertices.RemoveAt(removedVertices[j]);
			}
		}
		model->RebuildVertexBuffer();
	}

	void ModelEditorRenderer::DeleteSelection()
	{
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selected)
			{
				models.RemoveAt(i);
			}
			else if(model->editorInfo.selectedFaces.Count()>0)
			{
				if(model->editorInfo.selectedFaces.Count()==model->modelFaces.Count())
				{
					models.RemoveAt(i);
				}
				else
				{
					DeleteSelectedFaces(model);
				}
			}
			else if(model->editorInfo.selectedVertices.Count()>0)
			{
				if(model->editorInfo.selectedVertices.Count()==model->modelVertices.Count())
				{
					models.RemoveAt(i);
				}
				else
				{
					SortedList<int> selectedFaces;
					for(int j=0;j<model->editorInfo.selectedVertices.Count();j++)
					{
						Model::Vertex* vertex=model->modelVertices[model->editorInfo.selectedVertices[j]].Obj();
						for(int k=0;k<vertex->referencedFaces.Count();k++)
						{
							int faceIndex=vertex->referencedFaces[k];
							if(!selectedFaces.Contains(faceIndex))
							{
								selectedFaces.Add(faceIndex);
							}
						}
					}

					if(selectedFaces.Count()==model->modelFaces.Count())
					{
						models.RemoveAt(i);
					}
					else
					{
						model->editorInfo.selectedVertices.Clear();
						CopyFrom(model->editorInfo.selectedFaces.Wrap(), selectedFaces.Wrap());
						DeleteSelectedFaces(model);
					}
				}
			}
		}
	}

	void ModelEditorRenderer::DeleteSelectedLineBetweenSelectionPoints()
	{
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedVertices.Count()==2)
			{
				int newVertexIndex=-1;
				int v1=model->editorInfo.selectedVertices[0];
				int v2=model->editorInfo.selectedVertices[1];
				for(int j=0;j<model->modelFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[j].Obj();
					int p1=face->vertexIndices.IndexOf(v1);
					int p2=face->vertexIndices.IndexOf(v2);
					if(p1!=-1 && p2!=-1)
					{
						if(p1-p2==1 || p2-p1==1 || (p1==0 && p2==face->vertexIndices.Count()-1) || (p2==0 && p1==face->vertexIndices.Count()-1))
						{
							model->editorInfo.selectedFaces.Add(j);
						}
					}
				}
				model->editorInfo.selectedVertices.Clear();
				if(model->editorInfo.selectedFaces.Count()==model->modelFaces.Count())
				{
					models.RemoveAt(i);
				}
				else
				{
					DeleteSelectedFaces(model);
				}
			}
		}
	}

	void ModelEditorRenderer::AddLineBetweenSelectionPoints()
	{
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedVertices.Count()==2)
			{
				int newVertexIndex=-1;
				int v1=model->editorInfo.selectedVertices[0];
				int v2=model->editorInfo.selectedVertices[1];
				int oldFaceCount=model->modelFaces.Count();
				for(int j=0;j<oldFaceCount;j++)
				{
					Model::Face* face=model->modelFaces[j].Obj();
					int p1=face->vertexIndices.IndexOf(v1);
					int p2=face->vertexIndices.IndexOf(v2);
					if(p1!=-1 && p2!=-1)
					{
						if(!(p1-p2==1 || p2-p1==1 || (p1==0 && p2==face->vertexIndices.Count()-1) || (p2==0 && p1==face->vertexIndices.Count()-1)))
						{
							if(p1>p2)
							{
								int t=p1;
								p1=p2;
								p2=t;
							}

							Model::Face* newFace=new Model::Face;
							for(int k=p1;k<=p2;k++)
							{
								newFace->vertexIndices.Add(face->vertexIndices[k]);
							}
							model->modelFaces.Add(newFace);
							for(int k=p1+1;k<p2;k++)
							{
								face->vertexIndices.RemoveAt(p1+1);
							}
						}
					}
				}
				if(model->modelFaces.Count()!=oldFaceCount)
				{
					model->RebuildVertexBuffer();
				}
			}
		}
	}

	void ModelEditorRenderer::AddPointBetweenSelectionPoints(int count)
	{
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedVertices.Count()==2)
			{
				List<int> newVertexIndices;
				int v1=model->editorInfo.selectedVertices[0];
				int v2=model->editorInfo.selectedVertices[1];
				for(int j=0;j<model->modelFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[j].Obj();
					int p1=face->vertexIndices.IndexOf(v1);
					int p2=face->vertexIndices.IndexOf(v2);
					if(p1!=-1 && p2!=-1)
					{
						if(p1-p2==1 || p2-p1==1 || (p1==0 && p2==face->vertexIndices.Count()-1) || (p2==0 && p1==face->vertexIndices.Count()-1))
						{
							if(newVertexIndices.Count()==0)
							{
								for(int i=0;i<count;i++)
								{
									float c1=(float)(i+1)/(count+1);
									float c2=1-c1;
									Model::Vertex* vertex=new Model::Vertex;
									vertex->position=model->modelVertices[v1]->position*c1+model->modelVertices[v2]->position*c2;
									vertex->diffuse=model->modelVertices[v1]->diffuse*c1+model->modelVertices[v2]->diffuse*c2;
									newVertexIndices.Add(model->modelVertices.Add(vertex));
								}
							}
							int minp=p1<p2?p1:p2;
							int maxp=p1>p2?p1:p2;
							bool overflow=maxp-minp!=1;
							bool reverse=overflow^(minp==p1);
							int start=overflow?0:minp+1;
							for(int i=0;i<count;i++)
							{
								if(reverse)
								{
									face->vertexIndices.Insert(start+i, newVertexIndices[count-i-1]);
								}
								else
								{
									face->vertexIndices.Insert(start+i, newVertexIndices[i]);
								}
							}
						}
					}
				}
				if(newVertexIndices.Count()>0)
				{
					CopyFrom(model->editorInfo.selectedVertices.Wrap(), newVertexIndices.Wrap());
					model->RebuildVertexBuffer();
				}
			}
		}
	}

	void ModelEditorRenderer::PushSelectedFaces()
	{
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedFaces.Count()>0)
			{
				for(int j=model->editorInfo.selectedFaces.Count()-1;j>=0;j--)
				{
					Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[j]].Obj();

					int newStart=model->modelVertices.Count();
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						Model::Vertex* vertex=new Model::Vertex;
						Model::Vertex* prototype=model->modelVertices[face->vertexIndices[k]].Obj();
						vertex->position=prototype->position;
						vertex->diffuse=prototype->diffuse;
						model->modelVertices.Add(vertex);
					}

					List<int> oldVertexIndices;
					CopyFrom(oldVertexIndices.Wrap(), face->vertexIndices.Wrap());
					face->vertexIndices.Clear();
					for(int k=0;k<oldVertexIndices.Count();k++)
					{
						face->vertexIndices.Add(newStart+k);
					}

					for(int k=0;k<oldVertexIndices.Count();k++)
					{
						Model::Face* newFace=new Model::Face();
						int i1=k;
						int i2=(k+1)%oldVertexIndices.Count();

						newFace->vertexIndices.Add(oldVertexIndices[i1]);
						newFace->vertexIndices.Add(oldVertexIndices[i2]);
						newFace->vertexIndices.Add(newStart+i2);
						newFace->vertexIndices.Add(newStart+i1);
						model->modelFaces.Add(newFace);
					}
				}
				model->RebuildVertexBuffer();
			}
		}
	}

	void ModelEditorRenderer::PushSelectedLines()
	{
	}

	void ModelEditorRenderer::PushSelectedPoints()
	{
	}
}