#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

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
}