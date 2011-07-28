#include "ModelSceneRenderer.h"
#include "ModelBuilder.h"
#include "..\..\Shared\WindowSetup.h"

namespace modeleditor
{
/***********************************************************************
ModelSceneRenderer
***********************************************************************/

	void ModelSceneRenderer::ViewCalculateDirection()
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

	void ModelSceneRenderer::UpdateConstantBuffer(const D3DXMATRIX& worldMatrix)
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

	void ModelSceneRenderer::UpdateGeometryAxis()
	{
		{
			const int size=32;
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
			geometryAxisLineGlobal->Fill(vertices, indices);
		}
		{
			VertexAxis vertices[6] =
			{
				{D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1, 0, 0, 1)},
				{D3DXVECTOR3(2, 0, 0), D3DXCOLOR(1, 0, 0, 1)},
				{D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 1, 0, 1)},
				{D3DXVECTOR3(0, 2, 0), D3DXCOLOR(0, 1, 0, 1)},
				{D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 1, 1)},
				{D3DXVECTOR3(0, 0, 2), D3DXCOLOR(0, 0, 1, 1)},
			};

			unsigned int indices[6] =
			{
				0, 1,
				2, 3,
				4, 5,
			};

			geometryAxisLineLocal->Fill(vertices, indices);
		}
	}

	void ModelSceneRenderer::Initialize()
	{
		env=new DirectXEnvironment(editorControl, clientSize.cx, clientSize.cy);

		depthBuffer=new DirectXDepthBuffer(env);
		depthBuffer->Update(clientSize.cx, clientSize.cy);
		renderTarget=new DirectXWindowRenderTarget(env);
		renderer=new DirectXRenderer(env);
		viewport=new DirectXViewport(env);
		selectorStagingBuffer=new DirectXTextureBuffer(env);
		selectorStagingBuffer->UpdateUint(clientSize.cx, clientSize.cy, DirectXTextureBuffer::CpuRead);
		selectorBuffer=new DirectXTextureBuffer(env);
		selectorBuffer->UpdateUint(clientSize.cx, clientSize.cy, DirectXTextureBuffer::GpuOnly);
		selectorRenderTarget=new DirectXTextureRenderTarget(env);
		selectorRenderTarget->Update(selectorBuffer);

		constantBuffer=new DirectXConstantBuffer<ConstantBuffer>(env);
		geometryAxisLineGlobal=new DirectXVertexBuffer<VertexAxis>(env);
		geometryAxisLineLocal=new DirectXVertexBuffer<VertexAxis>(env);
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
		shaderSelectedObject=new DirectXShader<VertexObject>(env);
		shaderSelectedObject->Fill(workingDirectory+L"Shaders\\ObjectShader.txt", L"VShader", L"PShaderSelected")
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
		
		if(!editorModeBitmap)
		{
			editorModeBitmap=new WinBitmap(EditorModeTextureSize, EditorModeTextureSize, WinBitmap::vbb32Bits, true);
			editorModeBitmap->GetWinDC()->SetBackTransparent(true);
			editorModeBitmap->GetWinDC()->SetBrush(new WinBrush(RGB(0, 0, 0)));
			editorModeBitmap->GetWinDC()->SetTextColor(RGB(255, 255, 255));
		}
		editorModeTexture=new DirectXTextureBuffer(env);
		editorModeTexture->UpdateRGBA(EditorModeTextureSize, EditorModeTextureSize, DirectXTextureBuffer::CpuWrite);
		UpdateEditorMode();

		editorModeRectangle=new DirectXVertexBuffer<VertexImage>(env);
		{
			VertexImage vertices[4] =
			{
				{D3DXVECTOR3(-1, -1, 0), D3DXVECTOR2( 0,  1)},
				{D3DXVECTOR3( 1, -1, 0), D3DXVECTOR2( 1,  1)},
				{D3DXVECTOR3( 1,  1, 0), D3DXVECTOR2( 1,  0)},
				{D3DXVECTOR3(-1,  1, 0), D3DXVECTOR2( 0,  0)},
			};

			unsigned __int32 indices[6] =
			{
				0, 3, 2,
				0, 2, 1,
			};

			editorModeRectangle->Fill(vertices, indices);
		}
		editorModeShader=new DirectXShader<VertexImage>(env);
		editorModeShader->Fill(workingDirectory+L"Shaders\\ImageShader.txt", L"VShader", L"PShader")
			.Field(L"POSITION", &VertexImage::position)
			.Field(L"TEXCOORD", &VertexImage::texcoord)
			;
		editorModeSampler=new DirectXSamplerBuffer(env);
		editorModeSampler->Update(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_BORDER, D3DXCOLOR(0, 0, 0, 0));

		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	void ModelSceneRenderer::Finalize()
	{
		DeleteAndZero(editorModeSampler);
		DeleteAndZero(editorModeShader);
		DeleteAndZero(editorModeRectangle);
		DeleteAndZero(editorModeTexture);
		DeleteAndZero(shaderSelector);
		DeleteAndZero(shaderSelectedObject);
		DeleteAndZero(shaderObject);
		DeleteAndZero(shaderAxis);
		DeleteAndZero(geometryAxisObject);
		DeleteAndZero(geometryAxisLineLocal);
		DeleteAndZero(geometryAxisLineGlobal);
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

	void ModelSceneRenderer::RenderAxisLine()
	{
		D3DXMATRIX axisWorldMatrix;
		D3DXMatrixIdentity(&axisWorldMatrix);
		UpdateConstantBuffer(axisWorldMatrix);
		UpdateGeometryAxis();
		geometryAxisLineGlobal->SetCurrentAndRender(shaderAxis, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		if(CallbackRenderLocalAxis(axisWorldMatrix))
		{
			depthBuffer->Clear();
			UpdateConstantBuffer(axisWorldMatrix);
			geometryAxisLineLocal->SetCurrentAndRender(shaderAxis, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}
	}

	void ModelSceneRenderer::RenderAxisObject()
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

	void ModelSceneRenderer::UpdateEditorMode()
	{
		WinDC* dc=editorModeBitmap->GetWinDC();
		dc->FillRect(0, 0, EditorModeTextureSize, EditorModeTextureSize);
		CallbackDrawEditorMode(dc);
		
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));
		env->context->Map(editorModeTexture->RawTexture(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		for(int y=0;y<EditorModeTextureSize;y++)
		{
			unsigned char* src=editorModeBitmap->GetScanLines()[y];
			unsigned char* dst=(unsigned char*)mappedResource.pData+y*mappedResource.RowPitch;
			for(int x=0;x<EditorModeTextureSize;x++)
			{
				dst[0]=255;
				dst[1]=255;
				dst[2]=0;
				dst[3]=(src[0]+src[1]+src[2])/3;

				src+=4;
				dst+=4;
			}
		}

		env->context->Unmap(editorModeTexture->RawTexture(), 0);
	}

	void ModelSceneRenderer::RenderEditorMode()
	{
		viewport->SetViewport(0, 0, EditorModeTextureSize, EditorModeTextureSize, (float)D3DX_PI/2, 0.1f, 1000.0f);
		depthBuffer->Clear();

		{
			D3DXMATRIX worldMatrix, viewMatrix;
			D3DXMatrixIdentity(&worldMatrix);
			D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&(*constantBuffer)->worldMatrix, &worldMatrix);
			D3DXMatrixTranspose(&(*constantBuffer)->viewMatrix, &viewMatrix);
			D3DXMatrixTranspose(&(*constantBuffer)->projectionMatrix, &viewport->projectionMatrix);
			constantBuffer->Update();
		}

		editorModeTexture->PSBindToRegisterTN(0);
		editorModeSampler->PSBindToRegisterSN(0);
		editorModeRectangle->SetCurrentAndRender(editorModeShader);
	}

	void ModelSceneRenderer::ToolRenderSelector(bool onlySelected)
	{
		renderer->SetRenderTarget(selectorRenderTarget, depthBuffer);
		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		selectorRenderTarget->Clear(D3DXCOLOR((UINT)0));
		
		constantBuffer->VSBindToRegisterBN(0);
		constantBuffer->PSBindToRegisterBN(0);

		CallbackRenderModels(onlySelected, shaderSelector, shaderSelector);
		renderer->SetRenderTarget(renderTarget, depthBuffer);
	}

	unsigned __int32 ModelSceneRenderer::ToolGetSelectorResult(int x, int y)
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

	void ModelSceneRenderer::ToolSetWorldMatrix(const D3DXMATRIX& worldMatrix)
	{
		UpdateConstantBuffer(worldMatrix);
	}

	void ModelSceneRenderer::ToolDrawEditorMode()
	{
		UpdateEditorMode();
	}

	void ModelSceneRenderer::Constructor()
	{
		clientSize=WindowGetClient(editorControl);
		Initialize();
		ViewReset();
	}

	void ModelSceneRenderer::Destructor()
	{
		Finalize();
	}

	ModelSceneRenderer::ModelSceneRenderer(HWND _editorControl, const WString& _workingDirectory)
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
		,geometryAxisLineGlobal(0)
		,geometryAxisLineLocal(0)
		,geometryAxisObject(0)
		,shaderAxis(0)
		,shaderObject(0)
		,shaderSelectedObject(0)
		,shaderSelector(0)
		,editorModeTexture(0)
		,editorModeRectangle(0)
		,editorModeShader(0)
		,editorModeSampler(0)
	{
	}

	ModelSceneRenderer::~ModelSceneRenderer()
	{
	}

	DirectXEnvironment* ModelSceneRenderer::Env()
	{
		return env;
	}

	void ModelSceneRenderer::Resize()
	{
		clientSize=WindowGetClient(editorControl);
		Finalize();
		Initialize();
		CallbackRebuildModels();
	}

	void ModelSceneRenderer::Render()
	{
		CallbackRenderSelectorSelected();

		viewport->SetViewport(0, 0, clientSize.cx, clientSize.cy, (float)D3DX_PI/4, 0.1f, 1000.0f);
		depthBuffer->Clear();
		renderTarget->Clear(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		
		constantBuffer->VSBindToRegisterBN(0);
		constantBuffer->PSBindToRegisterBN(0);
		selectorBuffer->PSBindToRegisterTN(0);

		CallbackRenderModels(false, shaderObject, shaderSelectedObject);
		RenderAxisLine();
		RenderAxisObject();
		RenderEditorMode();

		env->swapChain->Present(0, 0);
	}

	void ModelSceneRenderer::ViewReset()
	{
		viewAt=D3DXVECTOR3(0, 0, 0);
		viewAngleVertical=asinf(1/1.732f);
		viewAngleHorizontal=-(float)D3DX_PI*3/4;
		viewDistance=20;
		ViewCalculateDirection();
	}

	void ModelSceneRenderer::ViewRotateVertical(float angle)
	{
		viewAngleVertical+=angle;
		ViewCalculateDirection();
	}

	void ModelSceneRenderer::ViewRotateHorizontal(float angle)
	{
		viewAngleHorizontal+=angle;
		ViewCalculateDirection();
	}

	void ModelSceneRenderer::ViewMove(float left, float up, float front)
	{
		viewAt=D3DXVECTOR3(
			viewAt.x + left*viewLeft.x + up*viewUp.x + front*viewFront.x,
			viewAt.y + left*viewLeft.y + up*viewUp.y + front*viewFront.y,
			viewAt.z + left*viewLeft.z + up*viewUp.z + front*viewFront.z
			);
	}

	D3DXVECTOR3 ModelSceneRenderer::GetViewAt()
	{
		return viewAt;;
	}

	D3DXVECTOR3 ModelSceneRenderer::GetViewFront()
	{
		return viewFront;
	}

	D3DXVECTOR3 ModelSceneRenderer::GetViewLeft()
	{
		return viewLeft;
	}

	D3DXVECTOR3 ModelSceneRenderer::GetViewUp()
	{
		return viewUp;
	}

	float ModelSceneRenderer::GetViewVertical()
	{
		return viewAngleVertical;
	}

	float ModelSceneRenderer::GetViewHorizontal()
	{
		return viewAngleHorizontal;
	}

	float ModelSceneRenderer::GetViewDistance()
	{
		return viewDistance;
	}
}