#ifndef VCZH_DIRECTX_MODELEDITORCORE
#define VCZH_DIRECTX_MODELEDITORCORE

#ifdef MODELEDITORCORE_EXPORTS
#define MODELEDITORCORE_API __declspec(dllexport)
#else
#define MODELEDITORCORE_API __declspec(dllimport)
#endif

#include <Windows.h>

namespace modeleditor
{
	struct ModelEditorWindow;
}
using namespace modeleditor;

extern "C"
{
	MODELEDITORCORE_API ModelEditorWindow*		__stdcall CreateEditorWindow(HWND editorControl);
	MODELEDITORCORE_API void					__stdcall DestroyEditorWindow(ModelEditorWindow* window);
}

#endif