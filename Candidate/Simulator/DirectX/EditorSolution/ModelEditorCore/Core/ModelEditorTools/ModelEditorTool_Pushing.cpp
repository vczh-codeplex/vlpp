#include "ModelEditorTools.h"
#include "..\..\ModelEditorCore.h"
#include "..\..\..\Shared\WindowSetup.h"

using namespace vl;
using namespace vl::directx;

namespace modeleditor
{

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
				editorWindow->PushStopModify();
			}
			break;
		case WM_MOUSEMOVE:
			{
				int deltaX=info.x-editorWindow->modelEditorData.originX;
				int deltaY=info.y-editorWindow->modelEditorData.originY;
				SIZE clientSize=WindowGetClient(hWnd);
				float percent=(float)deltaX/(clientSize.cx/2);
				float distance=(float)deltaY/editorWindow->GetViewDistance();

				editorWindow->PushModify(distance, percent);
				editorWindow->Render();
			}
			break;
		}
	}
}