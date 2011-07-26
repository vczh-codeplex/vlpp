#include "ModelEditorRenderer.h"
#include "ModelBuilder.h"
#include "..\..\Shared\WindowSetup.h"

namespace modeleditor
{

	template<typename T>
	void DeleteAndZero(T*& object)
	{
		delete object;
		object=0;
	}

/***********************************************************************
Model
***********************************************************************/

	Model::Model(DirectXEnvironment* _env)
		:env(0)
		,geometry(0)
		,selected(false)
	{
		D3DXMatrixIdentity(&worldMatrix);
		Rebuild(_env);
	}

	Model::~Model()
	{
		delete geometry;
	}

	void Model::Update()
	{
		if(vertices.Count()>0 && indices.Count()>0)
		{
			geometry->Fill(&vertices[0], vertices.Count(), &indices[0], indices.Count());
		}
	}

	void Model::Rebuild(DirectXEnvironment* _env)
	{
		env=_env;
		delete geometry;
		geometry=new DirectXVertexBuffer<VertexObject>(env);
		Update();
	}

	DirectXVertexBuffer<VertexObject>* Model::Geometry()
	{
		return geometry;
	}

/***********************************************************************
ModelEditorData
***********************************************************************/

	ModelEditorData::ModelEditorData()
		:originX(0)
		,originY(0)
		,modelEditorOperation(ModelEditorOperation::None)
		,modelEditorOperationActivated(false)
	{
	}

	ModelEditorData::~ModelEditorData()
	{
	}

/***********************************************************************
ModelEditorWindow
***********************************************************************/

	void ModelEditorWindow::ViewCalculateDirection()
	{
		D3DXMATRIX v, h, m;
		D3DXMatrixRotationX(&v, viewAngleVertical);
		D3DXMatrixRotationY(&h, viewAngleHorizontal);
		D3DXMatrixMultiply(&m, &v, &h);

		D3DXVECTOR4 front(0, 0, 1, 1);
		D3DXVECTOR4 up(0, 1, 0, 1);
		D3DXVECTOR4 left(-1, 0, 0, 1);
		D3DXVec4Transform(&front, &front, &m);
		D3DXVec4Transform(&up, &up, &m);
		D3DXVec4Transform(&left, &left, &m);

		viewFront=D3DXVECTOR3(front.x/front.w, front.y/front.w, front.z/front.w);
		viewUp=D3DXVECTOR3(up.x/up.w, up.y/up.w, up.z/up.w);
		viewLeft=D3DXVECTOR3(left.x/left.w, left.y/left.w, left.z/left.w);
	}

	void ModelEditorWindow::UpdateConstantBuffer(const D3DXMATRIX& worldMatrix)
	{
		D3DXMATRIX viewMatrix;
		{
			D3DXVECTOR3 eye=D3DXVECTOR3(
				viewAt.x-viewFront.x*viewDistance,
				viewAt.y-viewFront.y*viewDistance,
				viewAt.z-viewFront.z*viewDistance
				);
			D3DXMatrixLookAtLH(&viewMatrix, &eye, &viewAt, &viewUp);
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
		VertexAxis vertices[(size+1)*4+2];
		unsigned int indices[(size+3)*4];
		int currentVertex=0;
		
		for(int i=-size/2;i<=size/2;i++)
		{
			D3DXCOLOR color=(i==0?D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f):D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*i, 0, -cellSize*size/2);
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*i, 0, cellSize*size/2);
		}
		for(int i=-size/2;i<=size/2;i++)
		{
			D3DXCOLOR color=(i==0?D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f):D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(-cellSize*size/2, 0, cellSize*i);
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(cellSize*size/2, 0, cellSize*i);
		}
		{
			D3DXCOLOR color=D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(0, -cellSize*size/2, 0);
			vertices[currentVertex].color=color;
			vertices[currentVertex++].position=D3DXVECTOR3(0, cellSize*size/2, 0);
		}
		for(int i=0;i<(size+3)*4;i++)
		{
			indices[i]=i;
		}
		geometryAxisLine->Fill(vertices, indices);
	}

	void ModelEditorWindow::Initialize()
	{
		env=new DirectXEnvironment(editorControl, clientSize.cx, clientSize.cy);

		depthBuffer=new DirectXDepthBuffer(env);
		renderTarget=new DirectXWindowRenderTarget(env);
		renderer=new DirectXRenderer(env);
		viewport=new DirectXViewport(env);
		selectorStagingBuffer=new DirectXTextureBuffer(env);
		selectorBuffer=new DirectXTextureBuffer(env);
		selectorRenderTarget=new DirectXTextureRenderTarget(env);

		constantBuffer=new DirectXConstantBuffer<ConstantBuffer>(env);
		geometryAxisLine=new DirectXVertexBuffer<VertexAxis>(env);
		geometryAxisObject = new Model(env);
		BuildAxis(geometryAxisObject);
		geometryAxisObject->Update();
		shaderAxis=new DirectXShader<VertexAxis>(env);
		shaderAxis->Fill(workingDirectory+L"Shaders\\AxisShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &VertexAxis::position)
			.Field(L"COLOR", &VertexAxis::color)
			;
		shaderObject=new DirectXShader<VertexObject>(env);
		shaderObject->Fill(workingDirectory+L"Shaders\\ObjectShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &VertexObject::position)
			.Field(L"NORMAL", &VertexObject::normal)
			.Field(L"COLOR", &VertexObject::color)
			.Field(L"TEXCOORD", &VertexObject::id)
			;
		shaderSelector=new DirectXShader<VertexObject>(env);
		shaderSelector->Fill(workingDirectory+L"Shaders\\SelectorShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &VertexObject::position)
			.Field(L"NORMAL", &VertexObject::normal)
			.Field(L"COLOR", &VertexObject::color)
			.Field(L"TEXCOORD", &VertexObject::id)
			;
			
		depthBuffer->Update(clientSize.cx, clientSize.cy);
		selectorStagingBuffer->UpdateUint(clientSize.cx, clientSize.cy, true);
		selectorBuffer->UpdateUint(clientSize.cx, clientSize.cy, false);
		selectorRenderTarget->Update(selectorBuffer);
		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	void ModelEditorWindow::Finalize()
	{
		DeleteAndZero(shaderSelector);
		DeleteAndZero(shaderObject);
		DeleteAndZero(shaderAxis);
		DeleteAndZero(geometryAxisObject);
		DeleteAndZero(geometryAxisLine);
		DeleteAndZero(constantBuffer);
		DeleteAndZero(selectorRenderTarget);
		DeleteAndZero(selectorBuffer);
		DeleteAndZero(selectorStagingBuffer);
		DeleteAndZero(viewport);
		DeleteAndZero(renderer);
		DeleteAndZero(renderTarget);
		DeleteAndZero(depthBuffer);
		DeleteAndZero(env);
	}

	void ModelEditorWindow::RebuildModels()
	{
		for(int i=0;i<models.Count();i++)
		{
			models[i]->Rebuild(env);
		}
	}

	void ModelEditorWindow::RenderSelector()
	{
		renderer->SetRenderTarget(selectorRenderTarget, depthBuffer);
		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		selectorRenderTarget->Clear(D3DXCOLOR((UINT)0));
		
		constantBuffer->VSBindToRegisterBN(0);
		constantBuffer->PSBindToRegisterBN(0);

		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			UpdateConstantBuffer(model->worldMatrix);
			model->Geometry()->SetCurrentAndRender(shaderSelector);
		}
		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	void ModelEditorWindow::RenderSelectorModelIndexIncremented()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			for(int j=0;j<model->vertices.Count();j++)
			{
				model->vertices[j].id=i+1;
			}
			model->Update();
		}
		RenderSelector();
	}

	void ModelEditorWindow::RenderSelectorSelected()
	{
		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			unsigned __int32 id=model->selected?1:0;
			for(int j=0;j<model->vertices.Count();j++)
			{
				model->vertices[j].id=id;
			}
			model->Update();
		}
		RenderSelector();
	}

	unsigned __int32 ModelEditorWindow::GetSelectorResult(int x, int y)
	{
		if(x<0 || x>=clientSize.cx || y<0 || y>=clientSize.cy)
		{
			return 0;
		}
		ID3D11Texture2D* texture=selectorBuffer->RawTexture();
		ID3D11Texture2D* dest=selectorStagingBuffer->RawTexture();
		
		HRESULT hr=S_OK;

		env->context->CopyResource(dest, texture);
		unsigned __int32 result=0;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));
		hr=env->context->Map(dest, 0, D3D11_MAP_READ, 0, &mappedResource);
		result=*(unsigned __int32*)((char*)mappedResource.pData + y*mappedResource.RowPitch + x*sizeof(unsigned __int32));
		env->context->Unmap(dest, 0);
		return result;
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
		,selectorStagingBuffer(0)
		,selectorBuffer(0)
		,selectorRenderTarget(0)
		,constantBuffer(0)
		,geometryAxisLine(0)
		,geometryAxisObject(0)
		,shaderAxis(0)
		,shaderObject(0)
		,shaderSelector(0)
	{
		clientSize=WindowGetClient(editorControl);
		Initialize();
		ViewReset();
	}

	ModelEditorWindow::~ModelEditorWindow()
	{
		Finalize();
	}

	DirectXEnvironment* ModelEditorWindow::Env()
	{
		return env;
	}

	void ModelEditorWindow::AddModel(Model* model)
	{
		models.Add(model);
	}

	void ModelEditorWindow::RemoveModel(Model* model)
	{
		models.Remove(model);
	}

	int ModelEditorWindow::ModelCount()
	{
		return models.Count();
	}

	Model* ModelEditorWindow::GetModel(int index)
	{
		return models[index].Obj();
	}

	void ModelEditorWindow::Resize()
	{
		clientSize=WindowGetClient(editorControl);
		Finalize();
		Initialize();
		RebuildModels();
	}

	void ModelEditorWindow::Render()
	{
		RenderSelectorSelected();

		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		renderTarget->Clear(D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		
		constantBuffer->VSBindToRegisterBN(0);
		constantBuffer->PSBindToRegisterBN(0);
		selectorBuffer->PSBindToRegisterTN(0);

		for(int i=0;i<models.Count();i++)
		{
			Model* model=models[i].Obj();
			UpdateConstantBuffer(model->worldMatrix);
			model->Geometry()->SetCurrentAndRender(shaderObject);
		}
		{
			D3DXMATRIX axisWorldMatrix;
			D3DXMatrixIdentity(&axisWorldMatrix);
			UpdateConstantBuffer(axisWorldMatrix);
			UpdateGeometryAxis();
			geometryAxisLine->SetCurrentAndRender(shaderAxis, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}
		{
			viewport->SetViewport(0, clientSize.cy-100, 100, 100, (float)D3DX_PI/2, 0.1f, 1000.0f);
			depthBuffer->Clear();

			D3DXMATRIX axisWorldMatrix, axisTranslation;
			D3DXMatrixScaling(&axisWorldMatrix, 2.0f, 2.0f, 2.0f);
			D3DXMatrixTranslation(&axisTranslation, viewAt.x, viewAt.y, viewAt.z);
			D3DXMatrixMultiply(&axisWorldMatrix, &axisWorldMatrix, &axisTranslation);
			UpdateConstantBuffer(axisWorldMatrix);
			geometryAxisObject->Geometry()->SetCurrentAndRender(shaderObject);
		}

		env->swapChain->Present(0, 0);
	}

	int ModelEditorWindow::QueryModel(int x, int y)
	{
		RenderSelectorModelIndexIncremented();
		unsigned __int32 result=GetSelectorResult(x, y);
		return (int)result-1;
	}

	void ModelEditorWindow::SelectModel(int index)
	{
		for(int i=0;i<models.Count();i++)
		{
			models[i]->selected=i==index;
		}
	}

	void ModelEditorWindow::ViewReset()
	{
		viewAt=D3DXVECTOR3(0, 0, 0);
		viewAngleVertical=asinf(1/1.732f);
		viewAngleHorizontal=-(float)D3DX_PI*3/4;
		viewDistance=20;
		ViewCalculateDirection();
	}

	void ModelEditorWindow::ViewRotateVertical(float angle)
	{
		viewAngleVertical+=angle;
		ViewCalculateDirection();
	}

	void ModelEditorWindow::ViewRotateHorizontal(float angle)
	{
		viewAngleHorizontal+=angle;
		ViewCalculateDirection();
	}

	void ModelEditorWindow::ViewMove(float left, float up, float front)
	{
		viewAt=D3DXVECTOR3(
			viewAt.x + left*viewLeft.x + up*viewUp.x + front*viewFront.x,
			viewAt.y + left*viewLeft.y + up*viewUp.y + front*viewFront.y,
			viewAt.z + left*viewLeft.z + up*viewUp.z + front*viewFront.z
			);
	}
}