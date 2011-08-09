#include "ModelEditorTools.h"
#include "..\..\ModelEditorCore.h"
#include "..\..\..\Shared\WindowSetup.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{
/***********************************************************************
Helper Functions
***********************************************************************/

	bool ToolObjectEditingInfo(D3DXVECTOR3& axis, Model*& selectedLocalModel, ModelEditorWindow* editorWindow, bool enableAxisCombination)
	{
		bool available=true;
		if(enableAxisCombination)
		{
			available=false;
			bool s=(GetKeyState(VK_SHIFT)&0xFF00)!=0;
			bool x=s^((GetKeyState('X')&0xFF00)!=0);
			bool y=s^((GetKeyState('Y')&0xFF00)!=0);
			bool z=s^((GetKeyState('Z')&0xFF00)!=0);

			if(x||y||z)
			{

				D3DXVECTOR3 vx=x?D3DXVECTOR3(1, 0, 0):D3DXVECTOR3(0, 0, 0);
				D3DXVECTOR3 vy=y?D3DXVECTOR3(0, 1, 0):D3DXVECTOR3(0, 0, 0);
				D3DXVECTOR3 vz=z?D3DXVECTOR3(0, 0, 1):D3DXVECTOR3(0, 0, 0);
				axis=vx+vy+vz;
			}
			else
			{
				axis=D3DXVECTOR3(1, 1, 1);
			}
		}
		else
		{
			switch(editorWindow->modelEditorData.modelEditorAxisDirection)
			{
			case ModelEditorAxisDirection::None:
				available=false;
				break;
			case ModelEditorAxisDirection::X:
				axis=D3DXVECTOR3(1, 0, 0);
				break;
			case ModelEditorAxisDirection::Y:
				axis=D3DXVECTOR3(0, 1, 0);
				break;
			case ModelEditorAxisDirection::Z:
				axis=D3DXVECTOR3(0, 0, 1);
				break;
			}
		}
		if(editorWindow->modelEditorData.modelEditorAxis==ModelEditorAxis::AxisLocal)
		{
			if(editorWindow->GetMainSelectedModel())
			{
				selectedLocalModel=editorWindow->GetMainSelectedModel();
			}
		}
		if(!enableAxisCombination && selectedLocalModel)
		{
			D3DXVec3TransformNormal(&axis, &axis, &selectedLocalModel->editorInfo.worldMatrix);
			D3DXVec3Normalize(&axis, &axis);
		}
		return available;
	}

	D3DXVECTOR3 ToolObjectTransform(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& inverseMatrix, const D3DXMATRIX& transformation, D3DXVECTOR3 modelVertex)
	{
		D3DXVECTOR4 temp;
		D3DXVec3Transform(&temp, &modelVertex, &worldMatrix);
		D3DXVec4Transform(&temp, &temp, &transformation);
		D3DXVec4Transform(&temp, &temp, &inverseMatrix);
		return D3DXVECTOR3(temp.x/temp.w, temp.y/temp.w, temp.z/temp.w);
	}

	void ToolObjectTransformSelectedVertices(Model* model, const D3DXMATRIX& transformation)
	{
		D3DXMATRIX inverse;
		D3DXMatrixInverse(&inverse, NULL, &model->editorInfo.worldMatrix);
		if(model->editorInfo.selectedVertices.Count()>0)
		{
			for(int i=0;i<model->editorInfo.selectedVertices.Count();i++)
			{
				D3DXVECTOR3& v=model->modelVertices[model->editorInfo.selectedVertices[i]]->position;
				v=ToolObjectTransform(model->editorInfo.worldMatrix, inverse, transformation, v);
			}
		}
		else
		{
			SortedList<int> transformedVertices;
			for(int i=0;i<model->editorInfo.selectedFaces.Count();i++)
			{
				Model::Face* face=model->modelFaces[model->editorInfo.selectedFaces[i]].Obj();
				for(int j=0;j<face->vertexIndices.Count();j++)
				{
					int k=face->vertexIndices[j];
					if(!transformedVertices.Contains(k))
					{
						transformedVertices.Add(k);
						D3DXVECTOR3& v=model->modelVertices[k]->position;
						v=ToolObjectTransform(model->editorInfo.worldMatrix, inverse, transformation, v);
					}
				}
			}
		}
		model->RebuildVertexBuffer();
	}

/***********************************************************************
MetTranslation
***********************************************************************/

	void MetTranslation::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
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
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow, false))
				{
					int deltaY=info.y-editorWindow->modelEditorData.originY;
					float distance=(float)deltaY/editorWindow->GetViewDistance();
					D3DXMATRIX translation;
					D3DXMatrixTranslation(&translation, axis.x*distance, axis.y*distance, axis.z*distance);

					int count=editorWindow->ModelCount();
					for(int i=0;i<count;i++)
					{
						Model* model=editorWindow->GetModel(i);
						if(model->editorInfo.selected)
						{
							D3DXMatrixMultiply(&model->editorInfo.worldMatrix, &model->editorInfo.worldMatrix, &translation);
						}
						else if(model->editorInfo.IsGeneralSelected())
						{
							ToolObjectTransformSelectedVertices(model, translation);
						}
					}

					editorWindow->Render();
					editorWindow->modelEditorData.originX=info.x;
					editorWindow->modelEditorData.originY=info.y;
				}
			}
			break;
		}
	}

/***********************************************************************
MetRotation
***********************************************************************/

	void MetRotation::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
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
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow, false))
				{
					int deltaY=info.y-editorWindow->modelEditorData.originY;
					SIZE clientSize=WindowGetClient(hWnd);
					float angle=(float)D3DX_PI*2*deltaY/clientSize.cy;
					D3DXMATRIX rotation;
					D3DXMatrixRotationAxis(&rotation, &axis, angle);

					if(selectedLocalModel)
					{
						D3DXMATRIX r, m;
						D3DXVECTOR3 o(0, 0, 0);
						D3DXVECTOR4 ot;

						D3DXVec3Transform(&ot, &o,&selectedLocalModel->editorInfo.worldMatrix);
						o=D3DXVECTOR3(ot.x/ot.w, ot.y/ot.w, ot.z/ot.w);

						D3DXMatrixIdentity(&r);
						D3DXMatrixTranslation(&m, -o.x, -o.y, -o.z);
						D3DXMatrixMultiply(&r, &r, &m);
						D3DXMatrixMultiply(&r, &r, &rotation);
						D3DXMatrixTranslation(&m, o.x, o.y, o.z);
						D3DXMatrixMultiply(&r, &r, &m);

						rotation=r;
					}

					int count=editorWindow->ModelCount();
					for(int i=0;i<count;i++)
					{
						Model* model=editorWindow->GetModel(i);
						if(model->editorInfo.selected)
						{
							D3DXMatrixMultiply(&model->editorInfo.worldMatrix, &model->editorInfo.worldMatrix, &rotation);
						}
						else if(model->editorInfo.IsGeneralSelected())
						{
							ToolObjectTransformSelectedVertices(model, rotation);
						}
					}

					editorWindow->Render();
					editorWindow->modelEditorData.originX=info.x;
					editorWindow->modelEditorData.originY=info.y;
				}
			}
			break;
		}
	}

/***********************************************************************
MetScaling
***********************************************************************/

	void MetScaling::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
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
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(!ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow, true))
				{
					int deltaY=info.y-editorWindow->modelEditorData.originY;
					SIZE clientSize=WindowGetClient(hWnd);
					float distance=
						deltaY>0
						?1+(float)deltaY/clientSize.cy*3
						:1/(1-(float)deltaY/clientSize.cy*3)
						;
					D3DXMATRIX scaling;
					D3DXMatrixScaling(&scaling, distance*axis.x+(1-axis.x), distance*axis.y+(1-axis.y), distance*axis.z+(1-axis.z));

					if(selectedLocalModel)
					{
						D3DXMATRIX r, m;
						D3DXVECTOR3 o(0, 0, 0);
						D3DXVECTOR4 ot;

						D3DXVec3Transform(&ot, &o,&selectedLocalModel->editorInfo.worldMatrix);
						o=D3DXVECTOR3(ot.x/ot.w, ot.y/ot.w, ot.z/ot.w);

						D3DXMatrixIdentity(&r);
						D3DXMatrixTranslation(&m, -o.x, -o.y, -o.z);
						D3DXMatrixMultiply(&r, &r, &m);
						D3DXMatrixMultiply(&r, &r, &scaling);
						D3DXMatrixTranslation(&m, o.x, o.y, o.z);
						D3DXMatrixMultiply(&r, &r, &m);

						scaling=r;
					}

					int count=editorWindow->ModelCount();
					for(int i=0;i<count;i++)
					{
						Model* model=editorWindow->GetModel(i);
						if(model->editorInfo.selected)
						{
							if(axis.x==axis.y && axis.y==axis.z)
							{
								D3DXMatrixMultiply(&model->editorInfo.worldMatrix, &model->editorInfo.worldMatrix, &scaling);
							}
						}
						else if(model->editorInfo.IsGeneralSelected())
						{
							ToolObjectTransformSelectedVertices(model, scaling);
						}
					}

					editorWindow->Render();
					editorWindow->modelEditorData.originX=info.x;
					editorWindow->modelEditorData.originY=info.y;
				}
			}
			break;
		}
	}
}