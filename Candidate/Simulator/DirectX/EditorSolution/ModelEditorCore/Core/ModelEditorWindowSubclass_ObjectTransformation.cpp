#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
ModelEditorMode::ObjectTranslation
***********************************************************************/

	void ToolObjectTranslation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
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
			}
			break;
		case WM_RBUTTONDOWN:
			{
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow))
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
ModelEditorMode::ObjectRotation
***********************************************************************/

	void ToolObjectRotation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
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
			}
			break;
		case WM_RBUTTONDOWN:
			{
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow))
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
ModelEditorMode::ObjectScaling
***********************************************************************/

	void ToolObjectScaling(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
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
			}
			break;
		case WM_RBUTTONDOWN:
			{
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				D3DXVECTOR3 axis;
				Model* selectedLocalModel=0;
				if(!ToolObjectEditingInfo(axis, selectedLocalModel, editorWindow))
				{
					int deltaY=info.y-editorWindow->modelEditorData.originY;
					SIZE clientSize=WindowGetClient(hWnd);
					float distance=
						deltaY>0
						?1+(float)deltaY/clientSize.cy*3
						:1/(1-(float)deltaY/clientSize.cy*3)
						;
					D3DXMATRIX scaling;
					D3DXMatrixScaling(&scaling, distance, distance, distance);

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
							D3DXMatrixMultiply(&model->editorInfo.worldMatrix, &model->editorInfo.worldMatrix, &scaling);
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