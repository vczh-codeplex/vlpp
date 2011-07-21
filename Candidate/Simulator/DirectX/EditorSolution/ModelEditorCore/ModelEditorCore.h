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
	class Model;
	class ModelEditorWindow;
}
using namespace modeleditor;

extern "C"
{
	MODELEDITORCORE_API ModelEditorWindow*		__stdcall CreateEditorWindow(HWND editorControl, const wchar_t* workingDirectory);
	MODELEDITORCORE_API void					__stdcall DestroyEditorWindow(ModelEditorWindow* window);
	MODELEDITORCORE_API void					__stdcall RenderEditorWindow(ModelEditorWindow* window);
	
	MODELEDITORCORE_API void					__stdcall DestroyModel(ModelEditorWindow* window, Model* model);
	MODELEDITORCORE_API Model*					__stdcall CreateModelCube(ModelEditorWindow* window);
}

#endif