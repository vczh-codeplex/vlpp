#ifndef VCZH_DIRECTX_MODELEDITORWINDOWSUBCLASS
#define VCZH_DIRECTX_MODELEDITORWINDOWSUBCLASS

#include "ModelEditorWindow.h"

namespace modeleditor
{
	extern LRESULT CALLBACK EditorWindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
}

#endif