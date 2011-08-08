#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

#include <CommCtrl.h>

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
ModelEditorOperation
***********************************************************************/

	extern void ToolViewRotation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);

	extern void ToolObjectSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void ToolObjectFaceSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void IdleObjectFaceSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void ToolObjectVertexSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void IdleObjectVertexSelection(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);

	extern void ToolObjectTranslation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void ToolObjectRotation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void ToolObjectScaling(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);
	extern void ToolObjectPushing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow);

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

	D3DXVECTOR3 ToolObjectTransform(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& inverseMatrix, const D3DXMATRIX& transformation, D3DXVECTOR3 modelVertex)
	{
		D3DXVECTOR4 temp;
		D3DXVec3Transform(&temp, &modelVertex, &worldMatrix);
		D3DXVec4Transform(&temp, &temp, &transformation);
		D3DXVec4Transform(&temp, &temp, &inverseMatrix);
		return D3DXVECTOR3(temp.x/temp.w, temp.y/temp.w, temp.z/temp.w);
	}

/***********************************************************************
EditorWindowSubclassProc
***********************************************************************/

	ModelEditorWindow::ToolMessageProc ChooseActivatedToolMouseProc(ModelEditorWindow* editorWindow)
	{
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
		case ModelEditorMode::ObjectPushing:
			return &ToolObjectPushing;
		default:
			return 0;
		}
	}

	ModelEditorWindow::ToolMessageProc ChooseIdleMouseProc(ModelEditorWindow* editorWindow)
	{
		switch(editorWindow->modelEditorData.modelEditorMode)
		{
		case ModelEditorMode::ObjectFaceSelection:
			return &IdleObjectFaceSelection;
		case ModelEditorMode::ObjectVertexSelection:
			return &IdleObjectVertexSelection;
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
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
				editorWindow->currentToolMessageProc=&ToolViewRotation;
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
			}
			break;
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			{
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				editorWindow->currentToolMessageProc=0;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
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