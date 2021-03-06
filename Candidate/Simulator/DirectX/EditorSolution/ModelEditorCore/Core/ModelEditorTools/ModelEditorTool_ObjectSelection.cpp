#include "ModelEditorTools.h"
#include "..\..\ModelEditorCore.h"
#include "..\..\..\Shared\WindowSetup.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
MetObjectSelection
***********************************************************************/

	void MetObjectSelection::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONUP:
			{
				int index=editorWindow->QueryModel(info.x, info.y);
				editorWindow->SelectModel(index, info.ctrl);
				editorWindow->Render();
			}
			break;
		}
	}

/***********************************************************************
MetFaceSelection
***********************************************************************/

	void MetFaceSelection::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				editorWindow->StartRanging(info.x, info.y);
				editorWindow->Render();
			}
			break;
		case WM_LBUTTONUP:
			{
				int x=0, y=0, w=0, h=0;
				if(editorWindow->GetRanging(x, y, w, h))
				{
					if(w==1 && h==1)
					{
						int modelIndex=-1;
						int faceIndex=-1;
						if(editorWindow->QueryFace(info.x, info.y, modelIndex, faceIndex))
						{
							editorWindow->SelectFace(modelIndex, faceIndex, info.ctrl);
						}
						else
						{
							editorWindow->SelectModel(-1, info.ctrl);
						}
					}
					else
					{
						editorWindow->SelectFaceFromSelectedModels(x, y, w, h, info.ctrl, !info.shift);
					}
				}
				editorWindow->StopRanging();
				editorWindow->Render();
			}
			break;
		case WM_MOUSEMOVE:
			{
				editorWindow->ContinueRanging(info.x, info.y);
				editorWindow->Render();
			}
			break;
		}
	}

/***********************************************************************
MetVertexSelection
***********************************************************************/

	void MetVertexSelection::Execute(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass)
	{
		WindowMouseInfo info(wParam, lParam, false);
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				editorWindow->StartRanging(info.x, info.y);
				editorWindow->Render();
			}
			break;
		case WM_LBUTTONUP:
			{
				int x=0, y=0, w=0, h=0;
				if(editorWindow->GetRanging(x, y, w, h))
				{
					int modelIndex=-1;
					int vertexIndex=-1;
					if(w==1 && h==1)
					{
						if(editorWindow->QueryVertex(info.x, info.y, modelIndex, vertexIndex))
						{
							editorWindow->SelectVertex(modelIndex, vertexIndex, info.ctrl);
						}
						else
						{
							editorWindow->SelectModel(-1, info.ctrl);
						}
					}
					else
					{
						editorWindow->SelectVertexFromSelectedModels(x, y, w, h, info.ctrl, !info.shift);
					}
				}
				editorWindow->StopRanging();
				editorWindow->Render();
			}
			break;
		case WM_MOUSEMOVE:
			{
				editorWindow->ContinueRanging(info.x, info.y);
				editorWindow->Render();
			}
			break;
		}
	}
}