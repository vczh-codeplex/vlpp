#include "ModelEditorRenderer.h"
#include "..\..\Shared\WindowSetup.h"

namespace modeleditor
{
/***********************************************************************
ModelEditorWindow
***********************************************************************/

	ModelEditorWindow::ModelEditorWindow(HWND _editorControl)
		:editorControl(_editorControl)
		,subclass(0)
		,env(0)
		,depthBuffer(0)
		,renderTarget(0)
		,renderer(0)
		,viewport(0)
	{
		clientSize=WindowGetClient(editorControl);
		env=new DirectXEnvironment(editorControl, clientSize.cx, clientSize.cy);

		depthBuffer=new DirectXDepthBuffer(env);
		renderTarget=new DirectXWindowRenderTarget(env);
		renderer=new DirectXRenderer(env);
		viewport=new DirectXViewport(env);
			
		Resize();
		Render();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		delete viewport;
		delete renderer;
		delete renderTarget;
		delete depthBuffer;
		delete env;
	}

	void ModelEditorWindow::Resize()
	{
		clientSize=WindowGetClient(editorControl);
		depthBuffer->Update(clientSize.cx, clientSize.cy);
		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	void ModelEditorWindow::Render()
	{
		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		renderTarget->Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		env->swapChain->Present(0, 0);
	}
}