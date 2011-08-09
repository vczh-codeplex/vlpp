#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

#include <CommCtrl.h>

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
ToolViewRotation
***********************************************************************/

	void ToolViewRotation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, ModelEditorWindow* editorWindow)
	{
		WindowMouseInfo info(wParam, lParam, (uMsg==WM_MOUSEWHEEL));
		switch(uMsg)
		{
		case WM_MBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_RBUTTONDOWN:
			{
				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				float distance=(float)info.wheel/120;

				editorWindow->ViewMove(0, 0, distance);
				editorWindow->Render();

				editorWindow->modelEditorData.originX=info.x;
				editorWindow->modelEditorData.originY=info.y;
			}
			break;
		case WM_MOUSEMOVE:
			{
				if(info.right)
				{
					int deltaX=info.x-editorWindow->modelEditorData.originX;
					int deltaY=info.y-editorWindow->modelEditorData.originY;
					SIZE clientSize=WindowGetClient(hWnd);
					int minSize=clientSize.cx<clientSize.cy?clientSize.cx:clientSize.cy;
					
					float vertical=(float)D3DX_PI*2*deltaY/minSize;
					float horizontal=(float)D3DX_PI*2*deltaX/minSize;

					D3DXVECTOR3 center=editorWindow->GetViewAt();
					if(editorWindow->GetMainSelectedModel())
					{
						D3DXVECTOR4 o(0, 0, 0, 1);
						D3DXVec4Transform(&o, &o, &editorWindow->GetMainSelectedModel()->editorInfo.worldMatrix);
						center=D3DXVECTOR3(o.x/o.w, o.y/o.w, o.z/o.w);
					}
					editorWindow->ViewRotate(vertical, horizontal, center);
					editorWindow->Render();

					editorWindow->modelEditorData.originX=info.x;
					editorWindow->modelEditorData.originY=info.y;
				}
				else if(info.middle)
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
			}
			break;
		}
	}

/***********************************************************************
EditorWindowSubclassProc
***********************************************************************/

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
				editorWindow->modelEditorData.executingEditorTool=true;
				editorWindow->currentEditorTool->Execute(hWnd, uMsg, wParam, lParam, uIdSubclass);
			}
			break;
		case WM_LBUTTONUP:
			{
				editorWindow->currentEditorTool->Execute(hWnd, uMsg, wParam, lParam, uIdSubclass);
				editorWindow->modelEditorData.executingEditorTool=false;
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
				editorWindow->modelEditorData.rotatingView=true;
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
			}
			break;
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			{
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				editorWindow->modelEditorData.rotatingView=false;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
			}
			break;
		case WM_MOUSEMOVE:
			{
				if(editorWindow->modelEditorData.rotatingView)
				{
					ToolViewRotation(hWnd, uMsg, wParam, lParam, uIdSubclass, editorWindow);
				}
				else if(editorWindow->modelEditorData.executingEditorTool)
				{
					editorWindow->currentEditorTool->Execute(hWnd, uMsg, wParam, lParam, uIdSubclass);
				}
			}
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}