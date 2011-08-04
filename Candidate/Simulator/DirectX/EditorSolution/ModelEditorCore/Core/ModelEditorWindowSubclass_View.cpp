#include "ModelEditorWindowSubclass.h"
#include "..\..\Shared\WindowSetup.h"
#include "..\ModelEditorCore.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

/***********************************************************************
ModelEditorOperation::ViewRotation
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
}