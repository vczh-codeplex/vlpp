#include "ModelEditorTools.h"
#include "..\..\ModelEditorCore.h"
#include "..\..\..\Shared\WindowSetup.h"
#include "..\..\..\..\..\..\..\Library\Collections\OperationCopyFrom.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
MetPushing::PushData
***********************************************************************/

	void MetPushing::PushData::Clear()
	{
		affectedModels.Clear();
		distanceVertices.Clear();
		percentVertices.Clear();
	}

	bool MetPushing::PushData::Available()
	{
		return affectedModels.Count()>0;
	}

/***********************************************************************
MetPushing
***********************************************************************/

	void MetPushing::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_LBUTTONUP:
			{
				PushStopModify();
			}
			break;
		case WM_MOUSEMOVE:
			{
				int deltaX=info.x-editorWindow->modelEditorData.originX;
				int deltaY=info.y-editorWindow->modelEditorData.originY;
				SIZE clientSize=WindowGetClient(hWnd);
				float percent=(float)deltaX/(clientSize.cx/2);
				float distance=(float)deltaY/editorWindow->GetViewDistance();

				PushModify(distance, percent);
				editorWindow->Render();
			}
			break;
		}
	}

	bool MetPushing::PushSelectedFaces()
	{
		pushData.Clear();
		int modelCount=editorWindow->ModelCount();
		for(int i=modelCount-1;i>=0;i--)
		{
			Model* model=editorWindow->GetModel(i);
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

	bool MetPushing::PushSelectedLines()
	{
		pushData.Clear();
		int modelCount=editorWindow->ModelCount();
		for(int i=modelCount-1;i>=0;i--)
		{
			Model* model=editorWindow->GetModel(i);
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
								D3DXVECTOR3 vn=v0->position+(n1+n2)/2.0f;

								Model::Vertex* newVertex=new Model::Vertex;
								newVertex->position=(v0->position+vn)/2.0f;
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

	bool MetPushing::PushSelectedPoints()
	{
		pushData.Clear();
		int modelCount=editorWindow->ModelCount();
		for(int i=modelCount-1;i>=0;i--)
		{
			Model* model=editorWindow->GetModel(i);
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

	void MetPushing::PushModify(float distance, float percent)
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

	void MetPushing::PushStopModify()
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