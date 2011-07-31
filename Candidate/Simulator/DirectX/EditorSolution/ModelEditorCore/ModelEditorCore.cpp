#include "ModelEditorCore.h"
#include "Core\ModelBuilder.h"
#include "Core\ModelEditorWindow.h"
#include "Core\ModelEditorWindowSubclass.h"

#include <CommCtrl.h>

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
		SendMessage(window->editorControl, WM_PAINT, 0, 0);
	}

	MODELEDITORCORE_API void __stdcall DestroyModel(ModelEditorWindow* window, Model* model)
	{
		window->RemoveModel(model);
	}

	Model* CreateModel(ModelEditorWindow* window, void(*modelBuilder)(Model*))
	{
		Model* model=new Model(window->Env());
		modelBuilder(model);
		model->UpdateVertexBuffer();
		window->AddModel(model);
		return model;
	}

	MODELEDITORCORE_API Model* __stdcall CreateModelCube(ModelEditorWindow* window)
	{
		return CreateModel(window, BuildCube);
	}

	MODELEDITORCORE_API void __stdcall ResetView(ModelEditorWindow* window)
	{
		window->ViewReset();
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorModeSelection(ModelEditorWindow* window)
	{
		window->SetEditorMode(ModelEditorMode::ObjectSelection);
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorModeTranslation(ModelEditorWindow* window)
	{
		window->SetEditorMode(ModelEditorMode::ObjectTranslation);
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorModeRotation(ModelEditorWindow* window)
	{
		window->SetEditorMode(ModelEditorMode::ObjectRotation);
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorModeScaling(ModelEditorWindow* window)
	{
		window->SetEditorMode(ModelEditorMode::ObjectScaling);
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorAxisGlobal(ModelEditorWindow* window)
	{
		window->SetEditorAxis(ModelEditorAxis::AxisGlobal);
		window->Render();
	}

	MODELEDITORCORE_API void __stdcall EditorAxisLocal(ModelEditorWindow* window)
	{
		window->SetEditorAxis(ModelEditorAxis::AxisLocal);
		window->Render();
	}
}