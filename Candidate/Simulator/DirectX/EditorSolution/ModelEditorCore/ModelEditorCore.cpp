#include "ModelEditorCore.h"
#include "..\Shared\DirectXSetup.h"
#include "..\Shared\DirectXShader.h"
#include "..\Shared\WindowSetup.h"

#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

using namespace vl;
using namespace vl::directx;

extern "C"
{
	struct ModelEditorWindow
	{
		HWND							editorControl;
		UINT							subclass;

		DirectXEnvironment*				env;
		DirectXDepthBuffer*				depthBuffer;
		DirectXWindowRenderTarget*		renderTarget;
		DirectXRenderer*				renderer;
		DirectXViewport*				viewport;

		ModelEditorWindow(HWND _editorControl)
			:editorControl(_editorControl)
			,subclass(0)
			,env(0)
			,depthBuffer(0)
			,renderTarget(0)
			,renderer(0)
			,viewport(0)
		{
			SIZE size=WindowGetClient(editorControl);
			env=new DirectXEnvironment(editorControl, size.cx, size.cy);

			depthBuffer=new DirectXDepthBuffer(env);
			renderTarget=new DirectXWindowRenderTarget(env);
			renderer=new DirectXRenderer(env);
			viewport=new DirectXViewport(env);
			
			depthBuffer->Update(size.cx, size.cy);
			renderer->SetRenderTarget(renderTarget, depthBuffer);

			Render();
		}

		~ModelEditorWindow()
		{
			delete viewport;
			delete renderer;
			delete renderTarget;
			delete depthBuffer;
			delete env;
		}

		void Render()
		{
			SIZE size=WindowGetClient(editorControl);
			viewport->SetViewport(0, 0, size.cx, size.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
			depthBuffer->Clear();
			renderTarget->Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
			env->swapChain->Present(0, 0);
		}
	};
}

namespace modeleditor
{
	LRESULT CALLBACK EditorWindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		ModelEditorWindow* editorWindow=(ModelEditorWindow*)dwRefData;
		switch(uMsg)
		{
		case WM_ERASEBKGND:
		case WM_PAINT:
			return 0;
		case WM_SIZE:
			editorWindow->Render();
			break;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}
using namespace modeleditor;

extern "C"
{
	MODELEDITORCORE_API ModelEditorWindow* __stdcall CreateEditorWindow(HWND editorControl)
	{
		ModelEditorWindow* editorWindow=new ModelEditorWindow(editorControl);
		SetWindowSubclass(editorControl, &EditorWindowSubclassProc, (UINT_PTR)&editorWindow->subclass,(DWORD_PTR)editorWindow);
		return editorWindow;
	}

	MODELEDITORCORE_API void __stdcall DestroyEditorWindow(ModelEditorWindow* window)
	{
		RemoveWindowSubclass(window->editorControl, &EditorWindowSubclassProc, (UINT_PTR)&window->subclass);
		delete window;
	}
}