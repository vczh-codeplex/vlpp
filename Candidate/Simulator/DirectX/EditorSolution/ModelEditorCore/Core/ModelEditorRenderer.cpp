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
		const float cellSize=2.0f;
		const float centerSize=0.2f;
		VertexAxis vertices[(size+3)*4];
		unsigned int indices[(size+3)*4];
		int currentVertex=0;
		
		for(int i=-size/2;i<=size/2;i++)
		{
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*i, 0, -cellSize*size/2);
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*i, 0, cellSize*size/2);
		}
		for(int i=-size/2;i<=size/2;i++)
		{
			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*size/2, 0, cellSize*i);
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*size/2, 0, cellSize*i);
		}
		{
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*centerSize, 0, -cellSize*size/2);
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*centerSize, 0, cellSize*size/2);
			
			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*centerSize, 0, -cellSize*size/2);
			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*centerSize, 0, cellSize*size/2);

			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*size/2, 0, cellSize*centerSize);
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*size/2, 0, cellSize*centerSize);
			
			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*size/2, 0, -cellSize*centerSize);
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*size/2, 0, -cellSize*centerSize);
		}
		for(int i=0;i<(size+3)*4;i++)
		{
			indices[i]=i;
		}
		geometryAxis->Fill(vertices, indices);
	}

	void ModelEditorWindow::Initialize()
	{
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
	}

	template<typename T>
	void DeleteAndZero(T*& object)
	{
		delete object;
		object=0;
	}

	void ModelEditorWindow::Finalize()
	{
		DeleteAndZero(shaderAxis);
		DeleteAndZero(geometryAxis);
		DeleteAndZero(constantBuffer);
		DeleteAndZero(viewport);
		DeleteAndZero(renderer);
		DeleteAndZero(renderTarget);
		DeleteAndZero(depthBuffer);
		DeleteAndZero(env);
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
		Initialize();
		Render();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		Finalize();
	}

	void ModelEditorWindow::Resize()
	{
		clientSize=WindowGetClient(editorControl);
		Finalize();
		Initialize();
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