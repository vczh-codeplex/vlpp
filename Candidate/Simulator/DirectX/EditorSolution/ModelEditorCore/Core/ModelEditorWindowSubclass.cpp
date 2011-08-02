#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

#include <CommCtrl.h>

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
ModelEditorOperation::ViewMoving
***********************************************************************/

	void ToolViewMoving(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				int deltaX=info.x-editorWindow->modelEditorData.originX;
				int deltaY=info.y-editorWindow->modelEditorData.originY;

				if(deltaX!=0 || deltaY!=0)
				{
					editorWindow->ViewMove((float)deltaX/editorWindow->GetViewDistance(), (float)deltaY/editorWindow->GetViewDistance(), 0);
					editorWindow->Render();
				}

				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		}
	}

/***********************************************************************
ModelEditorOperation::ViewZooming
***********************************************************************/

	void ToolViewZooming(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				int deltaY=info.y-editorWindow->modelEditorData.originY;
				float distance=(float)deltaY/editorWindow->GetViewDistance();

				editorWindow->ViewMove(0, 0, distance);
				editorWindow->Render();

				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		}
	}

/***********************************************************************
ModelEditorOperation::ViewRotation
***********************************************************************/

	void ToolViewRotation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_RBUTTONUP:
			{
			}
			break;
		case WM_MOUSEMOVE:
			{
				int deltaX=info.x-editorWindow->modelEditorData.originX;
				int deltaY=info.y-editorWindow->modelEditorData.originY;
				SIZE clientSize=WindowGetClient(hWnd);
				int minSize=clientSize.cx<clientSize.cy?clientSize.cx:clientSize.cy;
					
				float vertical=(float)D3DX_PI*2*deltaY/minSize;
				float horizontal=(float)D3DX_PI*2*deltaX/minSize;
				editorWindow->ViewRotateVertical(vertical);
				editorWindow->ViewRotateHorizontal(horizontal);
				editorWindow->Render();

				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		}
	}

/***********************************************************************
ModelEditorMode::ObjectSelection
***********************************************************************/

	void ToolObjectSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				int index=editorWindow->QueryModel(info.x, info.y);
				editorWindow->SelectModel(index);
				editorWindow->Render();
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
			}
			break;
		}
	}

/***********************************************************************
ModelEditorMode::ObjectFaceSelection
***********************************************************************/

	void ToolObjectFaceSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				int modelIndex=-1;
				int faceIndex=-1;
				if(editorWindow->QueryFace(info.x, info.y, modelIndex, faceIndex))
				{
					editorWindow->SelectFace(modelIndex, faceIndex);
				}
				else
				{
					editorWindow->SelectModel(-1);
				}
				editorWindow->Render();
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
			}
			break;
		}
	}

	void IdleObjectFaceSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
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
			}
			break;
		}
	}

/***********************************************************************
ModelEditorMode::ObjectVertexSelection
***********************************************************************/

	void ToolObjectVertexSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
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
			}
			break;
		}
	}

	void IdleObjectVertexSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
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
			}
			break;
		}
	}

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
ToolObjectEditingInfo
***********************************************************************/

	bool ToolObjectEditingInfo(D3DXVECTOR3& axis, Model*& selectedLocalModel, ModelEditorWindow* editorWindow)
	{
		bool available=true;
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
		if(editorWindow->modelEditorData.modelEditorAxis==ModelEditorAxis::AxisLocal)
		{
			if(editorWindow->GetMainSelectedModel())
			{
				selectedLocalModel=editorWindow->GetMainSelectedModel();
			}
		}
		if(selectedLocalModel)
		{
			D3DXVec3TransformNormal(&axis, &axis, &selectedLocalModel->editorInfo.worldMatrix);
			D3DXVec3Normalize(&axis, &axis);
		}
		return available;
	}

/***********************************************************************
EditorWindowSubclassProc
***********************************************************************/

	ModelEditorWindow::ToolMessageProc ChooseActivatedToolMouseProc(ModelEditorWindow* editorWindow)
	{
		switch(editorWindow->modelEditorData.modelEditorOperation)
		{
		case ModelEditorOperation::ViewRotation:
			return &ToolViewRotation;
		case ModelEditorOperation::ViewZooming:
			return &ToolViewZooming;
		case ModelEditorOperation::ViewMoving:
			return &ToolViewMoving;
		case ModelEditorOperation::ObjectEditing:
			switch(editorWindow->modelEditorData.modelEditorMode)
			{
			case ModelEditorMode::ObjectSelection:
				return &ToolObjectSelection;
			case ModelEditorMode::ObjectFaceSelection:
				return &ToolObjectFaceSelection;
			case ModelEditorMode::ObjectVertexSelection:
				return &ToolObjectVertexSelection;
			case ModelEditorMode::ObjectTranslation:
				return &ToolObjectTranslation;
			case ModelEditorMode::ObjectRotation:
				return &ToolObjectRotation;
			case ModelEditorMode::ObjectScaling:
				return &ToolObjectScaling;
			default:
				return 0;
			}
		default:
			return 0;
		}
	}

	ModelEditorWindow::ToolMessageProc ChooseIdleMouseProc(ModelEditorWindow* editorWindow)
	{
		switch(editorWindow->modelEditorData.modelEditorOperation)
		{
		case ModelEditorOperation::ObjectEditing:
			switch(editorWindow->modelEditorData.modelEditorMode)
			{
			case ModelEditorMode::ObjectFaceSelection:
				return &IdleObjectFaceSelection;
			case ModelEditorMode::ObjectVertexSelection:
				return &IdleObjectVertexSelection;
			default:
				return 0;
			}
		default:
			return 0;
		}
	}

	LRESULT CALLBACK EditorWindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		ModelEditorWindow* editorWindow=(ModelEditorWindow*)dwRefData;
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			return 0;
		case WM_SIZE:
			editorWindow->Resize();
			editorWindow->Render();
			break;
		case WM_KEYDOWN:
			{
				WindowKeyInfo info(wParam, lParam);
				switch(info.key)
				{
				case VK_CONTROL:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::None:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewMoving;
							break;
						}
					}
					break;
				case VK_SHIFT:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::None:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewZooming;
							break;
						}
					}
					break;
				case 'C':
					EditorModeSelection(editorWindow);
					break;
				case 'F':
					EditorModeFaceSelection(editorWindow);
					break;
				case 'V':
					EditorModeVertexSelection(editorWindow);
					break;
				case 'T':
					EditorModeTranslation(editorWindow);
					break;
				case 'R':
					EditorModeRotation(editorWindow);
					break;
				case 'S':
					EditorModeScaling(editorWindow);
					break;
				case 'G':
					EditorAxisGlobal(editorWindow);
					break;
				case 'L':
					EditorAxisLocal(editorWindow);
					break;
				case 'X':
					editorWindow->SetEditorAxisDirection(ModelEditorAxisDirection::X);
					editorWindow->Render();
					break;
				case 'Y':
					editorWindow->SetEditorAxisDirection(ModelEditorAxisDirection::Y);
					editorWindow->Render();
					break;
				case 'Z':
					editorWindow->SetEditorAxisDirection(ModelEditorAxisDirection::Z);
					editorWindow->Render();
					break;
				}
			}
			break;
		case WM_KEYUP:
			{
				WindowKeyInfo info(wParam, lParam);
				switch(info.key)
				{
				case VK_CONTROL:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::ViewMoving:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
							break;
						}
					}
					break;
				case VK_SHIFT:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::ViewZooming:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
							break;
						}
					}
					break;
				case 'X':
				case 'Y':
				case 'Z':
					editorWindow->SetEditorAxisDirection(ModelEditorAxisDirection::None);
					editorWindow->Render();
					break;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				SetFocus(hWnd);
				if(editorWindow->modelEditorData.modelEditorOperation==ModelEditorOperation::None)
				{
					editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ObjectEditing;
				}

				editorWindow->currentToolMessageProc=ChooseActivatedToolMouseProc(editorWindow);
				if(editorWindow->currentToolMessageProc)
				{
					editorWindow->currentToolMessageProc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				if(editorWindow->currentToolMessageProc)
				{
					editorWindow->currentToolMessageProc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
				editorWindow->currentToolMessageProc=0;

				if(editorWindow->modelEditorData.modelEditorOperation==ModelEditorOperation::ObjectEditing)
				{
					editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
				}
			}
			break;
		case WM_RBUTTONDOWN:
			{
				if(editorWindow->modelEditorData.modelEditorOperation==ModelEditorOperation::None)
				{
					editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewRotation;
				}

				editorWindow->currentToolMessageProc=ChooseActivatedToolMouseProc(editorWindow);
				if(editorWindow->currentToolMessageProc)
				{
					editorWindow->currentToolMessageProc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
			}
			break;
		case WM_RBUTTONUP:
			{
				if(editorWindow->currentToolMessageProc)
				{
					editorWindow->currentToolMessageProc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
				editorWindow->currentToolMessageProc=0;

				if(editorWindow->modelEditorData.modelEditorOperation==ModelEditorOperation::ViewRotation)
				{
					editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				if(editorWindow->currentToolMessageProc)
				{
					editorWindow->currentToolMessageProc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
				else
				{
					ModelEditorWindow::ToolMessageProc proc=ChooseIdleMouseProc(editorWindow);
					if(proc)
					{
						proc(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
					}
				}
			}
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}