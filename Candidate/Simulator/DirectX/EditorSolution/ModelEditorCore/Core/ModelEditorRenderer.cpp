#include "ModelEditorRenderer.h"
#include "..\..\..\..\..\..\Library\Collections\OperationCopyFrom.h"

using namespace vl::collections;

namespace modeleditor
{

	void ModelEditorRenderer::PushData::Clear()
	{
		affectedModels.Clear();
		distanceVertices.Clear();
		percentVertices.Clear();
	}

	bool ModelEditorRenderer::PushData::Available()
	{
		return affectedModels.Count()>0;
	}

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
				currentFirstIndex+=model->modelFaces.Count();
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
								model->editorInfo.selectedFaces.Remove(j);
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
								model->editorInfo.selectedVertices.Remove(j);
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
		SelectModel(-1, false);
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
		SelectModel(-1, false);
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

	bool ModelEditorRenderer::PushSelectedFaces()
	{
		pushData.Clear();
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedFaces.Count()>0)
			{
				Dictionary<Pair<int, int>, int> lineCounter;
				Array<int> vertexMap(model->modelVertices.Count());
				for(int j=0;j<model->modelVertices.Count();j++)
				{
					vertexMap[j]=-1;
				}

				for(int j=model->editorInfo.selectedFaces.Count()-1;j>=0;j--)
				{
					Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[j]].Obj();
					
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						int vi=face->vertexIndices[k];
						if(vertexMap[vi]==-1)
						{
							Model::Vertex* vertex=new Model::Vertex;
							Model::Vertex* prototype=model->modelVertices[vi].Obj();
							vertex->position=prototype->position;
							vertex->diffuse=prototype->diffuse;
							vertexMap[vi]=model->modelVertices.Add(vertex);
						}

						int vi2=face->vertexIndices[(k+1)%face->vertexIndices.Count()];
						if(vi>vi2)
						{
							int t=vi;
							vi=vi2;
							vi2=t;
						}
						Pair<int, int> key(vi, vi2);
						int index=lineCounter.Keys().IndexOf(key);
						if(index==-1)
						{
							lineCounter.Add(key, 1);
						}
						else
						{
							lineCounter.Set(key, lineCounter.Values()[index]+1);
						}
					}
				}

				for(int j=model->editorInfo.selectedFaces.Count()-1;j>=0;j--)
				{
					Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[j]].Obj();

					List<int> oldVertexIndices;
					CopyFrom(oldVertexIndices.Wrap(), face->vertexIndices.Wrap());
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						face->vertexIndices[k]=vertexMap[face->vertexIndices[k]];
					}

					for(int k=0;k<oldVertexIndices.Count();k++)
					{
						int i1=k;
						int i2=(k+1)%oldVertexIndices.Count();
						int p1=oldVertexIndices[i1];
						int p2=oldVertexIndices[i2];
						if(lineCounter[Pair<int, int>((p1<p2?p1:p2), (p1>p2?p1:p2))]==1)
						{
							Model::Face* newFace=new Model::Face();
							newFace->vertexIndices.Add(p1);
							newFace->vertexIndices.Add(p2);
							newFace->vertexIndices.Add(vertexMap[p2]);
							newFace->vertexIndices.Add(vertexMap[p1]);
							model->modelFaces.Add(newFace);
						}
					}
				}
				model->RebuildVertexBuffer();
				pushData.affectedModels.Add(model);

				SortedList<int> vertexIndices;
				for(int j=0;j<model->editorInfo.selectedFaces.Count();j++)
				{
					Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[j]].Obj();
					for(int k=0;k<face->vertexIndices.Count();k++)
					{
						int l=face->vertexIndices[k];
						if(!vertexIndices.Contains(l))
						{
							vertexIndices.Add(l);
						}
					}
				}

				for(int j=0;j<vertexIndices.Count();j++)
				{
					Model::Vertex* vertex=model->modelVertices[vertexIndices[j]].Obj();
					PushDataDistanceVertex dv;
					dv.model=model;
					dv.vertexIndex=vertexIndices[j];
					dv.originalPosition=vertex->position;
					
					D3DXVECTOR3 normal(0, 0, 0);
					for(int k=0;k<vertex->referencedFaces.Count();k++)
					{
						int f=vertex->referencedFaces[k];
						if(model->editorInfo.selectedFaces.Contains(f))
						{
							normal+=model->vertexBufferVertices[model->modelFaces[f]->referencedStartVertexBufferVertex].normal;
						}
					}
					D3DXVec3Normalize(&dv.normal, &normal);

					pushData.distanceVertices.Add(dv);
				}
			}
		}
		return pushData.Available();
	}

	struct PSP_FaceModifier
	{
		Model::Face*			face;
		int						index;
		int						newp1, newp2;

		int Compare(const PSP_FaceModifier& value)
		{
			if(face>value.face) return -1;
			else if(face<value.face) return 1;
			else if(index>value.index) return -1;
			else if(index<value.index) return 1;
			return 0;
		}

		bool operator==(const PSP_FaceModifier& value){return Compare(value)==0;};
		bool operator!=(const PSP_FaceModifier& value){return Compare(value)!=0;};
		bool operator<(const PSP_FaceModifier& value){return Compare(value)<0;};
		bool operator<=(const PSP_FaceModifier& value){return Compare(value)<=0;};
		bool operator>(const PSP_FaceModifier& value){return Compare(value)>0;};
		bool operator>=(const PSP_FaceModifier& value){return Compare(value)>=0;};
	};

	bool ModelEditorRenderer::PushSelectedLines()
	{
		pushData.Clear();
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedVertices.Count()>0)
			{
				SortedList<int> selectedVertices;		// [vertexIndex]

				// search for end vertex indices of available lines
				{
					SortedList<int> temporarySelectedVertices;
					CopyFrom(temporarySelectedVertices.Wrap(), model->editorInfo.selectedVertices.Wrap());

					for(int j=0;j<temporarySelectedVertices.Count();j++)
					{
						Model::Vertex* vertex=model->modelVertices[temporarySelectedVertices[j]].Obj();
						for(int k=0;k<vertex->referencedFaces.Count();k++)
						{
							Model::Face* face=model->modelFaces[vertex->referencedFaces[k]].Obj();
							int index=face->vertexIndices.IndexOf(temporarySelectedVertices[j]);
							int is[]={
								(index+face->vertexIndices.Count()-1)%face->vertexIndices.Count(),
								(index+1)%face->vertexIndices.Count()
							};
							int ps[]={
								face->vertexIndices[is[0]],
								face->vertexIndices[is[1]]
							};
							if(temporarySelectedVertices.Contains(ps[0]) || temporarySelectedVertices.Contains(ps[1]))
							{
								selectedVertices.Add(temporarySelectedVertices[j]);
								break;
							}
						}
					}
				}

				if(selectedVertices.Count()>0)
				{
					SortedList<Pair<int, int>> lines;		// [(startVertexIndex, endVertexIndex)]
					Group<int, int> lineMapSelected;		// lineStartVertexIndex => [lineEndVertexIndex]
					Group<int, Pair<int, int>> lineMapAll;	// lineStartVertexIndex => [(lineEndVertexIndex, faceIndexRightOfLine)]
					SortedList<PSP_FaceModifier> faceModifier;

					// scan available end vertex to build "star"
					for(int j=0;j<selectedVertices.Count();j++)
					{
						Model::Vertex* vertex=model->modelVertices[selectedVertices[j]].Obj();
						Dictionary<int, Pair<int, int>> linePairs; // lineEndVertexIndex1 => (lineEndVertexIndex2, faceIndex)
						for(int k=0;k<vertex->referencedFaces.Count();k++)
						{
							int faceIndex=vertex->referencedFaces[k];
							Model::Face* face=model->modelFaces[faceIndex].Obj();
							int index=face->vertexIndices.IndexOf(selectedVertices[j]);
							int is[]={
								(index+face->vertexIndices.Count()-1)%face->vertexIndices.Count(),
								(index+1)%face->vertexIndices.Count()
							};
							int ps[]={
								face->vertexIndices[is[0]],
								face->vertexIndices[is[1]]
							};
							linePairs.Add(ps[0], Pair<int, int>(ps[1], faceIndex));
							for(int l=0;l<2;l++)
							{
								if(selectedVertices.Contains(ps[l]))
								{
									int p1=selectedVertices[j];
									int p2=ps[l];
									if(!lineMapSelected.Contains(p1, p2))
									{
										lineMapSelected.Add(p1, p2);
									}
									if(p1>p2)
									{
										int t=p1;
										p1=p2;
										p2=t;
									}
									Pair<int, int> key(p1, p2);
									if(!lines.Contains(key))
									{
										lines.Add(key);
									}
								}
							}
						}

						int count=linePairs.Count();
						int current=linePairs.Keys()[0];
						for(int k=0;k<count;k++)
						{
							Pair<int, int> lineEnd=linePairs[current];
							lineMapAll.Add(selectedVertices[j], Pair<int, int>(current, lineEnd.value));
							current=lineEnd.key;
						}
					}

					// create distance vertex
					for(int j=0;j<selectedVertices.Count();j++)
					{
						Model::Vertex* vertex=model->modelVertices[selectedVertices[j]].Obj();
						{
							PushDataDistanceVertex distanceVertex;
							distanceVertex.model=model;
							distanceVertex.vertexIndex=selectedVertices[j];
							distanceVertex.originalPosition=vertex->position;
					
							D3DXVECTOR3 normal(0, 0, 0);
							for(int k=0;k<vertex->referencedFaces.Count();k++)
							{
								int f=vertex->referencedFaces[k];
								normal+=model->vertexBufferVertices[model->modelFaces[f]->referencedStartVertexBufferVertex].normal;
							}
							D3DXVec3Normalize(&distanceVertex.normal, &normal);

							pushData.distanceVertices.Add(distanceVertex);
						}
					}

					// build pointsLeftOfLine and pointsRightOfLine, to extract to line quad structure
					Dictionary<Pair<int, int>, int> pointsLeftOfLine;		// (lineStartVertexIndex, lineEndVertexIndex) => [pointLeftOfRaisedLine]
					Dictionary<Pair<int, int>, int> pointsRightOfLine;		// (lineStartVertexIndex, lineEndVertexIndex) => [pointRightOfRaisedLine]
					Dictionary<Pair<int, int>, int> linePercentVertexMap;	// (lineStartVertexIndex, lineEndVertexIndex) => percentVertexIndex
					for(int j=0;j<selectedVertices.Count();j++)
					{
						int vertexIndex=selectedVertices[j];
						const IReadonlyList<int>& selectedLines=lineMapSelected[vertexIndex];
						const IReadonlyList<Pair<int, int>>& allLines=lineMapAll[vertexIndex];

						for(int k=0;k<allLines.Count();k++)
						{
							Pair<int, int> end1=allLines[k];
							Pair<int, int> end2=allLines[(k+1)%allLines.Count()];

							if(!selectedLines.Contains(end1.key))
							{
								Model::Vertex* v0=model->modelVertices[vertexIndex].Obj();
								Model::Vertex* v1=model->modelVertices[end1.key].Obj();

								Model::Vertex* newVertex=new Model::Vertex;
								newVertex->position=v0->position*0.75f+v1->position*0.25f;
								newVertex->diffuse=v0->diffuse;
								int newVertexIndex=model->modelVertices.Add(newVertex);
								linePercentVertexMap.Add(Pair<int, int>(vertexIndex, end1.key), newVertexIndex);
								model->editorInfo.selectedVertices.Add(newVertexIndex);

								PushDataPercentVertex pv;
								pv.model=model;
								pv.vertexIndex=newVertexIndex;
								pv.p1=v0->position;
								pv.p2=(v0->position+v1->position)/2.0f;
								pv.originalPercent=0.5f;
								pv.percent=0.5f;
								pushData.percentVertices.Add(pv);
							}
							else if(selectedLines.Contains(end2.key))
							{
								Model::Vertex* v0=model->modelVertices[vertexIndex].Obj();
								Model::Vertex* v1=model->modelVertices[end1.key].Obj();
								Model::Vertex* v2=model->modelVertices[end2.key].Obj();

								// TODO: the length is calculated using a temporary solution
								D3DXVECTOR3 n1=v1->position-v0->position;
								D3DXVECTOR3 n2=v2->position-v0->position;
								D3DXVec3Normalize(&n1, &n1);
								D3DXVec3Normalize(&n2, &n2);
								D3DXVECTOR3 n=(n1+n2)/2.0f;
								float l1=D3DXVec3Length(&n1);
								float l2=D3DXVec3Length(&n2);
								float l=(l1<l2?l1:l2)/2;
								D3DXVECTOR3 vn=v0->position+n*l;

								Model::Vertex* newVertex=new Model::Vertex;
								newVertex->position=v0->position+n*l/2;
								newVertex->diffuse=v0->diffuse;
								int newVertexIndex=model->modelVertices.Add(newVertex);
								model->editorInfo.selectedVertices.Add(newVertexIndex);
								pointsRightOfLine.Add(Pair<int, int>(vertexIndex, end1.key), newVertexIndex);
								pointsLeftOfLine.Add(Pair<int, int>(vertexIndex, end2.key), newVertexIndex);

								PushDataPercentVertex pv;
								pv.model=model;
								pv.vertexIndex=newVertexIndex;
								pv.p1=v0->position;
								pv.p2=vn;
								pv.originalPercent=0.5f;
								pv.percent=0.5f;
								pushData.percentVertices.Add(pv);
							}
						}

						for(int k=0;k<selectedLines.Count();k++)
						{
							int end=selectedLines[k];
							int right=-1;
							int left=-1;
							for(int l=0;l<allLines.Count();l++)
							{
								if(allLines[l].key==end)
								{
									right=allLines[(l+1)%allLines.Count()].key;
									if(selectedLines.Contains(right))
									{
										right=-1;
									}

									left=allLines[(l+allLines.Count()-1)%allLines.Count()].key;
									if(selectedLines.Contains(left))
									{
										left=-1;
									}

									break;
								}
							}

							if(right!=-1)
							{
								pointsRightOfLine.Add(Pair<int, int>(vertexIndex, end), linePercentVertexMap[Pair<int, int>(vertexIndex, right)]);
							}
							if(left!=-1)
							{
								pointsLeftOfLine.Add(Pair<int, int>(vertexIndex, end), linePercentVertexMap[Pair<int, int>(vertexIndex, left)]);
							}
						}
					}

					// build line triangle face
					for(int j=0;j<selectedVertices.Count();j++)
					{
						int vertexIndex=selectedVertices[j];
						const IReadonlyList<int>& selectedLines=lineMapSelected[vertexIndex];
						const IReadonlyList<Pair<int, int>>& allLines=lineMapAll[vertexIndex];

						for(int k=0;k<allLines.Count();k++)
						{
							Pair<int, int> end1=allLines[k];
							Pair<int, int> end2=allLines[(k+1)%allLines.Count()];
							if(!selectedLines.Contains(end1.key) && !selectedLines.Contains(end2.key))
							{
								int percent1=linePercentVertexMap[Pair<int, int>(vertexIndex, end1.key)];
								int percent2=linePercentVertexMap[Pair<int, int>(vertexIndex, end2.key)];
								{
									Model::Face* face=new Model::Face;
									face->vertexIndices.Add(percent1);
									face->vertexIndices.Add(vertexIndex);
									face->vertexIndices.Add(percent2);
									model->modelFaces.Add(face);
								}
								{
									PSP_FaceModifier fm;
									fm.face=model->modelFaces[end1.value].Obj();
									fm.index=fm.face->vertexIndices.IndexOf(vertexIndex);
									fm.newp1=percent1;
									fm.newp2=percent2;
									faceModifier.Add(fm);
								}
							}
						}
					}

					// build line quad face
					for(int j=0;j<lineMapSelected.Count();j++)
					{
						int start=lineMapSelected.Keys()[j];
						const IReadonlyList<int>& ends=lineMapSelected.GetByIndex(j);

						for(int k=0;k<ends.Count();k++)
						{
							int end=ends[k];
							int rightOfStart=pointsRightOfLine[Pair<int, int>(start, end)];
							int leftOfEnd=pointsLeftOfLine[Pair<int, int>(end, start)];
							{
								Model::Face* face=new Model::Face;
								face->vertexIndices.Add(end);
								face->vertexIndices.Add(start);
								face->vertexIndices.Add(rightOfStart);
								face->vertexIndices.Add(leftOfEnd);
								model->modelFaces.Add(face);
							}
							const IReadonlyList<Pair<int, int>>& allLines=lineMapAll[start];
							for(int l=0;l<allLines.Count();l++)
							{
								Pair<int, int> lineEnd=allLines[l];
								if(lineEnd.key==end)
								{
									Model::Face* face=model->modelFaces[lineEnd.value].Obj();
									{
										int index=face->vertexIndices.IndexOf(start);
										if(index!=-1)
										{
											face->vertexIndices[index]=rightOfStart;
										}
									}
									{
										int index=face->vertexIndices.IndexOf(end);
										if(index!=-1)
										{
											face->vertexIndices[index]=leftOfEnd;
										}
									}
									break;
								}
							}
						}
					}

					// fixed affected faces
					for(int j=0;j<faceModifier.Count();j++)
					{
						const PSP_FaceModifier& fm=faceModifier[j];
						fm.face->vertexIndices.RemoveAt(fm.index);
						fm.face->vertexIndices.Insert(fm.index, fm.newp1);
						fm.face->vertexIndices.Insert(fm.index+1, fm.newp2);
					}

					pushData.affectedModels.Add(model);
					model->RebuildVertexBuffer();
				}
			}
		}
		return pushData.Available();
	}

	bool ModelEditorRenderer::PushSelectedPoints()
	{
		pushData.Clear();
		for(int i=models.Count()-1;i>=0;i--)
		{
			Model* model=models[i].Obj();
			if(model->editorInfo.selectedVertices.Count()>0)
			{
				bool half=false;
				float percent=0.5f;
				SortedList<int> selectedVertices;
				CopyFrom(selectedVertices.Wrap(), model->editorInfo.selectedVertices.Wrap());

				for(int j=0;j<selectedVertices.Count();j++)
				{
					Model::Vertex* vertex=model->modelVertices[selectedVertices[j]].Obj();
					if(!half)
					{
						for(int k=0;k<vertex->referencedFaces.Count();k++)
						{
							Model::Face* face=model->modelFaces[vertex->referencedFaces[k]].Obj();
							int index=face->vertexIndices.IndexOf(selectedVertices[j]);
							int i1=(index+1)%face->vertexIndices.Count();
							int i2=(index+face->vertexIndices.Count()-1)%face->vertexIndices.Count();
							int p1=face->vertexIndices[i1];
							int p2=face->vertexIndices[i2];
							if(selectedVertices.Contains(p1) || selectedVertices.Contains(p2))
							{
								half=true;
							}
						}
					}
				}
				if(half)
				{
					percent=0.25f;
				}
				
				SortedList<PSP_FaceModifier> faceModifier;
				for(int j=0;j<selectedVertices.Count();j++)
				{
					Model::Vertex* vertex=model->modelVertices[selectedVertices[j]].Obj();
					{
						PushDataDistanceVertex distanceVertex;
						distanceVertex.model=model;
						distanceVertex.vertexIndex=selectedVertices[j];
						distanceVertex.originalPosition=vertex->position;
					
						D3DXVECTOR3 normal(0, 0, 0);
						for(int k=0;k<vertex->referencedFaces.Count();k++)
						{
							int f=vertex->referencedFaces[k];
							normal+=model->vertexBufferVertices[model->modelFaces[f]->referencedStartVertexBufferVertex].normal;
						}
						D3DXVec3Normalize(&distanceVertex.normal, &normal);

						pushData.distanceVertices.Add(distanceVertex);
					}
					
					Dictionary<int, int> vertexMap;
					for(int k=0;k<vertex->referencedFaces.Count();k++)
					{
						Model::Face* face=model->modelFaces[vertex->referencedFaces[k]].Obj();
						int index=face->vertexIndices.IndexOf(selectedVertices[j]);
						int is[]={
							(index+face->vertexIndices.Count()-1)%face->vertexIndices.Count(),
							(index+1)%face->vertexIndices.Count()
						};
						int ps[]={
							face->vertexIndices[is[0]],
							face->vertexIndices[is[1]]
						};

						int newps[2];
						for(int l=0;l<2;l++)
						{
							int p=ps[l];
							int mapIndex=vertexMap.Keys().IndexOf(p);
							if(mapIndex==-1)
							{
								Model::Vertex* relatedVertex=model->modelVertices[p].Obj();
								Model::Vertex* newVertex=new Model::Vertex;
								newVertex->diffuse=vertex->diffuse;
								newVertex->position=vertex->position*(1-percent)+relatedVertex->position*percent;
								newps[l]=model->modelVertices.Add(newVertex);
								vertexMap.Add(p, newps[l]);

								PushDataPercentVertex percentVertex;
								percentVertex.model=model;
								percentVertex.vertexIndex=newps[l];
								percentVertex.p1=vertex->position;
								percentVertex.p2=(half?(vertex->position+relatedVertex->position)/2.0f:relatedVertex->position);
								percentVertex.originalPercent=0.5f;
								percentVertex.percent=0.5f;
								pushData.percentVertices.Add(percentVertex);
								model->editorInfo.selectedVertices.Add(newps[l]);
							}
							else
							{
								newps[l]=vertexMap.Values()[mapIndex];
							}
						}

						{
							PSP_FaceModifier item;
							item.face=face;
							item.index=index;
							item.newp1=newps[0];
							item.newp2=newps[1];
							faceModifier.Add(item);
						}
						{
							Model::Face* face=new Model::Face;
							face->vertexIndices.Add(newps[0]);
							face->vertexIndices.Add(selectedVertices[j]);
							face->vertexIndices.Add(newps[1]);
							model->modelFaces.Add(face);
						}
					}
				}

				for(int j=0;j<faceModifier.Count();j++)
				{
					const PSP_FaceModifier& fm=faceModifier[j];
					fm.face->vertexIndices.RemoveAt(fm.index);
					fm.face->vertexIndices.Insert(fm.index, fm.newp1);
					fm.face->vertexIndices.Insert(fm.index+1, fm.newp2);
				}
				model->RebuildVertexBuffer();
				pushData.affectedModels.Add(model);
			}
		}
		return pushData.Available();
	}

	void ModelEditorRenderer::PushModify(float distance, float percent)
	{
		for(int i=0;i<pushData.distanceVertices.Count();i++)
		{
			PushDataDistanceVertex& v=pushData.distanceVertices[i];
			v.model->modelVertices[v.vertexIndex]->position=v.originalPosition+distance*v.normal;
		}
		for(int i=0;i<pushData.percentVertices.Count();i++)
		{
			PushDataPercentVertex& v=pushData.percentVertices[i];
			v.percent=v.originalPercent+percent;
			if(v.percent<0)
			{
				v.percent=0;
			}
			else if(v.percent>1)
			{
				v.percent=1;
			}
			v.model->modelVertices[v.vertexIndex]->position=v.p1*(1-v.percent)+v.p2*v.percent;
		}
		for(int i=0;i<pushData.affectedModels.Count();i++)
		{
			pushData.affectedModels[i]->RebuildVertexBuffer();
		}
	}

	void ModelEditorRenderer::PushStopModify()
	{
		for(int i=0;i<pushData.distanceVertices.Count();i++)
		{
			PushDataDistanceVertex& v=pushData.distanceVertices[i];
			v.originalPosition=v.model->modelVertices[v.vertexIndex]->position;
		}
		for(int i=0;i<pushData.percentVertices.Count();i++)
		{
			PushDataPercentVertex& v=pushData.percentVertices[i];
			v.originalPercent=v.percent;
		}
	}
}