#include "ModelEditorCore.h"
#include "Core\ModelEditorRenderer.h"
#include "Core\ModelBuilder.h"
#include "..\Shared\WindowSetup.h"

#include <CommCtrl.h>

using namespace vl;
using namespace vl::directx;
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
		case WM_SIZE:
			editorWindow->Resize();
			editorWindow->Render();
			break;
		case WM_KEYDOWN:
			{
				WindowKeyInfo info(wParam, lParam);
				switch(info.key)
				{
				case VK_CONTROL:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::None:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewMoving;
							break;
						}
					}
					break;
				case VK_SHIFT:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::None:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewZooming;
							break;
						}
					}
					break;
				case 'C':
					EditorModeSelection(editorWindow);
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
				}
			}
			break;
		case WM_KEYUP:
			{
				WindowKeyInfo info(wParam, lParam);
				switch(info.key)
				{
				case VK_CONTROL:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::ViewMoving:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
							break;
						}
					}
					break;
				case VK_SHIFT:
					{
						switch(editorWindow->modelEditorData.modelEditorOperation)
						{
						case ModelEditorOperation::ViewZooming:
							editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
							break;
						}
					}
					break;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				SetFocus(hWnd);
				switch(editorWindow->modelEditorData.modelEditorMode)
				{
				case ModelEditorMode::ObjectSelection:
					{
						WindowMouseInfo info(wParam, lParam, false);
						int index=editorWindow->QueryModel(info.x, info.y);
						editorWindow->SelectModel(index);
						editorWindow->Render();
					}
					break;
				}
			}
			break;
		case WM_RBUTTONDOWN:
			{
				WindowMouseInfo info(wParam, lParam, false);
				switch(editorWindow->modelEditorData.modelEditorOperation)
				{
				case ModelEditorOperation::None:
					{
						editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::ViewRotation;
						editorWindow->modelEditorData.modelEditorOperationActivated=true;
						editorWindow->modelEditorData.originX=info.x;
						editorWindow->modelEditorData.originY=info.y;
					}
					break;
				case ModelEditorOperation::ViewZooming:
				case ModelEditorOperation::ViewMoving:
					{
						editorWindow->modelEditorData.modelEditorOperationActivated=true;
						editorWindow->modelEditorData.originX=info.x;
						editorWindow->modelEditorData.originY=info.y;
					}
					break;
				}
			}
			break;
		case WM_RBUTTONUP:
			{
				WindowMouseInfo info(wParam, lParam, false);
				switch(editorWindow->modelEditorData.modelEditorOperation)
				{
				case ModelEditorOperation::ViewRotation:
					{
						editorWindow->modelEditorData.modelEditorOperation=ModelEditorOperation::None;
						editorWindow->modelEditorData.modelEditorOperationActivated=false;
					}
					break;
				case ModelEditorOperation::ViewZooming:
				case ModelEditorOperation::ViewMoving:
					{
						editorWindow->modelEditorData.modelEditorOperationActivated=false;
					}
					break;
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				WindowMouseInfo info(wParam, lParam, false);
				if(editorWindow->modelEditorData.modelEditorOperationActivated)
				{
					switch(editorWindow->modelEditorData.modelEditorOperation)
					{
					case ModelEditorOperation::ViewRotation:
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
					case ModelEditorOperation::ViewZooming:
						{
							int deltaY=info.y-editorWindow->modelEditorData.originY;
							float distance=(float)deltaY/20;

							editorWindow->ViewMove(0, 0, distance);
							editorWindow->Render();

							editorWindow->modelEditorData.originX=info.x;
							editorWindow->modelEditorData.originY=info.y;
						}
						break;
					case ModelEditorOperation::ViewMoving:
						{
							int deltaX=info.x-editorWindow->modelEditorData.originX;
							int deltaY=info.y-editorWindow->modelEditorData.originY;

							if(deltaX!=0 || deltaY!=0)
							{
								editorWindow->ViewMove((float)deltaX/20, (float)deltaY/20, 0);
								editorWindow->Render();
							}

							editorWindow->modelEditorData.originX=info.x;
							editorWindow->modelEditorData.originY=info.y;
						}
						break;
					}
				}
			}
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
		model->Update();
		window->AddModel(model);
		return model;
	}

	MODELEDITORCORE_API Model* __stdcall CreateModelAxis(ModelEditorWindow* window)
	{
		return CreateModel(window, BuildAxis);
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