#include "ModelEditorCore.h"
#include "Core\ModelEditorRenderer.h"
#include "Core\ModelBuilder.h"

#include <CommCtrl.h>

using namespace modeleditor;

namespace modeleditor
{
	LRESULT CALLBACK EditorWindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		ModelEditorWindow* editorWindow=(ModelEditorWindow*)dwRefData;
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			return 0;
		case WM_SIZE:
			editorWindow->Resize();
			editorWindow->Render();
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}
using namespace modeleditor;

extern "C"
{
	MODELEDITORCORE_API ModelEditorWindow* __stdcall CreateEditorWindow(HWND editorControl, const wchar_t* workingDirectory)
	{
		ModelEditorWindow* editorWindow=new ModelEditorWindow(editorControl, workingDirectory);
		SetWindowSubclass(editorControl, &EditorWindowSubclassProc, (UINT_PTR)&editorWindow->subclass,(DWORD_PTR)editorWindow);
		return editorWindow;
	}

	MODELEDITORCORE_API void __stdcall DestroyEditorWindow(ModelEditorWindow* window)
	{
		RemoveWindowSubclass(window->editorControl, &EditorWindowSubclassProc, (UINT_PTR)&window->subclass);
		delete window;
	}

	MODELEDITORCORE_API void __stdcall RenderEditorWindow(ModelEditorWindow* window)
	{
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall DestroyModel(ModelEditorWindow* window, Model* model)
	{
		window->RemoveModel(model);
	}

	MODELEDITORCORE_API Model* __stdcall CreateModelCube(ModelEditorWindow* window)
	{
		Model* model=new Model(window->Env());
		BuildCube(model);
		model->Update();
		window->AddModel(model);
		return model;
	}
}