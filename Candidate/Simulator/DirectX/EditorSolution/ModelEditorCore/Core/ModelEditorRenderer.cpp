#include "ModelEditorRenderer.h"
#include "..\..\Shared\WindowSetup.h"

namespace modeleditor
{
/***********************************************************************
ModelEditorWindow
***********************************************************************/

	void ModelEditorWindow::UpdateConstantBuffer()
	{
		D3DXMATRIX worldMatrix, viewMatrix;
		{
			D3DXMatrixIdentity(&worldMatrix);
		}
		{
			D3DXVECTOR3 at(-1, -1, -1);
			D3DXVECTOR3 up(-1, 1, -1);
			D3DXVec3Normalize(&at, &at);
			D3DXVec3Normalize(&up, &up);
			D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(20.0f, 20.0f, 20.0f), &at, &up);
		}

		D3DXMatrixTranspose(&(*constantBuffer)->worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&(*constantBuffer)->viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&(*constantBuffer)->projectionMatrix, &viewport->projectionMatrix);
		constantBuffer->Update();
	}

	void ModelEditorWindow::UpdateGeometryAxis()
	{
		const int size=16;
		VertexAxis vertices[(size+1)*4];
		unsigned int indices[(size+1)*4];
		int currentVertex=0;
		
		for(int i=-size/2;i<=size/2;i++)
		{
			vertices[currentVertex++].position=D3DXVECTOR3((float)i, 0, -size/2);
			vertices[currentVertex++].position=D3DXVECTOR3((float)i, 0, size/2);
		}
		for(int i=-size/2;i<=size/2;i++)
		{
			vertices[currentVertex++].position=D3DXVECTOR3(-size/2, 0, (float)i);
			vertices[currentVertex++].position=D3DXVECTOR3(size/2, 0, (float)i);
		}
		for(int i=0;i<(size+1)*4;i++)
		{
			indices[i]=i;
		}
		geometryAxis->Fill(vertices, indices);
	}

	ModelEditorWindow::ModelEditorWindow(HWND _editorControl, const WString& _workingDirectory)
		:editorControl(_editorControl)
		,subclass(0)
		,env(0)
		,workingDirectory(_workingDirectory)
		,depthBuffer(0)
		,renderTarget(0)
		,renderer(0)
		,viewport(0)
		,constantBuffer(0)
		,geometryAxis(0)
		,shaderAxis(0)
	{
		clientSize=WindowGetClient(editorControl);
		env=new DirectXEnvironment(editorControl, clientSize.cx, clientSize.cy);

		depthBuffer=new DirectXDepthBuffer(env);
		renderTarget=new DirectXWindowRenderTarget(env);
		renderer=new DirectXRenderer(env);
		viewport=new DirectXViewport(env);

		constantBuffer=new DirectXConstantBuffer<ConstantBuffer>(env);
		geometryAxis=new DirectXVertexBuffer<VertexAxis>(env);
		shaderAxis=new DirectXShader<VertexAxis>(env);
		shaderAxis->Fill(workingDirectory+L"Shaders\\AxisShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &VertexAxis::position)
			;
			
		depthBuffer->Update(clientSize.cx, clientSize.cy);
		renderer->SetRenderTarget(renderTarget, depthBuffer);

		Render();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		delete shaderAxis;
		delete geometryAxis;
		delete constantBuffer;
		delete viewport;
		delete renderer;
		delete renderTarget;
		delete depthBuffer;
		delete env;
	}

	void ModelEditorWindow::Resize()
	{
		clientSize=WindowGetClient(editorControl);
		renderer->SetRenderTarget(0, 0);
		delete renderTarget;
		delete depthBuffer;

		renderTarget=new DirectXWindowRenderTarget(env);
		depthBuffer=new DirectXDepthBuffer(env);
		depthBuffer->Update(clientSize.cx, clientSize.cy);
		env->Resize(clientSize.cx, clientSize.cy);
		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	void ModelEditorWindow::Render()
	{
		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		renderTarget->Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

		UpdateConstantBuffer();
		UpdateGeometryAxis();
		constantBuffer->VSBindToRegisterBN(0);
		constantBuffer->PSBindToRegisterBN(0);
		geometryAxis->SetCurrentAndRender(shaderAxis, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		env->swapChain->Present(0, 0);
	}
}